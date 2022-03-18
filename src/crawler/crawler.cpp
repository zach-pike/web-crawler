#include "crawler.hpp"
#include "worker/worker.hpp"

#include <iostream>

Webcrawler::Webcrawler(std::initializer_list<std::string> list) {
    for (auto i : list) {
        ts_push_url(i);
    }

    // Now lets initialize all our workers
    for (int i=0; i < MAX_N_OF_WORKERS; i++) {
        workers.push_back(std::make_shared<WebcrawlerWorker>());
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

void Webcrawler::assign_work() {
    for (int i = 0; i < workers.size(); i++) {
        // We have assigned all the work possible
        if (websites.empty()) break;

        // Get the first value
        std::string url = websites.front();
        websites.pop();

        workers.at(i)->assignWork(url, [](std::vector<std::string> links) {
            std::cout << "Finished\n";
        });
    }
}