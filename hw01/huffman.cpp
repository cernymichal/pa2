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

#define HUFF_CHUNK_SIZE 12
#define HUFF_CHUNK_MAX (1 << HUFF_CHUNK_SIZE)

struct BitValue {
    uint64_t value;
    uint8_t length;  // max 64

    BitValue(uint64_t value = 0, uint8_t length = 0) : value(value), length(length) {
    }
};

// fstream wrapper for single bit operations
class BitFStream {
    std::fstream _fstream;
    uint8_t _currentReadBit = 8;
    uint8_t _currentReadByte = 0; // read buffer
    uint8_t _currentWriteBit = 0;
    uint8_t _currentWriteByte = 0; // write buffer

public:
    bool getUTF8CharZeroBytes = false;

    BitFStream(const char *fileName, std::ios_base::openmode mode) : _fstream(fileName, mode | std::ios::binary) {
    }

    ~BitFStream() {
        if (_currentWriteBit != 0)
            _writeWriteByte();
    }

    bool good() const {
        return _fstream.good();
    }

    bool eof() const {
        return _fstream.eof();
    }

    void resetRead() {
        _currentReadBit = 8;
        _currentReadByte = 0;
        getUTF8CharZeroBytes = false;
        _fstream.clear();
        _fstream.seekg(0);
    }

    uint8_t getBit() {
        if (_currentReadBit % 8 == 0)
            _getReadByte();

        return (_currentReadByte >> (7 - _currentReadBit++)) & 1;
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
            if (!getByte(byte)) {
                if (byteCount == 0)
                    getUTF8CharZeroBytes = true;
                return false;
            }

            // check if consecutive bytes look like 10xxxxxx
            if (byteCount != 0)
                if ((byte & 0xc0) != 0x80)
                    return false;

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
        _currentWriteByte |= (value & 1) << (7 - _currentWriteBit++);

        if (_currentWriteBit == 8)
            _writeWriteByte();
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
    void _getReadByte() {
        _currentReadBit = 0;
        _currentReadByte = _fstream.get();
    }

    void _writeWriteByte() {
        _fstream << _currentWriteByte;
        _currentWriteBit = 0;
        _currentWriteByte = 0;
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
    TreeNode *_parent = nullptr;
    TreeNode *_left = nullptr;
    TreeNode *_right = nullptr;

public:
    char32_t character = 0;
    uint64_t count = 1;

    TreeNode() {
    }

    TreeNode(char32_t character) : character(character) {
    }

    ~TreeNode() {
        delete _left;
        _left = nullptr;
        delete _right;
        _right = nullptr;
    }

    bool isLeaf() const {
        return !this->_left && !this->_right;
    }

    // load tree from bit stream
    bool load(BitFStream &stream) {
        if (!stream.good())
            return false;

        // check if is leaf
        if (stream.getBit())
            return stream.getUTF8Char(character);

        // is node -> load branches
        _left = new TreeNode();
        _right = new TreeNode();

        return _left->load(stream) && _right->load(stream);
    }

    // write tree in pre-order to stream
    bool write(BitFStream &stream) const {
        // if leaf write itself
        if (isLeaf()) {
            stream.writeBit(1);
            return stream.good() && stream.writeUTF8Char(character);
        }

        // not a leaf
        stream.writeBit(0);

        if (!stream.good())
            return false;

        return this->_left->write(stream) && this->_right->write(stream);
    }

    // fill char to code map
    void mapCodes(std::map<char32_t, BitValue> &map) const {
        _mapCodes(map, BitValue(0, 0));
    }

    // read from stream until corresponding char is found in tree
    bool findChar(BitFStream &stream, char32_t &target) const {
        if (isLeaf()) {
            target = character;
            return true;
        }

        if (!stream.good())
            return false;

        if (!stream.getBit())
            return this->_left && this->_left->findChar(stream, target);
        else
            return this->_right && this->_right->findChar(stream, target);
    }

    void addBranches(TreeNode *left, TreeNode *right) {
        this->_left = left;
        this->_right = right;
        this->_left->_parent = this;
        this->_right->_parent = this;
        this->count = this->_left->count + this->_right->count;
    }

#ifdef PT_DEBUG

    void print() const;

#endif  // PT_DEBUG

private:
    // fill char to code map
    void _mapCodes(std::map<char32_t, BitValue> &map, BitValue code) const {
        if (isLeaf()) {
            map[character] = code;
            return;
        }

        code.length++;
        code.value = code.value << 1;
        this->_left->_mapCodes(map, code);

        code.value |= 1;
        this->_right->_mapCodes(map, code);
    }

#ifdef PT_DEBUG

    void _print(const std::string &prefix, bool isLeft) const;

#endif  // PT_DEBUG
};

class Decompressor {
    BitFStream _inStream;
    std::ofstream _outStream;
    TreeNode _tree;

public:
    Decompressor(const char *inFileName, const char *outFileName)
        : _inStream(inFileName, std::ios::in), _outStream(outFileName), _tree() {
    }

    bool run() {
        // load character tree
        if (!_tree.load(_inStream))
            return false;

        PRINT_TREE(_tree);

        // decompress chunks
        uint16_t chunkSize = HUFF_CHUNK_MAX;
        while (chunkSize == HUFF_CHUNK_MAX) {
            if (!_decompressChunk(chunkSize))
                return false;
        }

        // last chunk should be smaller
        return chunkSize != HUFF_CHUNK_MAX;
    }

private:
    bool _decompressChunk(uint16_t &chunkSize) {
        chunkSize = HUFF_CHUNK_MAX;

        if (!_inStream.good())
            return false;

        // if 1st bit is 0 get chunk size
        if (!_inStream.getBit()) {
            uint64_t temp;
            if (!_inStream.getNBits(HUFF_CHUNK_SIZE, temp))
                return false;

            chunkSize = temp;
        }

        for (uint16_t i = 0; i < chunkSize; i++) {
            char32_t c;

            if (!_tree.findChar(_inStream, c) || !_outStream.good())
                return false;

            _outStream << c;
        }

        return true;
    }
};

class Compressor {
    BitFStream _inStream;
    BitFStream _outStream;
    std::map<char32_t, TreeNode *> _charMap;
    TreeNode *_treeTop = nullptr;
    std::map<char32_t, BitValue> _charCodes;

public:
    Compressor(const char *inFileName, const char *outFileName)
        : _inStream(inFileName, std::ios::in), _outStream(outFileName, std::ios::out) {
    }

    ~Compressor() {
        // error after tree built
        if (_treeTop)
            delete _treeTop;
        // error while loading chars
        else {
            for (auto const &pair : _charMap)
                delete pair.second;
        }
    }

    bool run() {
        // create coding tree
        if (!_createTree())
            return false;

        PRINT_TREE(*_treeTop);

        _treeTop->mapCodes(_charCodes);

        _inStream.resetRead();

        // write tree to outFile
        if (!_treeTop->write(_outStream))
            return false;

        // write whole chunks to outFile
        for (uint64_t i = 0; i < _treeTop->count / HUFF_CHUNK_MAX; i++) {
            if (!_compressChunk(HUFF_CHUNK_MAX))
                return false;
        }

        // write last chunk to outFile
        if (!_compressChunk(_treeTop->count % HUFF_CHUNK_MAX))
            return false;

        return true;
    }

private:
    bool _createTree() {
        // create unconnected TreeNodes for each char in inFile
        while (true) {
            char32_t c;
            if (!_inStream.getUTF8Char(c)) {
                if (_inStream.eof() && _inStream.getUTF8CharZeroBytes)
                    break;
                else
                    return false;
            }

            auto search = _charMap.find(c);
            if (search == _charMap.end())
                _charMap[c] = new TreeNode(c);
            else
                search->second->count++;
        }

        // create priority queue of nodes sorted by their count ascending
        auto compareTreeNodes = [](const TreeNode *left, const TreeNode *right) {
            return left->count > right->count;
        };
        std::priority_queue<TreeNode *, std::vector<TreeNode *>, decltype(compareTreeNodes)> nodeQueue(compareTreeNodes);

        // add all leafs to queue
        for (auto const &pair : _charMap)
            nodeQueue.push(pair.second);

        // huffman coding is not defined for empty files or files with one character
        if (nodeQueue.size() <= 1)
            return false;

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

        _treeTop = nodeQueue.top();

        // char map to TreeNodes won't be needed anymore
        _charMap.clear();

        return true;
    }

    bool _compressChunk(uint16_t chunkSize) {
        // chunk header
        if (chunkSize == HUFF_CHUNK_MAX)
            _outStream.writeBit(1);
        else {
            _outStream.writeBit(0);
            _outStream.writeNBits(HUFF_CHUNK_SIZE, chunkSize);
        }

        if (!_outStream.good())
            return false;

        // chunk body
        for (int i = 0; i < chunkSize; i++) {
            char32_t c;
            if (!_inStream.getUTF8Char(c) || !_outStream.writeNBits(_charCodes[c]))
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
