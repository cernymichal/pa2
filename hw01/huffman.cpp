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

    bool eof() const {
        return fstream.eof() && currentBit == 8;
    }

    void resetRead() {
        currentBit = 8;
        currentByte = '\0';
        fstream.clear();
        fstream.seekg(0);
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

        int loadedBits = 0;
        for (; loadedBits < n && good(); loadedBits++)
            result |= (uint64_t)getBit() << (63 - loadedBits);

        if (loadedBits != n)
            return false;

        target = result >> (64 - loadedBits);
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
        uint32_t result = 0;

        int byteCount = 0;
        // after loading the first byte check for loading ones
        for (; byteCount < 4 && (byteCount == 0 || result >= ~0u << (31 - byteCount)); byteCount++) {
            uint8_t byte;
            if (!getByte(byte))
                return false;

            // check if consecutive bytes look like 10xxxxxx
            if (byteCount != 0)
                if ((byte & 0xc0) != 0x80) return false;

            // add newly read byte to result
            result |= (uint32_t)byte << ((3 - byteCount) * 8);
        }

        // check for leading zero if 1 byte
        if (byteCount == 1 && (((uint32_t)1 << 31) & result) != 0)
            return false;

        // check for zero after number of bytes 1110xxxx
        if (byteCount != 1 && (result >> (31 - byteCount)) % 2 != 0)
            return false;

        // if encoding is shorter than 4 bytes move value backwards;
        result = result >> (4 - byteCount) * 8;

        // check if result is within unicode code-space
        // U+10FFFF = 0xf48fbfbf in UTF8
        if (result > 0xf48fbfbf)
            return false;

        target = result;

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
    for (int currentByte = 1; currentByte < 5; currentByte++) {
        // check if byte is used
        // always print last byte
        if (currentByte == 4 || c > ~(char32_t)0 >> (8 * currentByte))
            stream << (uint8_t)(c >> (32 - 8 * currentByte));
    }

    return stream;
}

class TreeNode {
    TreeNode *parent = nullptr;
    TreeNode *left = nullptr;
    TreeNode *right = nullptr;

#ifdef PT_DEBUG

    void print(const std::string &prefix, bool isLeft) const;

#endif /* PT_DEBUG */

public:
    char32_t character = '\0';
    uint64_t count = 1;

    TreeNode() {
    }

    TreeNode(char32_t character) : character(character) {
    }

    ~TreeNode() {
        delete left;
        delete right;
    }

    // load tree from bit stream
    bool load(BitFStream &stream) {
        if (!stream.good())
            return false;

        // check if is leaf
        if (stream.getBit())
            return stream.getUTF8Char(character);

        // is node -> load branches
        left = new TreeNode();
        right = new TreeNode();

        return left->load(stream) && right->load(stream);
    }

    // todo
    bool write(BitFStream &stream) {
        if (!stream.good())
            return false;

        // check if is leaf
        if (stream.getBit())
            return stream.getUTF8Char(character);

        // is node -> load branches
        left = new TreeNode();
        right = new TreeNode();

        return left->load(stream) && right->load(stream);
    }

    bool findChar(BitFStream &stream, char32_t &target) const {
        if (!this->left && !this->right) {
            target = character;
            return true;
        }

        if (!stream.good())
            return false;

        if (!stream.getBit())
            return this->left && this->left->findChar(stream, target);
        else
            return this->right && this->right->findChar(stream, target);
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

    if (!inStream.good())
        return false;

    // if 1st bit is 0 get chunk size
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

    // load character tree
    TreeNode tree;
    if (!tree.load(inStream))
        return false;

    PRINT_TREE(tree);

    std::ofstream outStream(outFileName);

    // decompress chunks
    uint16_t chunkSize = 4096;
    while (chunkSize == 4096) {
        if (!decompressChunk(tree, inStream, outStream, chunkSize))
            return false;
    }

    // last chunk should be less than 4096 characters
    return chunkSize != 4096;
}

bool compressFile(const char *inFileName, const char *outFileName) {
    BitFStream inStream(inFileName, std::ios::in);

    if (!inStream.good())
        return false;

    // chars and their coresponding TreeNodes
    std::map<char32_t, TreeNode *> charMap;

    // create unconnected TreeNodes for each char in inFile
    while (inStream.good()) {
        char32_t c;
        if (!inStream.getUTF8Char(c))
            return false;

        auto search = charMap.find(c);
        if (search == charMap.end())
            charMap[c] = new TreeNode(c);
        else
            search->second->count++;
    }

    // create priority queue of nodes sorted by their count
    auto compareTreeNodes = [](const TreeNode *left, const TreeNode *right) {
        return left->count < right->count;
    };
    std::priority_queue<TreeNode *, std::vector<TreeNode *>, decltype(compareTreeNodes)> nodeQueue(compareTreeNodes);

    for (auto const &pair : charMap)
        nodeQueue.push(pair.second);

    while (!nodeQueue.empty()) {
        auto c = nodeQueue.top();
        std::cout << c->character << " : " << c->count << std::endl;
        nodeQueue.pop();
    }

    inStream.resetRead();

    return true;
}
