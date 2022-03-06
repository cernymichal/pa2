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

// utf8 console test
void UTF8CoutTest() {
    BitFStream stream("tests/utf8_test.txt", std::ios::in);
    for (unsigned i = 0; stream.good(); i++) {
        char32_t c;
        stream >> c;

        if (c != ' ' && c != '\t' && c != '\r' && c != '\n')
            std::cout << c << ' ';

        if (i % 50 == 0) std::cout << std::endl;
    }
    std::cout << std::endl;
}

#ifdef PT_DEBUG

// https://stackoverflow.com/a/51730733
void TreeNode::print(const std::string &prefix, bool isLeft) const {
    std::cout << prefix;

    std::cout << (isLeft ? "0───" : "1───");

    // print the value of the node
    if (!this->left && !this->right)
        std::cout << this->character << std::endl;
    else
        std::cout << "┐" << std::endl;

    // enter the next tree level - left and right branch
    if (this->left)
        this->left->print(prefix + (isLeft ? "│   " : "    "), true);

    if (this->right)
        this->right->print(prefix + (isLeft ? "│   " : "    "), false);
}

void TreeNode::print() const {
    print("", false);
}

#endif /* PT_DEBUG */
