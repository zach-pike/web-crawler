#include <cstdio>
#include <iostream>

#include "crawler/crawler.hpp"
#include "CxxUrl/url.hpp"
#include "svector/svector.hpp"

int main(int argc, char const *argv[]) {
    Webcrawler crawler = { "https://www.minecraftforum.net/" };

    crawler.start_workers(1);

    std::this_thread::sleep_for(std::chrono::seconds(10));

    crawler.stop_workers();

    return 0;
}
