#ifndef __PROGTEST__

#pragma once

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

#endif /* __PROGTEST__ */

class CVATRegister {
public:
    CVATRegister() {
        // todo
    }

    ~CVATRegister() {
        // todo
    }

    bool newCompany(const std::string& name, const std::string& addr, const std::string& taxID) {
        // todo

        return false;
    }

    bool cancelCompany(const std::string& name, const std::string& addr) {
        // todo

        return false;
    }

    bool cancelCompany(const std::string& taxID) {
        // todo

        return false;
    }

    bool invoice(const std::string& taxID, unsigned int amount) {
        // todo

        return false;
    }

    bool invoice(const std::string& name, const std::string& addr, unsigned int amount) {
        // todo

        return false;
    }

    bool audit(const std::string& name, const std::string& addr, unsigned int& sumIncome) const {
        // todo

        return false;
    }

    bool audit(const std::string& taxID, unsigned int& sumIncome) const {
        // todo

        return false;
    }

    bool firstCompany(std::string& name, std::string& addr) const {
        // todo

        return false;
    }

    bool nextCompany(std::string& name, std::string& addr) const {
        // todo

        return false;
    }

    unsigned int medianInvoice() const {
        // todo

        return 0;
    }

private:
    // todo
};
