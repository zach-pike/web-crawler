#include <cstdio>
#include <iostream>
#include <filesystem>

#include "crawler/crawler.hpp"
#include "CxxUrl/url.hpp"
#include "svector/svector.hpp"

#include "ANSI-color-codes.h"

int main(int argc, char const *argv[]) {
    Webcrawler crawler;
    crawler.load_from_files();

    // // seed links
    // crawler.ts_push_url("https://reddit.com");
    // crawler.ts_push_url("https://www.minecraftforum.net/");
    // crawler.ts_push_url("https://knowyourmeme.com/");

    crawler.start_workers(10);

    getchar();
    std::cout << GRN << "Stopping crawlers!" << reset << std::endl;

    crawler.stop_workers();

    crawler.save_to_files();

    return 0;
}
