#ifndef __WORKER_H
#define __WORKER_H

#include <thread>
#include <atomic>
#include <functional>

// C++ moment
class Webcrawler;

class WebcrawlerWorker {
    public:

        enum class WorkerState {
            WAITING_WORK,
            FETCHING,
            PARSING
        };
    private:
        std::atomic<WorkerState> workerState = WorkerState::WAITING_WORK;

        bool threadShouldRun = true;
        std::thread workerThread;
        Webcrawler& webcrawler;

        void workerThreadFunc();

    public:
        WebcrawlerWorker(Webcrawler& webcrawler);
        ~WebcrawlerWorker();
};

#endif