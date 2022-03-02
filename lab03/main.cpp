#include <iostream>
#include <cassert>

template <typename T>
class Array {
    size_t m_Size = 0;
    size_t m_Capacity = 0;
    T *m_Data = nullptr;

    void grow(size_t newCapacity) {
        auto newData = new T[newCapacity];

        for (size_t i = 0; i < m_Size; i++)
            newData[i] = m_Data[i];

        delete[] m_Data;
        m_Data = newData;

        m_Capacity = newCapacity;
    }

public:
    Array() {}

    Array(const Array &) = delete;

    ~Array() {
        delete[] m_Data;
    }

    void push_back(const T &value) {
        if (m_Capacity <= m_Size)
            grow(m_Capacity * 2 + 4);

        m_Data[m_Size++] = value;
    }

    T pop_back() {
        assert(m_Size > 0);
        m_Size--;
        return m_Data[m_Size];
    }

    size_t size() const {
        return m_Size;
    }

    T &at(size_t index) {
        assert(index < m_Size);
        return m_Data[index];
    }

    const T &at(size_t index) const {
        assert(index < m_Size);
        return m_Data[index];
    }

    T &operator[](size_t index) {
        return this->at(index);
    }

    const T &operator[](size_t index) const {
        return this->at(index);
    }
};

template <typename T>
std::ostream &operator<<(std::ostream &stream, Array<T> const &array) {
    stream << "{ ";
    for (size_t i = 0; i < array.size(); i++)
        stream << array[i] << ", ";
    return stream << "\b\b }";
}

int main() {
    Array<int> a;

    a.push_back(0);
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);

    assert(a.size() == 5);

    assert(a[0] == 0);
    assert(a[1] == 1);
    assert(a[2] == 2);
    assert(a[3] == 3);
    assert(a[4] == 4);

    a[1] = 3;
    assert(a[1] == 3);

    a.pop_back();
    assert(a.size() == 4);

    const Array<int> &b = a;
    assert(b[0] == 0);

    std::cout << a << std::endl;

    Array<std::string> c;
    c.push_back("hello");
    c.push_back("world");
    c.push_back("!");

    std::cout << c << std::endl;
    
    return 0;
}