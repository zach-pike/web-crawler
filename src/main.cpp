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

    crawler.start_workers(1);

    std::this_thread::sleep_for(std::chrono::minutes(5));

    crawler.stop_workers();

    crawler.save_to_files();

    // SerializableStringVector vec;

    // // Load sites
    // std::ifstream file("crawler/visited_sites.bin", std::ios::binary | std::ios::in );
    // size_t size = std::filesystem::file_size("crawler/visited_sites.bin");

    // uint8_t* buf = new uint8_t[size];
    // file.read((char*)buf, size);

    // SerializableStringVector::ByteArray v(buf, buf + size);
    // vec.from_bytearray(v);

    // for (auto str : vec) {
    //     std::cout << str << std::endl;
    // }

    return 0;
}
