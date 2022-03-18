#ifndef __WORKER_H
#define __WORKER_H

#include <thread>
#include <atomic>
#include <functional>

class WebcrawlerWorker {
    public:
        using NotifierFunc = std::function<void(std::vector<std::string>)>;

        enum class WorkerState {
            WAITING_WORK,
            FETCHING,
            PARSING
        };
    private:
        std::atomic<WorkerState> workerState = WorkerState::WAITING_WORK;

        bool threadInitialized = false;
        std::thread workerThread;
        std::string fetchUrl;

        void workerThreadFunc(NotifierFunc notifier);

    public:
        WebcrawlerWorker();
        ~WebcrawlerWorker();

        // Assign a asyncronous crawler work
        void assignWork(std::string url, NotifierFunc notifier);
};

#endif