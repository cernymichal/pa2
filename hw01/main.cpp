//#define PT_DEBUG

#include <fstream>

#include "huffman.cpp"
#include "testing.cpp"

int main(void) {
    // UTF8CoutTest();
    // UTF8BitFSTest();

    // identitical files asserts

    assert(identicalFiles("main.cpp", "main.cpp"));
    assert(!identicalFiles("main.cpp", "huffman.cpp"));

    // decompression asserts

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

    // compression asserts

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

    std::cout << "ok :)" << std::endl;
    return EXIT_SUCCESS;
}
