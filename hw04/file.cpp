#ifndef __PROGTEST__

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#endif  // __PROGTEST__

class CFile {
    template <typename T>
    class Vector {
    public:
        unsigned refCount = 1;
        uint32_t length = 0;
        uint32_t capacity;
        T* data = nullptr;

        Vector(uint32_t reserve = 16) : capacity(reserve), data(new T[sizeof(T) * capacity]) {
        }

        Vector(const Vector<T>& vector) : length(vector.length), capacity(vector.capacity), data(new T[sizeof(T) * vector.capacity]) {
            memcpy(data, vector.data, sizeof(T) * vector.length);
        }

        ~Vector() {
            delete[] data;
        }

        Vector<T>& operator=(const Vector<T>& vector) {
            if (&vector != this) {
                length = vector.length;
                capacity = vector.capacity;
                clear(capacity);
                memcpy(data, vector.data, sizeof(T) * vector.length);
            }

            return *this;
        }

        void push(const T& value) {
            if (length + 1 > capacity)
                grow();

            data[length++] = value;
        }

        void insertArray(const T* source, uint32_t count, uint32_t index) {
            while (index + count > capacity)
                grow();

            memcpy(&data[index], source, count);
            length = std::max(index + count, length);
        }

        T pop() {
            length--;
            return data[length];
        }

        void resize(uint32_t newCapacity) {
            if (capacity == newCapacity)
                return;

            capacity = newCapacity;
            length = std::min(capacity, length);
            T* newData = new T[sizeof(T) * capacity];
            memcpy(newData, data, sizeof(T) * length);
            delete[] data;
            data = newData;
        }

        Vector<T>* attach() {
            refCount++;
            return this;
        }

        void detach() {
            if (--refCount == 0)
                delete this;
        }

        void clear(uint32_t newCapacity = 16) {
            length = 0;
            capacity = newCapacity;
            delete[] data;
            data = new T[sizeof(T) * capacity];
        }

    private:
        void grow() {
            if (capacity == 0)
                capacity = 16;
            else
                capacity = 2 * capacity;

            T* newData = new T[sizeof(T) * capacity];
            memcpy(newData, data, sizeof(T) * length);
            delete[] data;
            data = newData;
        }
    };

    struct DataPositonPair {
        Vector<uint8_t>* data;
        uint32_t position;
    };

    Vector<uint8_t>* _data = nullptr;
    uint32_t _position = 0;
    Vector<DataPositonPair> _history;

public:
    CFile() : _data(new Vector<uint8_t>()), _history(16) {
    }

    CFile(const CFile& file) : _data(file._data->attach()), _position(file._position), _history(file._history) {
        for (uint32_t i = 0; i < _history.length; i++)
            _history.data[i].data->attach();
    }

    ~CFile() {
        _clear();
    }

    CFile& operator=(const CFile& file) {
        if (&file != this) {
            _clear();

            _data = file._data->attach();
            _position = file._position;
            _history = file._history;
            for (uint32_t i = 0; i < _history.length; i++)
                _history.data[i].data->attach();
        }

        return *this;
    }

    bool seek(uint32_t pos) {
        if (pos > _data->length)
            return false;

        _position = pos;
        return true;
    }

    uint32_t read(uint8_t* destination, uint32_t bytes) {
        uint32_t wrote = 0;
        for (; _position < _data->length && wrote < bytes; _position++, wrote++)
            destination[wrote] = _data->data[_position];

        return wrote;
    }

    uint32_t write(const uint8_t* source, uint32_t bytes) {
        if (bytes == 0)
            return bytes;

        _detachData();
        _data->insertArray(source, bytes, _position);
        _position += bytes;

        return bytes;
    }

    void truncate() {
        if (_position == _data->capacity)
            return;

        _detachData();
        _data->resize(_position);
    }

    uint32_t fileSize() const {
        return _data->length;
    }

    void addVersion() {
        _history.push({_data->attach(), _position});
    }

    bool undoVersion() {
        if (_history.length == 0)
            return false;

        _data->detach();
        auto version = _history.pop();
        _data = version.data;
        _position = version.position;

        return true;
    }

private:
    void _detachData() {
        if (_data->refCount > 1) {
            _data->detach();
            _data = new Vector<uint8_t>(*_data);
        }
    }

    void _clear() {
        _data->detach();
        _data = nullptr;

        for (uint32_t i = 0; i < _history.length; i++)
            _history.data[i].data->detach();

        _history.clear();
    }
};
