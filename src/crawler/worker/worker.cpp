#include "worker.hpp"
#include "httplib.h"

#include <iostream>

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
    // First we need to fetch the page
    httplib::Client request(this->fetchUrl);

    this->workerState = WorkerState::FETCHING;
    const auto response = request.Get("");

    // Now lets just print out the text
    this->workerState = WorkerState::PARSING;
    std::cout << "Got: " << response->status << std::endl;
    
    notifier(std::vector<std::string>());

    // Now were done so lets just return a empty array
    this->workerState = WorkerState::WAITING_WORK;
}