#include "worker.hpp"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "CxxUrl/url.hpp"

#include "crawler/crawler.hpp"
#include "ANSI-color-codes.h"

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

            try {

            // Parse the url
            Url url(urlstr);

            // Assemble just the scheme and host
            std::stringstream ss;
            ss << url.scheme();
            ss << "://";
            ss << url.host();

            // Full link of website we want to load
            std::string visited_link = ss.str() + url.path();

            // Check if we have visited the site already
            if (webcrawler.has_visited_site(visited_link) == true) continue;
            

            std::cout << CYN << "Popped: " << visited_link << reset << std::endl;

            // First we need to fetch the page
            httplib::Client request(ss.str());
            request.set_connection_timeout(std::chrono::seconds(10));
            request.set_read_timeout(std::chrono::seconds(10));
            request.set_write_timeout(std::chrono::seconds(10));

            // Set the worker state
            this->workerState = WorkerState::FETCHING;
            const auto response = request.Get(url.path().c_str());

            // Now lets parse for links
            this->workerState = WorkerState::PARSING;

            if (response.error() == httplib::Error::Success && response->status >= 200 && response->status < 300) {
                std::string content = response->body;

                std::cout << MAG << "Got: " << visited_link << reset << std::endl;

                // Link regex
                std::regex_iterator<std::string::iterator>::regex_type rx("https?:\\/\\/(www\\.)?"\
                                                                        "[-a-zA-Z0-9@:%._\\+~#=]{1,256}"\
                                                                        "\\.[a-zA-Z0-9()]{1,6}\\b([-a-z"\
                                                                        "A-Z0-9()@:%_\\+.~#?&//=]*)");

                // Link iterator
                std::regex_iterator<std::string::iterator> n(content.begin(), content.end(), rx), e;

                // Loop over all results
                while(n != e) {
                    auto str = n->str();

                    // If we have not visited the site yet, append it to the list
                    if (webcrawler.has_visited_site(str) == false && is_bad_link(str) == false) {
                        webcrawler.ts_push_url(str);
                        webcrawler.ts_push_visited(visited_link);

                        std::cout << BLU << "Pushed: " << str << reset << std::endl;
                    }

                    // Increment the iterator
                    n++;
                }
            } else {
                // Website is invalid, don't try to load it again
                webcrawler.ts_push_invalid_site(visited_link);

                if (response.error() == httplib::Error::Success) {
                    std::cout << RED << "Response code " << response->status << " for " << visited_link << reset << std::endl;
                } else {
                    std::cout << RED << "Error code " << static_cast<int>(response.error()) << " for " << visited_link << reset << std::endl;

                }
            }

            // Now were done so lets just return a empty array
            this->workerState = WorkerState::WAITING_WORK;

            } catch(Url::parse_error e) {
                std::cout << RED << "Could not parse url: " << urlstr << reset << std::endl;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(700));
    }
}

bool WebcrawlerWorker::is_bad_link(std::string url) const {
    for (auto ext : bad_exts) {
        if (url.find(ext) != std::string::npos) {
            return true;
        }
    }

    return false;
}