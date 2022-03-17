#ifndef __CRAWLER_H
#define __CRAWLER_H

#define MAX_N_OF_WORKERS 1

#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <string>

class Webcrawler {
    private:
        std::mutex queue_lock;
        std::queue<std::string> websites;

        std::vector<void> workers;
    public:
        Webcrawler(std::initializer_list<std::string> initial_urls);
        
        // Thread Safe url pop function
        [[nodiscard]]
        std::string ts_pop_url();

        // Thread safe URL push function
        void ts_push_url(std::string url);

};

#endif