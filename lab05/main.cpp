#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>

#define LEVEL 8

template <typename T>
struct Queue {
    ~Queue() {
        while (!empty()) pop();
    }

    // lvl 1
    bool empty() const { return _size == 0; }
    size_t size() const { return _size; }

    // Lvl 5 vyžaduje const a ne-const verze. Const verze by stále
    // mohla vracet hodnotou, ale reference je typicky efektivnější
    // a navíc nevyžaduje existenci copy konstruktoru.
    T& front() { return _front->value; }
    const T& front() const { return _front->value; }
    T& back() { return _back->value; }
    const T& back() const { return _back->value; }

    void push(const T& t) {
        Node* n = new Node(t);
        (_size++ ? _back->next : _front) = n;
        _back = n;
    }

    T pop() {
        _size--;
        // Použijeme unique_ptr, který zajistí uvolnění objektu. Navíc ho
        // uvolní až po returnu.
        std::unique_ptr<Node> n(_front);
        _front = _front->next;
        return n->value;
    }

    // lvl 2
    // Musíme vracet referenci na sebe, aby bylo možné volání řetězit.
    Queue& operator>>(T& t) {
        t = pop();
        return *this;
    }

    Queue& operator<<(const T& t) {
        push(t);
        return *this;
    }

    // lvl 3
    // Musíme vracet referenci, aby bylo možné položky modifikovat.
    T& operator[](size_t i) {
        Node* n = _front;
        while (i--) n = n->next;
        return n->value;
    }

    // lvl 5 vyžaduje i const verzi, práci zdelegujeme na ne-const verzi
    const T& operator[](size_t i) const {
        return const_cast<Queue*>(this)->operator[](i);
    }

    // lvl 4
    // Při práci se streamy vždy přetěžujeme ostream nebo istream.
    friend std::ostream& operator<<(std::ostream& out, const Queue& q) {
        out << "< ";
        for (Node* n = q._front; n; n = n->next) out << n->value << " ";
        return out << ">";
    }

    // lvl 6
    bool operator==(const Queue& q) const {
        if (_size != q._size) return false;
        Node* a = _front;
        Node* b = q._front;
        while (a) {
            if (a->value != b->value) return false;
            a = a->next;
            b = b->next;
        }
        return true;
    }

    bool operator!=(const Queue& q) const { return !(*this == q); }

    Queue& operator<<(const Queue& q) {
        size_t s = q._size;
        Node* n = q._front;

        // Kdybychom iterovali dokud n != nullptr, tak se zacyklíme
        // při volání q << q.
        while (s--) {
            push(n->value);
            n = n->next;
        }

        return *this;
    }

    // lvl 7
    operator bool() const { return _size; }

    // lvl 8 ze zdarma

private:
    struct Node {
        T value;
        Node* next = nullptr;
        Node(const T& t) : value(t) {}
    };

    size_t _size = 0;
    Node* _front;
    Node* _back;
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
        }                                                                                  \
        else                                                                               \
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
        for (int i = 0; i < 10; i++) q.push(i);

        TEST(q.size(), ==, 10U);
        TEST(q.front(), ==, 0);
        TEST(q.back(), ==, 9);

        int i = 0;
        while (!q.empty()) TEST(q.pop(), ==, i++);
        TEST(i, ==, 10);

        q.push(777);
        TEST(q.front(), ==, 777);
    }
#endif

#if LEVEL >= 2  // Operátory << >> pro přidávání a vybírání z fronty
    {
        Queue<int> q;
        for (int i = 0; i < 10; i++) q << i;

        int a, b;
        q >> a >> b;
        TEST(a, ==, 0);
        TEST(b, ==, 1);
        TEST(q.size(), ==, 8U);

#if LEVEL >= 3  // Operátor [] s možností modifikace
        for (int i = 0; i < 8; i++) {
            TEST(q[i], ==, i + 2);
            q[i] -= 1;
            TEST(q[i], ==, i + 1);
        }
#endif

#if LEVEL >= 4  // Operátor << pro výpis
        const auto& cq = q;
        std::ostringstream out;
        out << cq;
        TEST(out.str(), ==, "< 1 2 3 4 5 6 7 8 >");
#endif
    }
#endif

#if LEVEL >= 5  // Správné použití const
    {
        Queue<int> q;
        const auto& cq = q;
        for (int i = 0; i < 10; i++) q.push(i);

        TEST(cq.size(), ==, 10U);
        TEST(cq.front(), ==, 0);
        TEST(cq.back(), ==, 9);

        q.front() += 2;
        TEST(cq[0], ==, 2);
        q.push(q.pop());
        q.back() += 8;

        // cq.front() += 1; // NESMÍ fungovat
        // cq.back() += 1; // NESMÍ fungovat
        // cq[0] += 1; // NESMÍ fungovat

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

        q.push(text);
    }
#endif

#if LEVEL >= 6  // << na Queue a ==
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

#if LEVEL >= 7  // Přetypování na bool
    {
        Queue<int> q;
        const auto& cq = q;
        TEST(!!cq, ==, false);
        for (int i = 0; i < 10; i++) q << i;
        TEST(!!cq, ==, true);
    }
#endif

#if LEVEL >= 8  // Objekty bez defaultního konstruktoru
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
