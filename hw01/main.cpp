//#define PT_DEBUG

#include <fstream>

#include "huffman.cpp"
#include "testing.cpp"

int main(void) {
#ifdef PT_DEBUG

    UTF8CoutTest();
    UTF8BitFSTest();

#endif  // PT_DEBUG

    identicalFilesAsserts();
    decompressionAsserts();
    compressionAsserts();

    std::cout << "ok :)" << std::endl;
    return EXIT_SUCCESS;
}
