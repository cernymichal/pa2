#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

class CHash {
public:
    struct TItem {
        TItem(const std::string& key, const std::string& val, TItem* nextHash, TItem* nextOrd, TItem* prevOrd)
            : m_Key(key), m_Val(val), m_NextHash(nextHash), m_NextOrder(nextOrd), m_PrevOrder(prevOrd) {}

        std::string m_Key, m_Val;
        TItem *m_NextHash, *m_NextOrder, *m_PrevOrder;
    };

    class CIterator {
    public:
        CIterator& operator++() {
            if (item)
                item = item->m_NextOrder;

            return *this;
        }

        CIterator operator++(int) {
            auto ret = *this;

            if (item)
                item = item->m_NextOrder;

            return ret;
        }

        bool operator==(const CIterator& other) const {
            return item == other.item;
        }

        TItem* operator*() const {
            return item;
        }

    private:
        TItem* item;

        explicit CIterator(TItem* item) : item(item) {}

        friend class CHash;
    };

    CHash(unsigned int m) : m_Table(nullptr), m_Size(m), m_FirstOrder(nullptr), m_LastOrder(nullptr) {
        m_Table = new TItem*[m];
        for (unsigned i = 0; i < m; i++)
            m_Table[i] = nullptr;
    }

    CHash(const CHash& other) : m_Table(nullptr), m_Size(0), m_FirstOrder(nullptr), m_LastOrder(nullptr) {
        *this = other;
    }

    ~CHash() {
        delete[] m_Table;
        for (auto item = m_FirstOrder; item;) {
            auto nextItem = item->m_NextOrder;
            delete item;
            item = nextItem;
        }
    }

    CHash& operator=(const CHash& other) {
        if (&other == this)
            return *this;

        delete[] m_Table;
        for (auto item = m_FirstOrder; item;) {
            auto nextItem = item->m_NextOrder;
            delete item;
            item = nextItem;
        }

        m_FirstOrder = nullptr;
        m_LastOrder = nullptr;
        m_Size = other.m_Size;
        m_Table = new TItem*[m_Size];
        for (unsigned i = 0; i < m_Size; i++)
            m_Table[i] = nullptr;

        other.ForEach([this](TItem* it) {
            Ins(it->m_Key, it->m_Val);
        });

        return *this;
    }

    std::string& operator[](const std::string& key) {
        TItem* item = m_Table[hashFn(key)];
        while (item != nullptr && item->m_Key != key)
            item = item->m_NextHash;

        if (item == nullptr) {
            Ins(key, "");
            return m_LastOrder->m_Val;
        }

        return item->m_Val;
    }

    bool Ins(const std::string& key, const std::string& val) {
        if (IsSet(key))
            return false;

        auto hash = hashFn(key);
        auto item = new TItem(key, val, m_Table[hash], nullptr, m_LastOrder);

        m_Table[hash] = item;

        if (m_LastOrder)
            m_LastOrder->m_NextOrder = item;
        m_LastOrder = item;

        if (m_FirstOrder == nullptr)
            m_FirstOrder = item;

        return true;
    }

    bool Del(const std::string& key) {
        TItem** tmp = &m_Table[hashFn(key)];
        while (*tmp != nullptr && (*tmp)->m_Key != key)
            tmp = &(*tmp)->m_NextHash;

        if (*tmp == nullptr)
            return false;

        auto item = *tmp;
        *tmp = (*tmp)->m_NextHash;

        if (item->m_PrevOrder)
            item->m_PrevOrder->m_NextOrder = item->m_NextOrder;
        if (item->m_NextOrder)
            item->m_NextOrder->m_PrevOrder = item->m_PrevOrder;

        if (m_FirstOrder == item)
            m_FirstOrder = item->m_NextOrder;
        if (m_LastOrder == item)
            m_LastOrder = item->m_PrevOrder;

        delete item;

        return true;
    }

    bool Rehash(unsigned int m) {
        if (m == 0)
            return false;

        m_Size = m;
        delete[] m_Table;
        m_Table = new TItem*[m];
        for (unsigned i = 0; i < m; i++)
            m_Table[i] = nullptr;

        ForEach([this](TItem* item) {
            auto hash = hashFn(item->m_Key);
            item->m_NextHash = m_Table[hash];
            m_Table[hash] = item;
        });

        return true;
    }

    bool IsSet(const std::string& key) const {
        TItem* tmp = m_Table[hashFn(key)];
        while (tmp != nullptr && tmp->m_Key != key)
            tmp = tmp->m_NextHash;
        if (tmp == nullptr)
            return false;
        return true;
    }

    template <typename func>
    void ForEach(func f) const {
        TItem* tmp = m_FirstOrder;
        while (tmp != nullptr) {
            f(tmp);
            tmp = tmp->m_NextOrder;
        }
    }

    CIterator begin() const {
        return CIterator(m_FirstOrder);
    }

    CIterator end() const {
        return CIterator(nullptr);
    }

private:
    TItem** m_Table;
    unsigned int m_Size;
    TItem *m_FirstOrder, *m_LastOrder;

    unsigned int hashFn(const std::string& str) const {
        std::hash<std::string> hash_fn;
        return hash_fn(str) % m_Size;
    }

    friend int main(int argc, char** argv);
};

int main(int argc, char** argv) {
    CHash hashtable(100);
    hashtable.Ins("h1", "car");
    hashtable.Ins("h1", "phone");
    hashtable.Ins("h2", "field");
    hashtable.Ins("h3", "house");
    hashtable.Ins("h4", "tree");

    CHash hashtable2 = hashtable;

    hashtable.Del("h2");

    auto printLambda = [](CHash::TItem* it) {
        std::cout << it->m_Key << " - " << it->m_Val << std::endl;
    };

    std::cout << "\nhashtable >" << std::endl;
    hashtable.ForEach(printLambda);

    std::cout << "\nhashtable2 >" << std::endl;
    hashtable2.ForEach(printLambda);

    hashtable2 = hashtable;

    std::cout << "\nhashtable2 after reassign >" << std::endl;
    hashtable2.ForEach(printLambda);

    hashtable2.Rehash(1);

    hashtable2["asdf"] = "ahoj";

    std::cout << "\nhashtable2 after rehash >" << std::endl;
    hashtable2.ForEach(printLambda);

    return 0;
}
