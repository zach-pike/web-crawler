#include <cstdio>
#include <iostream>

#include "crawler/crawler.hpp"
#include "CxxUrl/url.hpp"

int main(int argc, char const *argv[]) {
    Webcrawler crawler = { "https://www.minecraftforum.net/" };

    crawler.start();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    crawler.stop();

    return 0;
}
