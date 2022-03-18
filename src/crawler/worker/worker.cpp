#include "worker.hpp"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "CxxUrl/url.hpp"

#include "crawler/crawler.hpp"

#include <iostream>
#include <sstream>
#include <chrono>
#include <regex>

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

            std::string visited_link = ss.str() + url.path();

            if (webcrawler.has_visited_site(visited_link) == true) continue;

            // First we need to fetch the page
            httplib::Client request(ss.str());

            // Set the worker state
            this->workerState = WorkerState::FETCHING;
            const auto response = request.Get(url.path().c_str());

            // Now lets parse for links
            this->workerState = WorkerState::PARSING;

            if (response.error() == httplib::Error::Success && response->status >= 200 && response->status < 300) {
                std::string content = response->body;

                std::cout << "Got: " << visited_link << std::endl;
            
                std::regex_iterator<std::string::iterator>::regex_type rx("https?:\\/\\/(www\\.)?"\
                                                                        "[-a-zA-Z0-9@:%._\\+~#=]{1,256}"\
                                                                        "\\.[a-zA-Z0-9()]{1,6}\\b([-a-z"\
                                                                        "A-Z0-9()@:%_\\+.~#?&//=]*)");

                std::regex_iterator<std::string::iterator> n(content.begin(), content.end(), rx), e;

                while(n != e) {
                    auto str = n->str();

                    if (webcrawler.has_visited_site(str) == false) {
                        webcrawler.ts_push_url(n->str());
                        webcrawler.ts_push_visited(visited_link);
                    }

                    n++;
                }
            } else {
                webcrawler.ts_push_invalid_site(visited_link);

                if (response.error() == httplib::Error::Success) {
                    printf("Response code %d\n", response->status);
                } else {
                    printf("Error number %d\n", static_cast<int>(response.error()));
                }
            }

            // Now were done so lets just return a empty array
            this->workerState = WorkerState::WAITING_WORK;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
}