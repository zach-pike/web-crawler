#include "worker.hpp"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "CxxUrl/url.hpp"

#include "crawler/crawler.hpp"

#include <iostream>
#include <sstream>
#include <chrono>

WebcrawlerWorker::WebcrawlerWorker(Webcrawler& c)
    : webcrawler{c} {
        workerThread = std::thread(std::bind(&WebcrawlerWorker::workerThreadFunc, this));
    }

WebcrawlerWorker::~WebcrawlerWorker() {
    threadShouldRun = false;
    workerThread.join();
}

void WebcrawlerWorker::workerThreadFunc() {
     while (this->threadShouldRun) {
        if (webcrawler.getQueueSize() == 0) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        } else {
            // Get a url
            std::string urlstr = webcrawler.ts_pop_url();

            Url url(urlstr);

            std::stringstream ss;
            ss << url.scheme();
            ss << "://";
            ss << url.host();

            // First we need to fetch the page
            httplib::Client request(ss.str());

            // Set the worker state
            this->workerState = WorkerState::FETCHING;
            const auto response = request.Get(url.path().c_str());

            // Now lets just print out the text
            this->workerState = WorkerState::PARSING;
            
            // Parse the XML here for links of any type
            std::vector<std::string> links_found;

            // Now were done so lets just return a empty array
            this->workerState = WorkerState::WAITING_WORK;
        }
    }
}