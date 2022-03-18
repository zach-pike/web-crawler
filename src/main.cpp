#include <cstdio>
#include <iostream>

#include "crawler/crawler.hpp"
#include "CxxUrl/url.hpp"

int main(int argc, char const *argv[]) {
    Webcrawler crawler = { "https://www.minecraftforum.net/" };

    crawler.assign_work();

    return 0;
}
