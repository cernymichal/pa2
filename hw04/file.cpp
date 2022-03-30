#ifndef __PROGTEST__

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#endif /* __PROGTEST__ */

class CFile {
    template <typename T>
    class Vector {
    public:
        unsigned refCount = 1;
        uint32_t length = 0;
        uint32_t capacity;
        T* data = nullptr;

        Vector(uint32_t reserve = 256) : capacity(reserve), data(new T[sizeof(T) * capacity]) {
        }

        Vector(const Vector<T>& vector) {
            // todo
        }

        ~Vector() {
            delete[] data;
        }

        void push(const T& value) {
            if (length + 1 > capacity)
                grow();

            data[length++] = value;
        }

        void pushBytes(const uint8_t* source, uint32_t bytes) {
            while (length + bytes / sizeof(T) > capacity)
                grow();

            memcpy(&data[length], source, bytes);
            length += bytes / sizeof(T);
        }

        void shrink() {
            capacity = length;
            T* newData = new T[sizeof(T) * capacity];
            memcpy(newData, data, sizeof(T) * length);
            delete[] data;
            data = newData;
        }

    private:
        void grow() {
            capacity = 32 + 2 * capacity;
            T* newData = new T[sizeof(T) * capacity];
            memcpy(newData, data, sizeof(T) * length);
            delete[] data;
            data = newData;
        }
    };

    Vector<uint8_t>* _data = nullptr;
    uint32_t _position = 0;
    Vector<Vector<uint8_t>*> _history;

public:
    CFile() : _data(new Vector<uint8_t>()), _history(16) {
    }

    CFile(const CFile& file) {
        // todo
    }

    ~CFile() {
        if (--_data->refCount == 0)
            delete _data;

        for (int i = 0; i < _history.length; i++) {
            if (--_history.data[i]->refCount == 0)
                delete _history.data[i];
        }
    }

    CFile& operator=(const CFile& file) {
        // todo
    }

    bool seek(uint32_t offset) {
        if (_position + offset < 0 || _position + offset > _data->length)
            return false;

        _position += offset;
        return true;
    }

    uint32_t read(uint8_t* destination, uint32_t bytes) {
        uint32_t wrote = 0;
        for (; _position < _data->length && wrote < bytes; _position++, wrote++)
            destination[wrote] = _data->data[_position];

        return wrote;
    }

    uint32_t write(const uint8_t* source, uint32_t bytes) {
        _detachData();
        // todo
    }

    void truncate() {
        _detachData();
        _data->shrink();
    }

    uint32_t fileSize() const {
        return _data->length;
    }

    void addVersion() {
        _data->refCount++;
        _history.push(_data);
    }

    bool undoVersion() {
        if (--_history.data[i]->refCount == 0)
                delete _history.data[i];
    }

private:
    void _detachData() {
        if (_data->refCount > 1)
            _data = new Vector<uint8_t>(*_data);
    }
};
