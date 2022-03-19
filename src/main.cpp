#include <cstdio>
#include <iostream>
#include <filesystem>

#include "crawler/crawler.hpp"
#include "CxxUrl/url.hpp"
#include "svector/svector.hpp"

int main(int argc, char const *argv[]) {
    Webcrawler crawler;
    crawler.ts_push_url("https://www.minecraftforum.net/");
    crawler.ts_push_url("https://www.msn.com/en-us");

    crawler.start_workers(10);

    std::this_thread::sleep_for(std::chrono::seconds(5));

    crawler.stop_workers();

    // crawler.save_to_files();

    return 0;
}
