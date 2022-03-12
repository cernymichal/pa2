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

#endif  // __PROGTEST__

#ifdef PT_DEBUG

#define PRINT_TREE(tree) (tree).print()

#else  // PT_DEBUG

#define PRINT_TREE(tree)

#endif  // PT_DEBUG

#define HUFF_CHUNK_SIZE 4096
#define HUFF_CHUNK_SIZE_BITS 12

struct BitValue {
    uint64_t value;
    uint8_t length;  // max 64

    BitValue(uint64_t value = 0, uint8_t length = 0) : value(value), length(length) {
    }
};

// fstream wrapper for single bit operations
class BitFStream {
    std::fstream fstream;
    uint8_t currentReadBit = 8;
    uint8_t currentReadByte = 0;
    uint8_t currentWriteBit = 0;
    uint8_t currentWriteByte = 0;

public:
    BitFStream(const char *fileName, std::ios_base::openmode mode) : fstream(fileName, mode | std::ios::binary) {
    }

    ~BitFStream() {
        if (currentWriteBit != 0)
            writeWriteByte();
    }

    bool good() const {
        return fstream.good();
    }

    bool eof() const {
        return fstream.eof();
    }

    void resetRead() {
        currentReadBit = 8;
        currentReadByte = 0;
        fstream.clear();
        fstream.seekg(0);
    }

    uint8_t getBit() {
        if (currentReadBit % 8 == 0)
            getReadByte();

        return (currentReadByte >> (7 - currentReadBit++)) & 1;
    }

    // max n = 64
    bool getNBits(uint8_t n, uint64_t &target) {
        uint64_t result = 0;

        int loadedBits = 0;
        for (; loadedBits < n && good(); loadedBits++)
            result |= (uint64_t)getBit() << (63 - loadedBits);

        if (!good())
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

    void writeBit(uint8_t value) {
        currentWriteByte |= (value & 1) << (7 - currentWriteBit++);

        if (currentWriteBit == 8)
            writeWriteByte();
    }

    // max n = 64
    bool writeNBits(uint8_t n, uint64_t value) {
        for (int i = 0; i < n && good(); i++)
            writeBit(value >> (n - i - 1));

        if (!good())
            return false;

        return true;
    }

    bool writeNBits(const BitValue &value) {
        return writeNBits(value.length, value.value);
    }

    // add utf8 encoded char to stream
    // https://en.wikipedia.org/wiki/UTF-8#Encoding
    bool writeUTF8Char(char32_t c) {
        // starting from the front send bytes
        for (int currentByte = 1; currentByte < 5; currentByte++) {
            // check if byte is used
            // always print last byte
            if (currentByte == 4 || c > ~(char32_t)0 >> (8 * currentByte)) {
                if (!writeNBits(8, c >> (32 - 8 * currentByte)))
                    return false;
            }
        }

        return true;
    }

private:
    void getReadByte() {
        currentReadBit = 0;
        currentReadByte = fstream.get();
    }

    void writeWriteByte() {
        fstream << currentWriteByte;
        currentWriteBit = 0;
        currentWriteByte = 0;
    }
};

// add utf8 encoded char to stream
// https://en.wikipedia.org/wiki/UTF-8#Encoding
std::ostream &operator<<(std::ostream &stream, char32_t c) {
    // starting from the front send bytes
    for (int currentByte = 1; currentByte < 5; currentByte++) {
        // check if byte is used
        // always print last byte
        if (currentByte == 4 || c > ~(char32_t)0 >> (8 * currentByte))
            stream << (uint8_t)(c >> (32 - 8 * currentByte));
    }

    return stream;
}

namespace huffman {

class TreeNode {
    TreeNode *parent = nullptr;
    TreeNode *left = nullptr;
    TreeNode *right = nullptr;

public:
    char32_t character = 0;
    uint64_t count = 1;

    TreeNode() {
    }

    TreeNode(char32_t character) : character(character) {
    }

    ~TreeNode() {
        delete left;
        left = nullptr;
        delete right;
        right = nullptr;
    }

    bool isLeaf() const {
        return !this->left && !this->right;
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

    // write tree in pre-order to stream
    bool write(BitFStream &stream) const {
        // if leaf write itself
        if (isLeaf()) {
            stream.writeBit(1);
            return stream.writeUTF8Char(character);
        }

        // not a leaf
        stream.writeBit(0);

        if (!stream.good())
            return false;

        return this->left->write(stream) && this->right->write(stream);
    }

    void mapCodes(std::map<char32_t, BitValue> &map) const {
        mapCodes(map, BitValue(0, 0));
    }

    bool findChar(BitFStream &stream, char32_t &target) const {
        if (isLeaf()) {
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

    void addBranches(TreeNode *left, TreeNode *right) {
        this->left = left;
        this->right = right;
        this->left->parent = this;
        this->right->parent = this;
        this->count = this->left->count + this->right->count;
    }

#ifdef PT_DEBUG

    void print() const;

#endif  // PT_DEBUG

private:
    void mapCodes(std::map<char32_t, BitValue> &map, BitValue code) const {
        if (isLeaf()) {
            map[character] = code;
            return;
        }

        code.length++;
        code.value = code.value << 1;
        this->left->mapCodes(map, code);

        code.value |= 1;
        this->right->mapCodes(map, code);
    }

#ifdef PT_DEBUG

    void print(const std::string &prefix, bool isLeft) const;

#endif  // PT_DEBUG
};

class Decompressor {
    BitFStream inStream;
    std::ofstream outStream;
    TreeNode tree;

public:
    Decompressor(const char *inFileName, const char *outFileName) : inStream(inFileName, std::ios::in), outStream(outFileName), tree() {
    }

    bool run() {
        // load character tree
        if (!tree.load(inStream))
            return false;

        PRINT_TREE(tree);

        // decompress chunks
        uint16_t chunkSize = HUFF_CHUNK_SIZE;
        while (chunkSize == HUFF_CHUNK_SIZE) {
            if (!decompressChunk(chunkSize))
                return false;
        }

        // last chunk should be smaller
        return chunkSize != HUFF_CHUNK_SIZE;
    }

private:
    bool decompressChunk(uint16_t &chunkSize) {
        chunkSize = HUFF_CHUNK_SIZE;

        if (!inStream.good())
            return false;

        // if 1st bit is 0 get chunk size
        if (!inStream.getBit()) {
            uint64_t temp;
            if (!inStream.getNBits(HUFF_CHUNK_SIZE_BITS, temp))
                return false;

            chunkSize = temp;
        }

        for (uint16_t i = 0; i < chunkSize; i++) {
            char32_t c;

            if (!tree.findChar(inStream, c) || !outStream.good())
                return false;

            outStream << c;
        }

        return true;
    }
};

class Compressor {
    BitFStream inStream;
    BitFStream outStream;
    std::map<char32_t, TreeNode *> charMap;
    TreeNode *treeTop = nullptr;
    std::map<char32_t, BitValue> charCodes;

public:
    Compressor(const char *inFileName, const char *outFileName) : inStream(inFileName, std::ios::in), outStream(outFileName, std::ios::out) {
    }

    ~Compressor() {
        // error after tree built
        if (treeTop)
            delete treeTop;
        // error while loading chars
        else {
            for (auto const &pair : charMap)
                delete pair.second;
        }
    }

    bool run() {
        // create coding tree
        if (!createTree())
            return false;

        PRINT_TREE(*treeTop);

        treeTop->mapCodes(charCodes);

        inStream.resetRead();

        // write tree to outFile
        if (!treeTop->write(outStream))
            return false;

        // write whole chunks to outFile
        for (uint64_t i = 0; i < treeTop->count / HUFF_CHUNK_SIZE; i++) {
            if (!compressChunk(HUFF_CHUNK_SIZE))
                return false;
        }

        // write last chunk to outFile
        if (!compressChunk(treeTop->count % HUFF_CHUNK_SIZE))
            return false;

        return true;
    }

private:
    bool createTree() {
        if (!inStream.good())
            return false;

        // create unconnected TreeNodes for each char in inFile
        while (true) {
            char32_t c;
            if (!inStream.getUTF8Char(c)) {
                if (inStream.eof())
                    break;
                else
                    return false;
            }

            auto search = charMap.find(c);
            if (search == charMap.end())
                charMap[c] = new TreeNode(c);
            else
                search->second->count++;
        }

        // create priority queue of nodes sorted by their count ascending
        auto compareTreeNodes = [](const TreeNode *left, const TreeNode *right) {
            return left->count > right->count;
        };
        std::priority_queue<TreeNode *, std::vector<TreeNode *>, decltype(compareTreeNodes)> nodeQueue(compareTreeNodes);

        // add all leafs to queue
        for (auto const &pair : charMap)
            nodeQueue.push(pair.second);

        // build tree
        while (nodeQueue.size() != 1) {
            auto node = new TreeNode();

            auto left = nodeQueue.top();
            nodeQueue.pop();
            auto right = nodeQueue.top();
            nodeQueue.pop();

            node->addBranches(left, right);

            nodeQueue.push(node);
        }

        treeTop = nodeQueue.top();
        charMap.clear();

        return true;
    }

    bool compressChunk(uint16_t chunkSize) {
        // chunk header
        if (chunkSize == HUFF_CHUNK_SIZE)
            outStream.writeBit(1);
        else {
            outStream.writeBit(0);
            outStream.writeNBits(HUFF_CHUNK_SIZE_BITS, chunkSize);
        }

        if (!outStream.good())
            return false;

        // chunk body
        for (int i = 0; i < chunkSize; i++) {
            char32_t c;
            if (!inStream.getUTF8Char(c) || !outStream.writeNBits(charCodes[c]))
                return false;
        }

        return true;
    }
};

}  // namespace huffman

bool decompressFile(const char *inFileName, const char *outFileName) {
    huffman::Decompressor decompressor(inFileName, outFileName);
    return decompressor.run();
}

bool compressFile(const char *inFileName, const char *outFileName) {
    huffman::Compressor compressor(inFileName, outFileName);
    return compressor.run();
}
