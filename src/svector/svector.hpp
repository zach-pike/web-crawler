#ifndef __SVECTOR_H
#define __SVECTOR_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class SerializableStringVector : public std::vector<std::string> {
    public:
        using ByteArray = std::vector<uint8_t>;

        struct VectorInfo {
            uint32_t datalen;
        };

        void to_bytearray(ByteArray& bytes) {
            ByteArray arrbytes;

            // Append all bytes of strings to the array
            for (auto s : *this) {
                ByteArray sbytes(s.begin(), s.end());
                arrbytes.insert(arrbytes.end(), sbytes.begin(), sbytes.end());

                // Insert a null terminator
                arrbytes.push_back(0);
            }

            // Set datasize
            VectorInfo vecinfo;
            vecinfo.datalen = arrbytes.size();

            // Append to master array
            ByteArray vecinfoarr(reinterpret_cast<uint8_t*>(&vecinfo), reinterpret_cast<uint8_t*>(&vecinfo) + sizeof(VectorInfo));

            bytes.insert(bytes.end(), vecinfoarr.begin(), vecinfoarr.end());
            bytes.insert(bytes.end(), arrbytes.begin(), arrbytes.end());
        }

        void from_bytearray(const ByteArray& bytes) {
            // Get the vector info
            VectorInfo vecinfo;
            memcpy(&vecinfo, bytes.data(), sizeof(VectorInfo));

            // Get the data
            ByteArray data(bytes.begin() + sizeof(VectorInfo), bytes.end());

            // Convert to string
            std::string str(data.begin(), data.end());

            // Split the string
            std::stringstream ss(str);
            std::string item;
            while (std::getline(ss, item, '\0')) {
                this->push_back(item);
            }
        }
};

#endif