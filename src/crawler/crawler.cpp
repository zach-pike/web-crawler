#include "crawler.hpp"
#include "worker/worker.hpp"

Webcrawler::Webcrawler(std::initializer_list<std::string> list) {
    for (auto i : list) {
        ts_push_url(i);
    }

    // Now lets initialize all our workers
    for (int i=0; i < MAX_N_OF_WORKERS; i++) {
        workers.push_back(WebcrawlerWorker());
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