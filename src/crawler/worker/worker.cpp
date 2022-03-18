#include "worker.hpp"
#include "HTTPRequest.hpp"

#include <iostream>

void WebcrawlerWorker::assignWork(std::string url, NotifierFunc notifier) {
    fetchUrl = url;
    threadInitialized = true;
    workerThread = std::thread(std::bind(&WebcrawlerWorker::workerThreadFunc, this), notifier);
}

WebcrawlerWorker::~WebcrawlerWorker() {
    if (threadInitialized) workerThread.join();
}

void WebcrawlerWorker::workerThreadFunc(NotifierFunc notifier) {
    // First we need to fetch the page
    http::Request request(this->fetchUrl);

    this->workerState = WorkerState::FETCHING;
    const auto response = request.send("GET");

    // Now lets just print out the text
    this->workerState = WorkerState::PARSING;
    std::cout << std::string(response.body.begin(), response.body.end()) << std::endl;
    
    notifier(std::vector<std::string>());

    // Now were done so lets just return a empty array
    this->workerState = WorkerState::WAITING_WORK;
}