#include <cstdio>
#include <iostream>

#include "crawler/crawler.hpp"
#include "CxxUrl/url.hpp"
#include "svector/svector.hpp"

int main(int argc, char const *argv[]) {
    Webcrawler crawler;
    // crawler.ts_push_url("https://www.minecraftforum.net/");

    crawler.load_from_files();

    crawler.start_workers(1);

    std::this_thread::sleep_for(std::chrono::seconds(5));

    crawler.stop_workers();

    crawler.save_to_files();

    return 0;
}
