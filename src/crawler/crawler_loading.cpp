#include "crawler.hpp"

#include <iostream>
#include <filesystem>

void Webcrawler::save_to_files() {
    if (is_running()) throw std::runtime_error("Cannot save to file while workers are working");

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
    if (is_running()) throw std::runtime_error("Cannot save to file while workers are working");

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