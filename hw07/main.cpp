#include <iostream>
#include <string>

#include "index.cpp"

class CStrComparator {
public:
    CStrComparator(bool caseSensitive) : m_CaseSensitive(caseSensitive) {
    }
    bool operator()(const std::string &a, const std::string &b) const {
        return m_CaseSensitive ? strcasecmp(a.c_str(), b.c_str()) < 0 : a < b;
    }
    bool m_CaseSensitive;
};

bool upperCaseCompare(const char &a, const char &b) {
    return toupper(a) < toupper(b);
}

int main(void) {
    CIndex<std::string> t0("abcabcabc");
    std::set<size_t> r0 = t0.search("a");
    assert(r0 == (std::set<size_t>{0, 3, 6}));
    std::set<size_t> r1 = t0.search("abc");
    assert(r1 == (std::set<size_t>{0, 3, 6}));
    std::set<size_t> r2 = t0.search("abcabc");
    assert(r2 == (std::set<size_t>{0, 3}));

    CIndex<std::string> t1("abcababc");
    std::set<size_t> r3 = t1.search("a");
    assert(r3 == (std::set<size_t>{0, 3, 5}));
    std::set<size_t> r4 = t1.search("abc");
    assert(r4 == (std::set<size_t>{0, 5}));
    std::set<size_t> r5 = t1.search("abcabc");
    assert(r5 == (std::set<size_t>{}));

    CIndex<std::string> t2("kokokokoskokosokos");
    std::set<size_t> r6 = t2.search("kos");
    assert(r6 == (std::set<size_t>{6, 11, 15}));
    std::set<size_t> r7 = t2.search("kokos");
    assert(r7 == (std::set<size_t>{4, 9}));

    CIndex<std::string> t3("aaaaaaau aaauaaaau");
    std::set<size_t> r8 = t3.search("aa");
    assert(r8 == (std::set<size_t>{0, 1, 2, 3, 4, 5, 9, 10, 13, 14, 15}));
    std::set<size_t> r9 = t3.search("aaa");
    assert(r9 == (std::set<size_t>{0, 1, 2, 3, 4, 9, 13, 14}));
    std::set<size_t> r10 = t3.search("aaaa");
    assert(r10 == (std::set<size_t>{0, 1, 2, 3, 13}));

    CIndex<std::string> t4("automatIc authentication automotive auTOmation raut");
    std::set<size_t> r11 = t4.search("auto");
    assert(r11 == (std::set<size_t>{0, 25}));
    std::set<size_t> r12 = t4.search("aut");
    assert(r12 == (std::set<size_t>{0, 10, 25, 48}));
    std::set<size_t> r13 = t4.search("tic");
    assert(r13 == (std::set<size_t>{16}));
    std::set<size_t> r14 = t4.search("trunk");
    assert(r14 == (std::set<size_t>{}));
    std::set<size_t> r15 = t4.search("a");
    assert(r15 == (std::set<size_t>{0, 5, 10, 19, 25, 36, 41, 48}));
    std::set<size_t> r16 = t4.search("");
    assert(r16 == (std::set<size_t>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50}));

    CIndex<std::string, bool (*)(const char &, const char &)> t5("automatIc authentication automotive auTOmation raut", upperCaseCompare);
    std::set<size_t> r17 = t5.search("auto");
    assert(r17 == (std::set<size_t>{0, 25, 36}));
    std::set<size_t> r18 = t5.search("aut");
    assert(r18 == (std::set<size_t>{0, 10, 25, 36, 48}));
    std::set<size_t> r19 = t5.search("tic");
    assert(r19 == (std::set<size_t>{6, 16}));
    std::set<size_t> r20 = t5.search("trunk");
    assert(r20 == (std::set<size_t>{}));
    std::set<size_t> r21 = t5.search("a");
    assert(r21 == (std::set<size_t>{0, 5, 10, 19, 25, 36, 41, 48}));
    std::set<size_t> r22 = t5.search("");
    assert(r22 == (std::set<size_t>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50}));

    CIndex<std::list<std::string>, CStrComparator> t6(std::list<std::string>{"Hello", "world", "test", "this", "foo", "TEsT", "this", "done"}, CStrComparator(false));
    std::set<size_t> r23 = t6.search(std::list<std::string>{"test", "this", "foo"});
    assert(r23 == (std::set<size_t>{2}));
    std::set<size_t> r24 = t6.search(std::list<std::string>{"test", "this"});
    assert(r24 == (std::set<size_t>{2}));

    CIndex<std::list<std::string>, CStrComparator> t7(std::list<std::string>{"Hello", "world", "test", "this", "foo", "TEsT", "this", "done"}, CStrComparator(true));
    std::set<size_t> r25 = t7.search(std::list<std::string>{"test", "this", "foo"});
    assert(r25 == (std::set<size_t>{2}));
    std::set<size_t> r26 = t7.search(std::list<std::string>{"test", "this"});
    assert(r26 == (std::set<size_t>{2, 5}));

    std::cout << "ok :)" << std::endl;
    return EXIT_SUCCESS;
}
