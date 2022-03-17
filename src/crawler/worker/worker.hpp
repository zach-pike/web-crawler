#ifndef __WORKER_H
#define __WORKER_H

#include <thread>
#include <functional>

class WebcrawlerWorker {
    public:
        enum class WorkerState {
            WAITING_WORK,
            FETCHING,
            PARSING
        };
    private:
        WorkerState workerState = WorkerState::WAITING_WORK;
        std::thread workerThread;
        std::string fetchUrl;

        void workerThread();

    public:
        WebcrawlerWorker();
        ~WebcrawlerWorker();

        // Assign a asyncronous crawler work
        void assignWork(std::string url, std::function<void(std::vector<std::string>)> notifier);
};

#endif