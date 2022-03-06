#ifndef __PROGTEST__

#pragma once

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

#ifdef PT_DEBUG

#define PRINT_TREE(tree) tree.print()

#else /* PT_DEBUG */

#define PRINT_TREE(tree)

#endif /* PT_DEBUG */

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
        uint64_t result = 0;

        int i = 0;
        for (; i < n && good(); i++)
            result |= (uint64_t)getBit() << (63 - i);

        if (i != n)
            return false;

        target = result >> (64 - i);
        return true;
    }

    bool getByte(uint8_t &target) {
        uint64_t temp;

        if (!getNBits(8, temp))
            return false;

        target = temp;
        return true;
    }

    // https://en.wikipedia.org/wiki/UTF-8#Encoding
    bool getUTF8Char(char32_t &target) {
        uint64_t result = 0;
        bool success = true;

        int i = 0;
        // after loading the first byte check for loading ones
        for (; success && (i == 0 || result >= ~0u << (31 - i)); i++) {
            uint8_t byte;
            success = getByte(byte);

            // check if consecutive bytes look like 10xxxxxx
            if (i != 0)
                success = success && byte >> 6 == 2;

            // add newly read byte to result
            result |= (uint32_t)byte << (8 * (3 - i));
        }

        if (!success)
            return false;

        // if encoding is shorter than 4 bytes move value backwards;
        target = result >> (4 - i) * 8;

        return true;
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
        if (i == 4 || c > ~(char32_t)0 >> (8 * i))
            stream << (uint8_t)(c >> (32 - 8 * i));
    }

    return stream;
}

class TreeNode {
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
    char32_t character = '\0';

#ifdef PT_DEBUG

    void print(const std::string &prefix, bool isLeft) const;

#endif /* PT_DEBUG */

public:
    // load tree from bit stream
    bool load(BitFStream &stream) {
        if (!stream.good())
            return false;

        bool success = true;

        if (stream.getBit())
            success = success && stream.getUTF8Char(character);
        else {
            left = std::make_unique<TreeNode>();
            success = success && left->load(stream);

            if (!success || !stream.good())
                return false;

            right = std::make_unique<TreeNode>();
            success = success && right->load(stream);
        }

        return success;
    }

    bool findChar(BitFStream &stream, char32_t &target) const {
        if (!this->left && !this->right) {
            target = character;
            return true;
        }

        if (!stream.good())
            return false;

        if (!stream.getBit())
            return this->left ? this->left->findChar(stream, target) : false;
        else
            return this->right ? this->right->findChar(stream, target) : false;
    }

    void addChar(const char32_t &character, const uint8_t &path) {
        // todo
    }

#ifdef PT_DEBUG

    void print() const;

#endif /* PT_DEBUG */
};

bool decompressChunk(TreeNode &tree, BitFStream &inStream, std::ofstream &outStream, uint16_t &chunkSize) {
    chunkSize = 4096;

    if (!inStream.getBit()) {
        uint64_t temp;
        if (!inStream.getNBits(12, temp))
            return false;

        chunkSize = temp;
    }

    for (int i = 0; i < chunkSize; i++) {
        char32_t c;

        if (!tree.findChar(inStream, c) || !outStream.good())
            return false;

        outStream << c;
    }

    return true;
}

bool decompressFile(const char *inFileName, const char *outFileName) {
    BitFStream inStream(inFileName, std::ios::in);

    TreeNode tree;
    if (!tree.load(inStream))
        return false;

    PRINT_TREE(tree);

    std::ofstream outStream(outFileName);

    uint16_t chunkSize = 4096;
    bool success = true;
    while (chunkSize == 4096 && inStream.good() && outStream.good() && success)
        success = decompressChunk(tree, inStream, outStream, chunkSize);

    return chunkSize != 4096 && success;
}

bool compressFile(const char *inFileName, const char *outFileName) {
    // keep this dummy implementation (no bonus) or implement the compression
    // (bonus)
    return false;
}
