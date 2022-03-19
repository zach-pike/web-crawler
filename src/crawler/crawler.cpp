#include "crawler.hpp"
#include "worker/worker.hpp"

#include <iostream>
#include <filesystem>
#include <algorithm>

Webcrawler::Webcrawler(std::initializer_list<std::string> list) {
    for (auto i : list) {
        ts_push_url(i);
    }
}

Webcrawler::Webcrawler() {}

Webcrawler::~Webcrawler() {}

bool Webcrawler::is_running() const {
    return workers.size() > 0;
}

size_t Webcrawler::getQueueSize() const {
    return url_queue.size();
}

void Webcrawler::start_workers(int n_of_workers) {
    if (is_running()) return;

    // Now lets initialize all our workers
    workers.push_back(std::make_shared<WebcrawlerWorker>(*this));
}

void Webcrawler::stop_workers() {
    workers.clear();
}

bool Webcrawler::has_visited_site(std::string url) {
    visited_sites_lock.lock();
    invalid_sites_lock.lock();

    bool v = std::count(visited_sites.begin(), visited_sites.end(), url) > 0
          || std::count(invalid_sites.begin(), invalid_sites.end(), url) > 0;

    visited_sites_lock.unlock();
    invalid_sites_lock.unlock();

    return v;
}

// Set the save location of the bin files
void Webcrawler::set_save_location(std::string s) {
    this->folder_path = s;
}

// (Thread Safe) Push a invalid site
void Webcrawler::ts_push_invalid_site(std::string url) {
    invalid_sites_lock.lock();

    invalid_sites.push_back(url);

    invalid_sites_lock.unlock();
}

// (Thread Safe) push a url into the visited list
void Webcrawler::ts_push_visited(std::string url) {
    visited_sites_lock.lock();

    if (std::count(visited_sites.begin(), visited_sites.end(), url) == 0) {
        visited_sites.push_back(url);
    }

    visited_sites_lock.unlock();
}

// (Thread Safe) push a url into the url queue
void Webcrawler::ts_push_url(std::string url) {
    // Lock the mutexes
    url_queue_lock.lock();

    if (std::count(url_queue.begin(), url_queue.end(), url) == 0) {
        url_queue.push_back(url);
    }

    // Unlock mutexes
    url_queue_lock.unlock();
}

// (Thread Safe) pop a url from the list
std::string Webcrawler::ts_pop_url() {
    url_queue_lock.lock();

    std::string url = url_queue.front();
    url_queue.pop_front();

    url_queue_lock.unlock();
    return url;
}