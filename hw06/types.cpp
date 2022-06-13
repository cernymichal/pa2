#ifndef __PROGTEST__

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#endif  // __PROGTEST__

struct Indent {
    uint8_t level = 0;

    Indent(uint8_t level) : level(level) {
    }

    friend std::ostream& operator<<(std::ostream& stream, const Indent& indent) {
        for (int i = 0; i < indent.level; i++)
            stream << "  ";
        return stream;
    }
};

struct DataType {
    virtual ~DataType() {
    }

    virtual std::shared_ptr<DataType> clone() const = 0;

    virtual size_t getSize() const = 0;

    virtual bool operator==(const DataType& other) const {
        return typeid(*this) == typeid(other);
    }

    bool operator!=(const DataType& other) const {
        return !(*this == other);
    }

    virtual const DataType& field(const std::string& name) const {
        throw std::invalid_argument("Cannot use field() for type: " + printToString());
    }

    const DataType& field(const char* name) const {
        return field(std::string(name));
    }

    virtual const DataType& element() const {
        throw std::invalid_argument("Cannot use element() for type: " + printToString());
    }

    friend std::ostream& operator<<(std::ostream& stream, const DataType& value) {
        return value.print(stream);
    }

    virtual std::ostream& print(std::ostream& stream, uint8_t level = 0) const = 0;

    std::string printToString() const {
        std::ostringstream oss;
        oss << *this;
        return oss.str();
    }
};

struct CDataTypeInt : public DataType {
    virtual std::shared_ptr<DataType> clone() const override {
        return std::make_shared<CDataTypeInt>(*this);
    }

    // metoda vrátí velikost typu, zde vždy 4
    size_t getSize() const override {
        return 4;
    }

    virtual std::ostream& print(std::ostream& stream, uint8_t level = 0) const override {
        return stream << Indent(level) << "int";
    }
};

struct CDataTypeDouble : public DataType {
    virtual std::shared_ptr<DataType> clone() const override {
        return std::make_shared<CDataTypeDouble>(*this);
    }

    // metoda vrátí velikost typu, zde vždy 8
    size_t getSize() const override {
        return 8;
    }

    virtual std::ostream& print(std::ostream& stream, uint8_t level = 0) const override {
        return stream << Indent(level) << "double";
    }
};

struct CDataTypeEnum : public DataType {
    virtual std::shared_ptr<DataType> clone() const override {
        return std::make_shared<CDataTypeEnum>(*this);
    }

    // metoda vrátí velikost typu, zde vždy 4
    size_t getSize() const override {
        return 4;
    }

    // porovná tento typ s jiným typem, vrátí true, pokud jsou oba typy shodné (oba jsou výčtové typy a mají stejný výčet hodnot ve stejném pořadí),
    bool operator==(const DataType& other) const override {
        if (!DataType::operator==(other))
            return false;

        return _options == static_cast<const CDataTypeEnum&>(other)._options;
    }

    // metoda přidá další hodnotu do výčtu. Pokud stejná hodnota ve výčtu již existuje, je vyvolaná výjimka (viz ukázkový běh)
    CDataTypeEnum& add(const std::string& option) {
        if (std::find(_options.begin(), _options.end(), option) != _options.end())
            throw std::invalid_argument("Duplicate enum value: " + option);

        _options.push_back(option);

        return *this;
    }

    CDataTypeEnum& add(const char* option) {
        return add(std::string(option));
    }

    // zobrazí název typu do zadaného proudu. Pozor, hodnoty výčtu musí být zobrazené v pořadí zadávání.
    std::ostream& print(std::ostream& stream, uint8_t level = 0) const override {
        stream << Indent(level) << "enum" << std::endl;
        stream << Indent(level) << '{' << std::endl;

        for (auto iter = _options.begin(); iter != _options.end();) {
            stream << Indent(level + 1) << *iter;

            if (++iter != _options.end())
                stream << ',';

            stream << std::endl;
        }

        stream << Indent(level) << '}';

        return stream;
    }

protected:
    std::list<std::string> _options;
};

struct CDataTypeStruct : public DataType {
    virtual std::shared_ptr<DataType> clone() const override {
        return std::make_shared<CDataTypeStruct>(*this);
    }

    // metoda vrátí velikost typu (dle obsažených složek)
    size_t getSize() const override {
        size_t sum = 0;
        for (const std::pair<std::string, std::shared_ptr<DataType>>& f : _fields)
            sum += f.second->getSize();

        return sum;
    }

    // porovná tento typ s jiným typem, vrátí true, pokud jsou oba typy shodné (oba typy jsou struktury, mají stejný počet složek, složky na každé pozici mají stejný typ, jména složek se ale mohou lišit)
    bool operator==(const DataType& other) const override {
        if (!DataType::operator==(other))
            return false;

        const CDataTypeStruct& otherI = static_cast<const CDataTypeStruct&>(other);

        auto iter = _fields.begin();
        auto iterOther = otherI._fields.begin();
        for (; iter != _fields.end() && iterOther != otherI._fields.end(); iter++, iterOther++) {
            if (*iter->second != *iterOther->second)
                return false;
        }

        return iter == _fields.end() && iterOther == otherI._fields.end();
    }

    // metoda přidá další složku zadaného jména a typu (int/double/enum) na konec seznamu složek. Pokud je jméno složky duplicitní, vyhlásí výjimku (viz ukázkový běh)
    CDataTypeStruct& addField(const std::string& name, const DataType& type) {
        for (const std::pair<std::string, std::shared_ptr<DataType>>& f : _fields) {
            if (f.first == name)
                throw std::invalid_argument("Duplicate field: " + name);
        }

        _fields.emplace_back(name, type.clone());

        return *this;
    }

    CDataTypeStruct& addField(const char* name, const DataType& type) {
        return addField(std::string(name), type);
    }

    // zpřístupní složku zadaného jména, případně vyhodí výjimku pokud složka takového jména neexistuje (viz ukázkový běh). Složka zpřístupněná touto metodou bude pouze čtena
    const DataType& field(const std::string& name) const override {
        for (const std::pair<std::string, std::shared_ptr<DataType>>& f : _fields) {
            if (f.first == name)
                return *f.second;
        }

        throw std::invalid_argument("Unknown field: " + name);
    }

    // zobrazí název typu do zadaného proudu. Pořadí složek odpovídá pořadí jejich přidání metodou addField.
    std::ostream& print(std::ostream& stream, uint8_t level = 0) const override {
        stream << Indent(level) << "struct" << std::endl;
        stream << Indent(level) << '{' << std::endl;

        for (const std::pair<std::string, std::shared_ptr<DataType>>& f : _fields)
            f.second->print(stream, level + 1) << ' ' << f.first << ';' << std::endl;

        stream << Indent(level) << '}';

        return stream;
    }

protected:
    std::list<std::pair<std::string, std::shared_ptr<DataType>>> _fields;
};

struct CDataTypePtr : public DataType {
    // inicializuje instanci třídy, ukazatel odkazuje na datový typ předaný v parametru
    CDataTypePtr(const DataType& type) : _type(type.clone()) {
    }

    virtual std::shared_ptr<DataType> clone() const override {
        return std::make_shared<CDataTypePtr>(*this);
    }

    // metoda vrátí velikost typu (zde vždy 8)
    size_t getSize() const override {
        return 8;
    }

    // zpřístupní typ odkazovaný ukazatelem (viz ukázkový běh)
    virtual const DataType& element() const override {
        return *_type;
    }

    // porovná tento typ s jiným typem, vrátí `true`, pokud jsou oba typy shodné (oba typy ukazatele a odkazují na stejný datový typ)
    bool operator==(const DataType& other) const override {
        if (!DataType::operator==(other))
            return false;

        return static_cast<const CDataTypePtr&>(other)._type == _type;
    }

    // zobrazí název typu do zadaného proudu
    std::ostream& print(std::ostream& stream, uint8_t level = 0) const override {
        // todo
        return _type->print(stream, level) << '*';
    }

protected:
    std::shared_ptr<DataType> _type;
};

struct CDataTypeArray : public CDataTypePtr {
    // inicializuje instanci třídy, pole má velikost size, prvkem pole mají typ type
    CDataTypeArray(size_t size, const DataType& type) : CDataTypePtr(type), _size(size) {
    }

    virtual std::shared_ptr<DataType> clone() const override {
        return std::make_shared<CDataTypeArray>(*this);
    }

    // metoda vrátí velikost typu (podle typu a velikosti)
    size_t getSize() const override {
        return _type->getSize() * _size;
    }

    // porovná tento typ s jiným typem, vrátí `true`, pokud jsou oba typy shodné (oba typu pole, stejná velikost, stejný typ prvku)
    bool operator==(const DataType& other) const override {
        if (!CDataTypePtr::operator==(other))
            return false;

        return static_cast<const CDataTypeArray&>(other)._size == _size;
    }

    // zobrazí název typu do zadaného proudu
    std::ostream& print(std::ostream& stream, uint8_t level = 0) const override {
        // todo
        return _type->print(stream, level) << '[' << _size << ']';
    }

protected:
    size_t _size;
};