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

struct indentManip {
    uint8_t level = 0;

    indentManip(uint8_t level) : level(level) {
    }

    friend std::ostream& operator<<(std::ostream& stream, const indentManip& indent) {
        for (int i = 0; i < indent.level; i++)
            stream << "  ";
        return stream;
    }
};

struct DataType {
    virtual size_t getSize() const = 0;

    virtual bool operator==(const DataType& other) const {
        return typeid(*this) == typeid(other);
    }

    bool operator!=(const DataType& other) const {
        return !(*this == other);
    }

    virtual std::string typeName() const = 0;

    friend std::ostream& operator<<(std::ostream& stream, const DataType& value) {
        return value.print(stream);
    }

protected:
    virtual std::ostream& print(std::ostream& stream, uint8_t level = 0) const {
        return stream << indentManip(level) << this->typeName();
    }
};

struct CDataTypeInt : public DataType {
    // metoda vrátí velikost typu, zde vždy 4
    size_t getSize() const override {
        return 4;
    }

    virtual std::string typeName() const override {
        return "int";
    }
};

struct CDataTypeDouble : public DataType {
    // metoda vrátí velikost typu, zde vždy 8
    size_t getSize() const override {
        return 8;
    }

    virtual std::string typeName() const override {
        return "double";
    }
};

struct CDataTypeEnum : public DataType {
    // metoda vrátí velikost typu, zde vždy 4
    size_t getSize() const override {
        return 4;
    }

    // porovná tento typ s jiným typem, vrátí true, pokud jsou oba typy shodné (oba jsou výčtové typy a mají stejný výčet hodnot ve stejném pořadí),
    bool operator==(const DataType& other) const override {
        return DataType::operator==(other) && options == ((const CDataTypeEnum&)other).options;
    }

    virtual std::string typeName() const override {
        return "enum";
    }

    // metoda přidá další hodnotu do výčtu. Pokud stejná hodnota ve výčtu již existuje, je vyvolaná výjimka (viz ukázkový běh)
    CDataTypeEnum& add(const std::string& option) {
        if (std::find(options.begin(), options.end(), option) != options.end())
            throw std::invalid_argument("Duplicate enum value: " + option);

        options.push_back(option);

        return *this;
    }

protected:
    std::vector<std::string> options;

    // zobrazí název typu do zadaného proudu. Pozor, hodnoty výčtu musí být zobrazené v pořadí zadávání.
    std::ostream& print(std::ostream& stream, uint8_t level = 0) const override {
        DataType::print(stream, level) << std::endl;
        stream << indentManip(level) << '{' << std::endl;

        for (const std::string& option : options)
            stream << indentManip(level + 1) << option;

        stream << indentManip(level) << '}';

        return stream;
    }
};

struct CDataTypeStruct : public DataType {
    // metoda vrátí velikost typu (dle obsažených složek)
    size_t getSize() const override {
        // todo
        return 4;
    }

    // porovná tento typ s jiným typem, vrátí true, pokud jsou oba typy shodné (oba typy jsou struktury, mají stejný počet složek, složky na každé pozici mají stejný typ, jména složek se ale mohou lišit)
    bool operator==(const DataType& other) const override {
        // todo
        return false;
    }

    virtual std::string typeName() const override {
        return "struct";
    }

    // metoda přidá další složku zadaného jména a typu (int/double/enum) na konec seznamu složek. Pokud je jméno složky duplicitní, vyhlásí výjimku (viz ukázkový běh)
    CDataTypeStruct& addField(const std::string& name, const DataType& type) {
        // todo
        return *this;
    }

    // zpřístupní složku zadaného jména, případně vyhodí výjimku pokud složka takového jména neexistuje (viz ukázkový běh). Složka zpřístupněná touto metodou bude pouze čtena
    const DataType& field(const std::string& name) const {
        // todo
    }

protected:
    // zobrazí název typu do zadaného proudu. Pořadí složek odpovídá pořadí jejich přidání metodou addField.
    std::ostream& print(std::ostream& stream, uint8_t level = 0) const override {
        // todo
        return stream;
    }
};

/*

struct CDataTypeArray : public DataType {
    size_t getSize() const override {
        // todo
        return 4;
    }

    bool operator==(const DataType& other) const override {
        // todo
        return false;
    }

    virtual std::string typeName() const override {
        // todo
        return "?";
    }

protected:
    std::ostream& print(std::ostream& stream, uint8_t level = 0) const override {
        // todo
        return stream;
    }
};

struct CDataTypePtr : public DataType {
    size_t getSize() const override {
        // todo
        return 4;
    }

    bool operator==(const DataType& other) const override {
        // todo
        return false;
    }

    virtual std::string typeName() const override {
        // todo
        return "?";
    }

protected:
    std::ostream& print(std::ostream& stream, uint8_t level = 0) const override {
        // todo
        return stream;
    }
};

*/
