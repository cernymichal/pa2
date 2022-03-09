//#define PT_DEBUG

#include <fstream>

#include "huffman.cpp"
#include "testing.cpp"

int main(void) {
    // UTF8CoutTest();

    // identitical files asserts

    assert(identicalFiles("main.cpp", "main.cpp"));
    assert(!identicalFiles("main.cpp", "huffman.cpp"));

    // decompression asserts

    assert(decompressFile("tests/test0.huf", "tempfile"));
    assert(identicalFiles("tests/test0.orig", "tempfile"));

    assert(decompressFile("tests/test1.huf", "tempfile"));
    assert(identicalFiles("tests/test1.orig", "tempfile"));

    assert(decompressFile("tests/test2.huf", "tempfile"));
    assert(identicalFiles("tests/test2.orig", "tempfile"));

    assert(decompressFile("tests/test3.huf", "tempfile"));
    assert(identicalFiles("tests/test3.orig", "tempfile"));

    assert(decompressFile("tests/test4.huf", "tempfile"));
    assert(identicalFiles("tests/test4.orig", "tempfile"));

    assert(!decompressFile("tests/test5.huf", "tempfile"));

    assert(decompressFile("tests/extra0.huf", "tempfile"));
    assert(identicalFiles("tests/extra0.orig", "tempfile"));

    assert(decompressFile("tests/extra1.huf", "tempfile"));
    assert(identicalFiles("tests/extra1.orig", "tempfile"));

    assert(decompressFile("tests/extra2.huf", "tempfile"));
    assert(identicalFiles("tests/extra2.orig", "tempfile"));

    assert(decompressFile("tests/extra3.huf", "tempfile"));
    assert(identicalFiles("tests/extra3.orig", "tempfile"));

    assert(decompressFile("tests/extra4.huf", "tempfile"));
    assert(identicalFiles("tests/extra4.orig", "tempfile"));

    assert(decompressFile("tests/extra5.huf", "tempfile"));
    assert(identicalFiles("tests/extra5.orig", "tempfile"));

    assert(decompressFile("tests/extra6.huf", "tempfile"));
    assert(identicalFiles("tests/extra6.orig", "tempfile"));

    assert(decompressFile("tests/extra7.huf", "tempfile"));
    assert(identicalFiles("tests/extra7.orig", "tempfile"));

    assert(decompressFile("tests/extra8.huf", "tempfile"));
    assert(identicalFiles("tests/extra8.orig", "tempfile"));

    assert(decompressFile("tests/extra9.huf", "tempfile"));
    assert(identicalFiles("tests/extra9.orig", "tempfile"));

    // compression asserts

    assert(compressFile("tests/test0.orig", "tempfile"));
    assert(identicalFiles("tests/test0.huf", "tempfile"));

    assert(compressFile("tests/test1.orig", "tempfile"));
    assert(identicalFiles("tests/test1.huf", "tempfile"));

    assert(compressFile("tests/test2.orig", "tempfile"));
    assert(identicalFiles("tests/test2.huf", "tempfile"));

    assert(compressFile("tests/test3.orig", "tempfile"));
    assert(identicalFiles("tests/test3.huf", "tempfile"));

    assert(compressFile("tests/test4.orig", "tempfile"));
    assert(identicalFiles("tests/test4.huf", "tempfile"));

    assert(compressFile("tests/extra0.orig", "tempfile"));
    assert(identicalFiles("tests/extra0.huf", "tempfile"));

    assert(compressFile("tests/extra1.orig", "tempfile"));
    assert(identicalFiles("tests/extra1.huf", "tempfile"));

    assert(compressFile("tests/extra2.orig", "tempfile"));
    assert(identicalFiles("tests/extra2.huf", "tempfile"));

    assert(compressFile("tests/extra3.orig", "tempfile"));
    assert(identicalFiles("tests/extra3.huf", "tempfile"));

    assert(compressFile("tests/extra4.orig", "tempfile"));
    assert(identicalFiles("tests/extra4.huf", "tempfile"));

    assert(compressFile("tests/extra5.orig", "tempfile"));
    assert(identicalFiles("tests/extra5.huf", "tempfile"));

    assert(compressFile("tests/extra6.orig", "tempfile"));
    assert(identicalFiles("tests/extra6.huf", "tempfile"));

    assert(compressFile("tests/extra7.orig", "tempfile"));
    assert(identicalFiles("tests/extra7.huf", "tempfile"));

    assert(compressFile("tests/extra8.orig", "tempfile"));
    assert(identicalFiles("tests/extra8.huf", "tempfile"));

    assert(compressFile("tests/extra9.orig", "tempfile"));
    assert(identicalFiles("tests/extra9.huf", "tempfile"));

    std::cout << "ok :)" << std::endl;
    return 0;
}
