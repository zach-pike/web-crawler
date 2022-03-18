#include "worker.hpp"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "CxxUrl/url.hpp"

#include <iostream>
#include <sstream>

void WebcrawlerWorker::assignWork(std::string url, NotifierFunc notifier) {
    fetchUrl = url;
    threadInitialized = true;
    workerThread = std::thread(std::bind(&WebcrawlerWorker::workerThreadFunc, this, notifier));
}

WebcrawlerWorker::WebcrawlerWorker() {}

WebcrawlerWorker::~WebcrawlerWorker() {
    if (threadInitialized) workerThread.join();
}

void WebcrawlerWorker::workerThreadFunc(NotifierFunc notifier) {

    // Parse the url
    Url url(this->fetchUrl);

    std::stringstream ss;
    ss << url.scheme();
    ss << "://";
    ss << url.host();

    // First we need to fetch the page
    httplib::Client request(ss.str());

    this->workerState = WorkerState::FETCHING;
    const auto response = request.Get(url.path().c_str());

    // Now lets just print out the text
    this->workerState = WorkerState::PARSING;
    std::cout << "Got: " << response->body << std::endl;
    
    notifier(std::vector<std::string>());

    // Now were done so lets just return a empty array
    this->workerState = WorkerState::WAITING_WORK;
}