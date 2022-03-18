#include <cstdio>
#include <iostream>

#include "crawler/crawler.hpp"
#include "CxxUrl/url.hpp"

int main(int argc, char const *argv[]) {
    Webcrawler crawler = { "https://www.minecraftforum.net/" };

    crawler.start();

    while (true) {
        if (getchar() == 'e') {
            crawler.stop();
        }
    }

    return 0;
}
