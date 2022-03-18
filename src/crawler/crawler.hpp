#ifndef __CRAWLER_H
#define __CRAWLER_H

#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <string>
#include <memory>

#include "worker/worker.hpp"

class Webcrawler {
    private:
        std::mutex queue_lock;
        
        std::queue<std::string> websites;

        std::vector<std::shared_ptr<WebcrawlerWorker>> workers;
    public:
        Webcrawler(std::initializer_list<std::string> initial_urls);
        
        // Thread Safe url pop function
        [[nodiscard]]
        std::string ts_pop_url();

        // Thread safe URL push function
        void ts_push_url(std::string url);

        size_t getQueueSize() const;

        // Starts all the workers
        void start_workers(int n_of_workers);

        // Stop all the workers
        void stop_workers();
};

#endif