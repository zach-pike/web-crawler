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
        std::vector<std::string> bad_exts = { ".jpg", ".png", ".mp4", ".mp3", ".js", ".jpeg", ".css", ".ico", ".mod" };
        bool is_bad_link(std::string url) const;

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