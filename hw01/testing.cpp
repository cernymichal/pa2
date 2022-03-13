#include <fstream>

#include "huffman.cpp"

bool identicalFiles(const char *fileName1, const char *fileName2) {
    std::ifstream file1(fileName1, std::ios::binary);
    std::ifstream file2(fileName2, std::ios::binary);

    char charFile1, charFile2;
    while (file1.good() && file2.good()) {
        charFile1 = file1.get();
        charFile2 = file2.get();

        if (charFile1 != charFile2)
            return false;
    }

    if (file1.good() != file2.good())
        return false;

    return true;
}

#ifdef PT_DEBUG

// utf8 console test
void UTF8CoutTest() {
    BitFStream stream("tests/utf8_test.txt", std::ios::in);
    for (unsigned i = 0; stream.good(); i++) {
        char32_t c;
        if (!stream.getUTF8Char(c))
            continue;

        if (c != ' ' && c != '\t' && c != '\r' && c != '\n')
            std::cout << c << ' ';

        if (i % 50 == 0) std::cout << std::endl;
    }
    std::cout << std::endl;
}

// utf8 BitFStream write test
void UTF8BitFSTest() {
    BitFStream inStream("tests/utf8_test.txt", std::ios::in);
    BitFStream outStream("test_utf8.temp", std::ios::out);

    while (true) {
        char32_t c;
        if (!inStream.getUTF8Char(c) || !outStream.writeUTF8Char(c))
            break;
    }
}

// https://stackoverflow.com/a/51730733
void huffman::TreeNode::_print(const std::string &prefix, bool isLeft) const {
    std::cout << prefix;

    std::cout << (isLeft ? "0───" : "1───");

    // print the value of the node
    if (!this->_left && !this->_right)
        std::cout << this->character << std::endl;
    else
        std::cout << "┐" << std::endl;

    // enter the next tree level - left and right branch
    if (this->_left)
        this->_left->_print(prefix + (isLeft ? "│   " : "    "), true);

    if (this->_right)
        this->_right->_print(prefix + (isLeft ? "│   " : "    "), false);
}

void huffman::TreeNode::print() const {
    _print("", false);
}

#endif  // PT_DEBUG

void identicalFilesAsserts() {
    assert(identicalFiles("main.cpp", "main.cpp"));
    assert(!identicalFiles("main.cpp", "huffman.cpp"));
}

void decompressionAsserts() {
    assert(decompressFile("tests/test0.huf", "test_orig.temp"));
    assert(identicalFiles("tests/test0.orig", "test_orig.temp"));

    assert(decompressFile("tests/test1.huf", "test_orig.temp"));
    assert(identicalFiles("tests/test1.orig", "test_orig.temp"));

    assert(decompressFile("tests/test2.huf", "test_orig.temp"));
    assert(identicalFiles("tests/test2.orig", "test_orig.temp"));

    assert(decompressFile("tests/test3.huf", "test_orig.temp"));
    assert(identicalFiles("tests/test3.orig", "test_orig.temp"));

    assert(decompressFile("tests/test4.huf", "test_orig.temp"));
    assert(identicalFiles("tests/test4.orig", "test_orig.temp"));

    assert(!decompressFile("tests/test5.huf", "test_orig.temp"));

    assert(decompressFile("tests/extra0.huf", "test_orig.temp"));
    assert(identicalFiles("tests/extra0.orig", "test_orig.temp"));

    assert(decompressFile("tests/extra1.huf", "test_orig.temp"));
    assert(identicalFiles("tests/extra1.orig", "test_orig.temp"));

    assert(decompressFile("tests/extra2.huf", "test_orig.temp"));
    assert(identicalFiles("tests/extra2.orig", "test_orig.temp"));

    assert(decompressFile("tests/extra3.huf", "test_orig.temp"));
    assert(identicalFiles("tests/extra3.orig", "test_orig.temp"));

    assert(decompressFile("tests/extra4.huf", "test_orig.temp"));
    assert(identicalFiles("tests/extra4.orig", "test_orig.temp"));

    assert(decompressFile("tests/extra5.huf", "test_orig.temp"));
    assert(identicalFiles("tests/extra5.orig", "test_orig.temp"));

    assert(decompressFile("tests/extra6.huf", "test_orig.temp"));
    assert(identicalFiles("tests/extra6.orig", "test_orig.temp"));

    assert(decompressFile("tests/extra7.huf", "test_orig.temp"));
    assert(identicalFiles("tests/extra7.orig", "test_orig.temp"));

    assert(decompressFile("tests/extra8.huf", "test_orig.temp"));
    assert(identicalFiles("tests/extra8.orig", "test_orig.temp"));

    assert(decompressFile("tests/extra9.huf", "test_orig.temp"));
    assert(identicalFiles("tests/extra9.orig", "test_orig.temp"));
}

void compressionAsserts() {
    assert(compressFile("tests/test0.orig", "test_huff.temp"));
    assert(decompressFile("test_huff.temp", "test_orig.temp"));
    assert(identicalFiles("tests/test0.orig", "test_orig.temp"));

    assert(compressFile("tests/test1.orig", "test_huff.temp"));
    assert(decompressFile("test_huff.temp", "test_orig.temp"));
    assert(identicalFiles("tests/test1.orig", "test_orig.temp"));

    assert(compressFile("tests/test2.orig", "test_huff.temp"));
    assert(decompressFile("test_huff.temp", "test_orig.temp"));
    assert(identicalFiles("tests/test2.orig", "test_orig.temp"));

    assert(compressFile("tests/test3.orig", "test_huff.temp"));
    assert(decompressFile("test_huff.temp", "test_orig.temp"));
    assert(identicalFiles("tests/test3.orig", "test_orig.temp"));

    assert(compressFile("tests/test4.orig", "test_huff.temp"));
    assert(decompressFile("test_huff.temp", "test_orig.temp"));
    assert(identicalFiles("tests/test4.orig", "test_orig.temp"));

    assert(compressFile("tests/extra0.orig", "test_huff.temp"));
    assert(decompressFile("test_huff.temp", "test_orig.temp"));
    assert(identicalFiles("tests/extra0.orig", "test_orig.temp"));

    assert(compressFile("tests/extra1.orig", "test_huff.temp"));
    assert(decompressFile("test_huff.temp", "test_orig.temp"));
    assert(identicalFiles("tests/extra1.orig", "test_orig.temp"));

    assert(compressFile("tests/extra2.orig", "test_huff.temp"));
    assert(decompressFile("test_huff.temp", "test_orig.temp"));
    assert(identicalFiles("tests/extra2.orig", "test_orig.temp"));

    assert(compressFile("tests/extra3.orig", "test_huff.temp"));
    assert(decompressFile("test_huff.temp", "test_orig.temp"));
    assert(identicalFiles("tests/extra3.orig", "test_orig.temp"));

    assert(compressFile("tests/extra4.orig", "test_huff.temp"));
    assert(decompressFile("test_huff.temp", "test_orig.temp"));
    assert(identicalFiles("tests/extra4.orig", "test_orig.temp"));

    assert(compressFile("tests/extra5.orig", "test_huff.temp"));
    assert(decompressFile("test_huff.temp", "test_orig.temp"));
    assert(identicalFiles("tests/extra5.orig", "test_orig.temp"));

    assert(compressFile("tests/extra6.orig", "test_huff.temp"));
    assert(decompressFile("test_huff.temp", "test_orig.temp"));
    assert(identicalFiles("tests/extra6.orig", "test_orig.temp"));

    assert(compressFile("tests/extra7.orig", "test_huff.temp"));
    assert(decompressFile("test_huff.temp", "test_orig.temp"));
    assert(identicalFiles("tests/extra7.orig", "test_orig.temp"));

    assert(compressFile("tests/extra8.orig", "test_huff.temp"));
    assert(decompressFile("test_huff.temp", "test_orig.temp"));
    assert(identicalFiles("tests/extra8.orig", "test_orig.temp"));

    assert(compressFile("tests/extra9.orig", "test_huff.temp"));
    assert(decompressFile("test_huff.temp", "test_orig.temp"));
    assert(identicalFiles("tests/extra9.orig", "test_orig.temp"));

    assert(compressFile("tests/utf8_test.txt", "test_huff.temp"));
    assert(decompressFile("test_huff.temp", "test_orig.temp"));
    assert(identicalFiles("tests/utf8_test.txt", "test_orig.temp"));

    assert(!compressFile("tests/advice0.orig", "test_huff.temp"));
}
