#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>

#define LEVEL 3

template <typename T>
class Queue {
    struct Node {
        T value;
        Node* next = nullptr;
        Node* prev = nullptr;

        Node(const T& value, Node* prev) : value(value), prev(prev) {}

        ~Node() {
            delete prev;
        }
    };

    Node* frontNode = nullptr;
    Node* backNode = nullptr;
    unsigned nodeCount = 0;

public:
    ~Queue() {
        delete backNode;
    }

    unsigned size() const {
        return nodeCount;
    }

    void push(const T& value) {
        auto node = new Node(value, backNode);

        if (backNode)
            backNode->next = node;

        backNode = node;

        if (!frontNode)
            frontNode = backNode;

        nodeCount++;
    };

    T pop() {
        auto node = frontNode;
        frontNode = node->next;

        if (frontNode)
            frontNode->prev = nullptr;
        else
            backNode = nullptr;

        nodeCount--;

        auto value = node->value;
        delete node;

        return value;
    }

    T& front() const {
        return frontNode->value;
    }

    T& back() const {
        return backNode->value;
    }

    bool empty() const {
        return nodeCount == 0;
    }

    Queue& operator>>(T& target) {
        target = pop();
        return *this;
    }

    Queue& operator<<(const T& value) {
        push(value);
        return *this;
    }

    T& operator[](int index) const {
        auto node = frontNode;
        for (; node && index > 0; node = node->next, index--)
            ;
        return node->value;
    }

    Queue& operator<<(const Queue<T>& other) {
        for (auto node = other.frontNode; node; node = node->next)
            push(node->value);

        return *this;
    }

    bool operator==(const Queue<T>& other) {
        return nodeCount == other.nodeCount;
    }

    bool operator!=(const Queue<T>& other) {
        return nodeCount != other.nodeCount;
    }

    friend std::ostream& operator<<(std::ostream& out, const Queue<T>& queue) {
        out << "< ";

        for (auto node = queue.frontNode; node; node = node->next) {
            out << node->value << " ";
        }

        return out << ">";
    }
};

// Černá magie, ignorujte.

#define TEST(l, op, r)                                                                     \
    do {                                                                                   \
        auto&& _l = (l);                                                                   \
        auto&& _r = (r);                                                                   \
        if (!(_l op _r)) {                                                                 \
            fail++;                                                                        \
            std::cout << "Line " << __LINE__ << " expression "                             \
                      << #l << " " << #op << " " << #r                                     \
                      << " failed, got: lhs = " << _l << " and rhs = " << _r << std::endl; \
        } else                                                                             \
            ok++;                                                                          \
    } while (0)

struct NoDef {
    int x;
    explicit NoDef(int x) : x(x) {}
};

std::ostream& operator<<(std::ostream& out, const NoDef& t) { return out << t.x; }

struct NoCopy {
    int x;
    explicit NoCopy(int x = 999) : x(x) {}
    NoCopy(const NoCopy&) = delete;
    NoCopy& operator=(const NoCopy&) = delete;
    NoCopy(NoCopy&&) = default;             // move konstruktor
    NoCopy& operator=(NoCopy&&) = default;  // move operátor přiřazení
};

std::ostream& operator<<(std::ostream& out, const NoCopy& t) { return out << t.x; }

// Konec černé magie

int main() {
    int ok = 0, fail = 0;

#if LEVEL >= 1  // Základní testy
    {
        Queue<int> q;
        const auto& cq = q;
        for (int i = 0; i < 10; i++) q.push(i);

        TEST(cq.size(), ==, 10U);
        TEST(cq.front(), ==, 0);
        TEST(cq.back(), ==, 9);

        q.front() += 2;
        TEST(cq.front(), ==, 2);
        q.push(q.pop());
        q.back() += 8;

        int i = 0;
        while (!q.empty()) TEST(q.pop(), ==, ++i);
        TEST(i, ==, 10);
    }

    {
        const char text[] = "lorem ipsum";
        Queue<std::string> q;
        const auto& cq = q;
        for (int i = 0; i < (int)sizeof(text); i++) q.push(text + i);

        TEST(cq.size(), ==, sizeof(text));
        TEST(cq.front(), ==, text);
        TEST(cq.back(), ==, "");

        int i = 0;
        while (!q.empty()) TEST(q.pop(), ==, text + i++);
    }
#endif

#if LEVEL >= 2  // Operátory [] << >>
    {
        Queue<int> q;
        const auto& cq = q;
        for (int i = 0; i < 10; i++) q << i;

        int a, b;
        q >> a >> b;
        TEST(a, ==, 0);
        TEST(b, ==, 1);

        TEST(q.size(), ==, 8U);
        for (int i = 0; i < 8; i++) {
            TEST(q[i], ==, i + 2);
            q[i] -= 1;
            TEST(cq[i], ==, i + 1);
        }

        std::ostringstream out;
        out << q;
        TEST(out.str(), ==, "< 1 2 3 4 5 6 7 8 >");
    }
#endif

#if LEVEL >= 3  // << na Queue a ==
    {
        Queue<int> q, qq;
        for (int i = 0; i < 10; i++) q << i;

        qq << q << q;
        TEST(qq.size(), ==, 20U);

        for (int i = 0; i < 20; i++) TEST(qq[i], ==, i % 10);

        q << q;
        TEST(qq.size(), ==, 20U);
        TEST(qq, ==, q);

        int i;
        q >> i;
        q << i;
        TEST(qq, !=, q);

        qq >> i;
        qq << i;
        TEST(qq, ==, q);
    }
#endif

#if LEVEL >= 4  // Přetypování na bool
    {
        Queue<int> q;
        const auto& cq = q;
        TEST(!!cq, ==, false);
        for (int i = 0; i < 10; i++) q << i;
        TEST(!!cq, ==, true);
    }
#endif

#if LEVEL >= 5  // Objekty bez defaultního konstruktoru
    {
        Queue<NoDef> q;
        const auto& cq = q;
        for (int i = 0; i < 10; i++) q << NoDef(i);

        NoDef a(9), b(9);
        q >> a >> b;
        TEST(q.size(), ==, 8U);

        std::ostringstream out;
        out << cq;
        TEST(out.str(), ==, "< 2 3 4 5 6 7 8 9 >");
    }
#endif

#if LEVEL >= 99  // move a copy Queue
    {
        Queue<int> q;
        for (int i = 0; i < 10; i++) q << i;

        auto qq = q;
        TEST(q, ==, qq);
        q.pop();
        TEST(q, !=, qq);
        for (int i = 0; i < 10; i++) TEST(qq[i], ==, i);
        for (int i = 0; i < 9; i++) TEST(q[i], ==, i + 1);

        q.back() += 1;
        TEST(q.back(), ==, 10);
        TEST(qq.back(), ==, 9);

        q = std::move(qq);
        std::ostringstream out;
        out << q;
        TEST(out.str(), ==, "< 0 1 2 3 4 5 6 7 8 9 >");

        std::cout << q << std::endl;
        std::cout << qq << std::endl;
    }
#endif

#if LEVEL >= 999  // Objekty bez copy konstruktoru
    {
        Queue<NoCopy> q;
        for (int i = 0; i < 10; i++) q << NoCopy(i);

        q.pop();
        auto qq = std::move(q);

        std::ostringstream out;
        out << qq;
        TEST(out.str(), ==, "< 1 2 3 4 5 6 7 8 9 >");

        std::cout << q << std::endl;
        std::cout << qq << std::endl;
    }
#endif

#if LEVEL >= 9999  // uživatelsky definované přetypování, vrací front()
    {
        Queue<int> q;
        const auto& cq = q;
        for (int i = 0; i < 10; i++) q << i;

        TEST(0, ==, static_cast<int>(cq));
        q.pop();
        TEST(1, ==, static_cast<int>(q));

#if LEVEL >= 10'000  // navíc umožňuje modifikaci
        static_cast<int&>(q) = 7;
        std::ostringstream out;
        out << cq;
        TEST(out.str(), ==, "< 7 2 3 4 5 6 7 8 9 >");
#endif
    }
#endif

    if (!fail)
        std::cout << "Passed all " << ok << " tests" << std::endl;
    else
        std::cout << "Failed " << fail << " of " << (ok + fail) << " tests" << std::endl;
}
