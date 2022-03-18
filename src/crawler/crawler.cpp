#include "crawler.hpp"
#include "worker/worker.hpp"

#include <iostream>

Webcrawler::Webcrawler(std::initializer_list<std::string> list) {
    for (auto i : list) {
        ts_push_url(i);
    }
}

void Webcrawler::ts_push_url(std::string url) {
    queue_lock.lock();
    websites.push(url);
    queue_lock.unlock();
}

std::string Webcrawler::ts_pop_url() {
    queue_lock.lock();

    std::string url = websites.front();
    websites.pop();

    queue_lock.unlock();
    return url;
}

size_t Webcrawler::getQueueSize() const {
    return websites.size();
}

void Webcrawler::start() {
    // Now lets initialize all our workers
    for (int i=0; i < MAX_N_OF_WORKERS; i++) {
        workers.push_back(std::make_shared<WebcrawlerWorker>(*this));
    }
}

void Webcrawler::stop() {
    // To be done
}