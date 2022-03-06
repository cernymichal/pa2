#ifndef __PROGTEST__

#include <algorithm>
#include <cassert>
#include <cctype>
#include <climits>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <stdexcept>
#include <vector>

#endif /* __PROGTEST__ */

// fstream wrapper for reading single or multiple bits
class BitFStream {
    std::fstream fstream;
    uint8_t currentBit = 8;
    uint8_t currentByte = '\0';

    void loadByte() {
        currentBit = 0;
        currentByte = fstream.get();
    }

public:
    BitFStream(const char *fileName, std::ios_base::openmode mode) : fstream(fileName, mode | std::ios::binary) {
    }

    bool good() const {
        return fstream.good() && !eof();
    }

    bool fail() const {
        return fstream.fail();
    }

    bool eof() const {
        return fstream.eof() && currentBit == 8;
    }

    uint8_t getBit() {
        if (currentBit % 8 == 0)
            loadByte();

        return (currentByte >> (7 - currentBit++)) & (uint8_t)1u;
    }

    // max n = 64
    // could be made faster rather than reading bit by bit
    bool getNBits(uint8_t n, uint64_t &target) {
        target = 0;

        int i = 0;
        for (; i < n && good(); i++)
            target |= (uint64_t)getBit() << (63 - i);

        target >>= 64 - i;
        return good();
    }

    bool getByte(uint8_t &target) {
        uint64_t temp;
        auto res = getNBits(8, temp);
        target = temp;
        return res;
    }

    // https://en.wikipedia.org/wiki/UTF-8#Encoding
    bool getUTF8Char(char32_t &target) {
        target = 0;
        bool res = true;

        int i = 0;
        // after loading the first byte check for loading ones
        for (; res && (i == 0 || target >= ~0u << (31 - i)); i++) {
            uint8_t byte;
            res = getByte(byte);

            // check if consecutive bytes look like 10xxxxxx
            if (i != 0)
                res = res && byte >> 6 == 2;

            // add newly read byte to target
            target |= (uint32_t)byte << (8 * (3 - i));
        }

        // if encoding is shorter than 4 bytes move value backwards;
        target >>= (4 - i) * 8;

        return res;
    }

    BitFStream &operator>>(uint8_t &variable) {
        variable = getBit();
        return *this;
    }

    BitFStream &operator>>(char &variable) {
        getByte((uint8_t &)variable);
        return *this;
    }

    BitFStream &operator>>(char32_t &variable) {
        getUTF8Char(variable);
        return *this;
    }
};

// add utf8 encoded char to stream as a variable number of normal chars
// https://en.wikipedia.org/wiki/UTF-8#Encoding
std::ostream &operator<<(std::ostream &stream, char32_t const &c) {
    // starting from the front send bytes
    for (int i = 1; i < 5; i++) {
        // check if byte is used
        if (i == 4 || c > 1u << (32 - 8 * i - 1))
            stream << (char)(c >> (32 - 8 * i));
    }

    return stream;
}

class TreeNode {
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
    char32_t character = '\0';

public:
    // load tree from bit stream
    void load(BitFStream &stream) {
        if (stream.getBit())
            stream >> character;
        else {
            left = std::make_unique<TreeNode>();
            right = std::make_unique<TreeNode>();
            left->load(stream);
            right->load(stream);
        }
    }

    char32_t findChar(BitFStream &stream) {
        return ' ';
    }

    void addChar(const char32_t &character, const uint8_t &path) {
    }
};

bool decompressFile(const char *inFileName, const char *outFileName) {
    BitFStream inStream(inFileName, std::ios::in);

    if (!inStream.good())
        return false;

    TreeNode tree;
    tree.load(inStream);

    return true;
}

bool compressFile(const char *inFileName, const char *outFileName) {
    // keep this dummy implementation (no bonus) or implement the compression
    // (bonus)
    return false;
}
