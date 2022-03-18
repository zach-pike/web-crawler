#include "crawler.hpp"
#include "worker/worker.hpp"

#include <iostream>
#include <filesystem>

Webcrawler::Webcrawler(std::initializer_list<std::string> list) {
    for (auto i : list) {
        ts_push_url(i);
    }
}

Webcrawler::Webcrawler() {}

Webcrawler::~Webcrawler() {}

void Webcrawler::save_to_files() {
    if (!workers.empty()) throw std::runtime_error("Cannot save to file while workers are working");

    // We will convert the queue into a SerializableStringVector
    SerializableStringVector to_be_visited;

    // Move the queue into the string vector
    while (this->url_queue.empty() == false) to_be_visited.push_back(this->ts_pop_url());



    // Convert to byte array
    SerializableStringVector::ByteArray to_be_visited_bytes;
    to_be_visited.to_bytearray(to_be_visited_bytes);

    // Write to file
    std::ofstream to_be_visited_file(this->folder_path + "to_be_visited_sites.bin", std::ios::binary | std::ios::out);
    to_be_visited_file.write((char*)to_be_visited_bytes.data(), to_be_visited_bytes.size());
    


    // Save the visited sites
    SerializableStringVector::ByteArray visited_sites_bytes;
    visited_sites.to_bytearray(visited_sites_bytes);

    // Save to file
    std::ofstream file2(this->folder_path + "visited_sites.bin", std::ios::binary | std::ios::out);
    file2.write((char*)visited_sites_bytes.data(), visited_sites_bytes.size());
}

void Webcrawler::load_from_files() {
    if (!workers.empty()) throw std::runtime_error("Cannot save to file while workers are working");

    // Get file & filesize
    std::ifstream visited(this->folder_path + "visited_sites.bin", std::ios::binary | std::ios::in);
    size_t visited_size = std::filesystem::file_size(this->folder_path + "visited_sites.bin");

    // Read the file
    uint8_t* visited_buffer = new uint8_t[visited_size];
    visited.read((char*)visited_buffer, visited_size);

    // Load into array
    SerializableStringVector::ByteArray visited_bytes(visited_buffer, visited_buffer + visited_size);
    visited_sites.from_bytearray(visited_bytes);

    // Get file & filesize
    std::ifstream to_be_visited(this->folder_path + "to_be_visited_sites.bin", std::ios::binary | std::ios::in);
    size_t to_be_visited_size = std::filesystem::file_size(this->folder_path + "to_be_visited_sites.bin");

    // Read the file
    uint8_t* to_be_visited_buffer = new uint8_t[to_be_visited_size];
    to_be_visited.read((char*)to_be_visited_buffer, to_be_visited_size);

    // Load into array
    SerializableStringVector to_be_visited_array;
    SerializableStringVector::ByteArray to_be_visited_bytes(to_be_visited_buffer, to_be_visited_buffer + to_be_visited_size);
    to_be_visited_array.from_bytearray(to_be_visited_bytes);

    // Push the array into the queue
    for (auto i : to_be_visited_array) {
        this->ts_push_url(i);
    }
}

// Set the save location of the bin files
void Webcrawler::set_save_location(std::string s) {
    this->folder_path = s;
}

// (Thread Safe) push a url into the url queue
void Webcrawler::ts_push_url(std::string url) {
    // Lock the mutexes
    url_queue_lock.lock();

    if (std::count(url_queue.begin(), url_queue.end(), url) == 0) {
        std::cout << "Pushed: " << url << std::endl;
        url_queue.push_back(url);
    }

    // Unlock mutexes
    url_queue_lock.unlock();
}

bool Webcrawler::is_running() const {
    return workers.size() == 0;
}

bool Webcrawler::has_visited_site(std::string url) {
    visited_sites_lock.lock();
    invalid_sites_lock.lock();

    bool v = std::count(visited_sites.begin(), visited_sites.end(), url) != 0 
          || std::count(invalid_sites.begin(), invalid_sites.end(), url) != 0;

    visited_sites_lock.unlock();
    invalid_sites_lock.unlock();

    return v;

}

std::string Webcrawler::ts_pop_url() {
    url_queue_lock.lock();

    std::string url = url_queue.front();
    url_queue.pop_front();

    std::cout << "Popped: " << url << std::endl;

    url_queue_lock.unlock();
    return url;
}

void Webcrawler::ts_push_visited(std::string url) {
    visited_sites_lock.lock();

    visited_sites.push_back(url);

    visited_sites_lock.unlock();
}

size_t Webcrawler::getQueueSize() const {
    return url_queue.size();
}

void Webcrawler::start_workers(int n_of_workers) {
    if (workers.size() > 0) return;

    // Now lets initialize all our workers
    workers.push_back(std::make_shared<WebcrawlerWorker>(*this));
}

void Webcrawler::stop_workers() {
    // To be done
    workers.clear();
}

void Webcrawler::ts_push_invalid_site(std::string url) {
    invalid_sites_lock.lock();

    invalid_sites.push_back(url);

    invalid_sites_lock.unlock();
} 