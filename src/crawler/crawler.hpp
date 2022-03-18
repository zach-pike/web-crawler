#ifndef __CRAWLER_H
#define __CRAWLER_H

#include <vector>
#include <thread>
#include <mutex>
#include <deque>
#include <string>
#include <memory>

#include "worker/worker.hpp"
#include "svector/svector.hpp"

class Webcrawler {
    private:
        std::mutex url_queue_lock;
        std::deque<std::string> url_queue;
        
        std::mutex visited_sites_lock;
        SerializableStringVector visited_sites;

        std::mutex invalid_sites_lock;
        std::vector<std::string> invalid_sites;
        

        std::string folder_path = "crawler/";

        std::vector<std::shared_ptr<WebcrawlerWorker>> workers;
    public:
        Webcrawler(std::initializer_list<std::string> initial_urls);
        Webcrawler();
        ~Webcrawler();
        
        // Thread Safe url pop function
        [[nodiscard]]
        std::string ts_pop_url();

        // Thread safe URL push function
        void ts_push_url(std::string url);

        // Return the size of the url queue
        size_t getQueueSize() const;

        // Load saved content from the files
        void load_from_files();

        // Push a url to the visited sites list
        void ts_push_visited(std::string url);

        // Save content found to files
        void save_to_files();

        // Set the save location of the files
        void set_save_location(std::string f);

        // Starts all the workers
        void start_workers(int n_of_workers);

        // Stop all the workers
        void stop_workers();

        // Return if the Webcrawler is working
        bool is_running() const;

        void ts_push_invalid_site(std::string url);

        bool has_visited_site(std::string url);
};

#endif