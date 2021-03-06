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

#endif  // __PROGTEST__

class CVATRegister {
    class Company {
    public:
        std::string name;
        std::string address;
        std::string taxID;
        unsigned int invoicesSum = 0;

        Company(const std::string& name, const std::string& address, const std::string& taxID)
            : name(name), address(address), taxID(taxID) {
        }

        Company(const std::string& name, const std::string& address)
            : name(name), address(address) {
        }

        Company(const std::string& taxID)
            : taxID(taxID) {
        }

        // check if taxID of companyA is less than companyBs
        static bool ltTaxID(const Company* companyA, const Company* companyB) {
            return companyA->taxID < companyB->taxID;
        }

        // check if name and address of companyA are less than companyBs
        static bool ltNameAddress(const Company* companyA, const Company* companyB) {
            if (equalStringsCI(companyA->name, companyB->name))
                return ltStringsCI(companyA->address, companyB->address);

            return ltStringsCI(companyA->name, companyB->name);
        }

        // check if taxIDs are equal
        static bool eqTaxID(const Company* companyA, const Company* companyB) {
            return companyA->taxID == companyB->taxID;
        }

        // check if names and addresses are equal
        static bool eqNameAddress(const Company* companyA, const Company* companyB) {
            return equalStringsCI(companyA->name, companyB->name) && equalStringsCI(companyA->address, companyB->address);
        }

    private:
        static bool equalStringsCI(const std::string& stringA, const std::string& stringB) {
            if (stringA.size() != stringB.size())
                return false;

            for (size_t i = 0; i < stringA.size(); i++) {
                if (std::tolower(stringA[i]) != std::tolower(stringB[i]))
                    return false;
            }

            return true;
        }

        static bool ltStringsCI(const std::string& stringA, const std::string& stringB) {
            for (size_t i = 0; i < stringA.size() && i < stringB.size(); i++) {
                auto lowerA = std::tolower(stringA[i]);
                auto lowerB = std::tolower(stringB[i]);
                if (lowerA != lowerB)
                    return lowerA < lowerB;
            }

            return stringA.size() < stringB.size();
        }
    };

    class MedianHeap {
        class Heap {
            std::vector<unsigned int> data;
            bool max = true;

        public:
            Heap(bool max = true) : max(max) {
                data.reserve(1000);
            }

            size_t size() const {
                return data.size();
            }

            void add(unsigned int value) {
                data.push_back(value);
                fixUp();
            }

            unsigned int top() const {
                return data[0];
            }

            unsigned int pop() {
                auto temp = data[0];

                data[0] = data.back();
                data.pop_back();
                fixDown();

                return temp;
            }

        private:
            bool parentExists(size_t index) const {
                return index >= 1;
            }

            bool leftExists(size_t index) const {
                return leftIndex(index) < data.size();
            }

            bool rightExists(size_t index) const {
                return rightIndex(index) < data.size();
            }

            size_t parentIndex(size_t index) const {
                return (index - 1) / 2;
            }

            size_t leftIndex(size_t index) const {
                return index * 2 + 1;
            }

            size_t rightIndex(size_t index) const {
                return index * 2 + 2;
            }

            bool compare(unsigned int a, unsigned int b) const {
                if (max)
                    return a > b;
                else
                    return a < b;
            }

            void fixUp() {
                size_t i = data.size() - 1;
                while (parentExists(i) && compare(data[i], data[parentIndex(i)])) {
                    std::swap(data[parentIndex(i)], data[i]);
                    i = parentIndex(i);
                }
            }

            void fixDown() {
                size_t i = 0;
                while (leftExists(i)) {
                    size_t bestChild = leftIndex(i);
                    if (rightExists(i) && compare(data[rightIndex(i)], data[bestChild]))
                        bestChild = rightIndex(i);

                    if (compare(data[i], data[bestChild]))
                        break;

                    std::swap(data[i], data[bestChild]);
                    i = bestChild;
                }
            }
        };

        Heap maxHeap;
        Heap minHeap;

    public:
        MedianHeap() : maxHeap(true), minHeap(false) {
        }

        unsigned int median() const {
            if (maxHeap.size() == 0 && minHeap.size() == 0)
                return 0;

            if (minHeap.size() == 0 || minHeap.size() < maxHeap.size())
                return maxHeap.top();

            return minHeap.top();
        }

        void add(unsigned int value) {
            if (value < median())
                maxHeap.add(value);
            else
                minHeap.add(value);

            rebalance();
        }

    private:
        void rebalance() {
            if (minHeap.size() + 1 < maxHeap.size())
                minHeap.add(maxHeap.pop());
            else if (maxHeap.size() + 1 < minHeap.size())
                maxHeap.add(minHeap.pop());
        }
    };

    std::vector<Company*> _companies;       // sorted by name and address
    std::vector<Company*> _companiesTaxID;  // sorted by id
    MedianHeap _invoices;

public:
    CVATRegister() : _invoices() {
        _companies.reserve(1000);
        _companiesTaxID.reserve(1000);
    }

    ~CVATRegister() {
        for (const Company* company : _companies)
            delete company;
    }

    // Metoda newCompany(name, addr, id) p??id?? do existuj??c?? datab??ze dal???? z??znam. Parametry name a addr reprezentuj?? jm??no a adresu, parametr id ud??v?? da??ov?? identifik??tor. Metoda vrac?? hodnotu true, pokud byl z??znam p??id??n, nebo hodnotu false, pokud p??id??n nebyl (proto??e ji?? v datab??zi existoval z??znam se stejn??m jm??nem a adresou, nebo z??znam se stejn??m id).
    bool newCompany(const std::string& name, const std::string& addr, const std::string& taxID) {
        // company with matching name and address or taxID was found
        if (_findCompanyNameAddress(name, addr) || _findCompanyTaxID(taxID))
            return false;

        auto newCompany = new Company(name, addr, taxID);

        _companies.insert(
            std::upper_bound(_companies.begin(), _companies.end(), newCompany, Company::ltNameAddress),
            newCompany);

        _companiesTaxID.insert(
            std::upper_bound(_companiesTaxID.begin(), _companiesTaxID.end(), newCompany, Company::ltTaxID),
            newCompany);

        return true;
    }

    // Metody cancelCompany (name, addr) / cancelCompany (id) odstran?? z??znam z datab??ze. Parametrem je jednozna??n?? identifikace pomoc?? jm??na a adresy (prvn?? varianta) nebo pomoc?? da??ov??ho identifik??toru (druh?? varianta). Pokud byl z??znam skute??n?? odstran??n, vr??t?? metoda hodnotu true. Pokud z??znam neodstran?? (proto??e neexistovala firma s touto identifikac??), vr??t?? metoda hodnotu false.
    bool cancelCompany(const std::string& name, const std::string& addr) {
        auto companiesPos = _findCompaniesIter(name, addr);

        // couldn't find company
        if (companiesPos == _companies.end())
            return false;

        auto company = *companiesPos.base();
        auto companiesTaxIDPos = _findCompaniesTaxIDIter(company->taxID);

        _companies.erase(companiesPos);
        _companiesTaxID.erase(companiesTaxIDPos);
        delete company;

        return true;
    }

    // Metody cancelCompany (name, addr) / cancelCompany (id) odstran?? z??znam z datab??ze. Parametrem je jednozna??n?? identifikace pomoc?? jm??na a adresy (prvn?? varianta) nebo pomoc?? da??ov??ho identifik??toru (druh?? varianta). Pokud byl z??znam skute??n?? odstran??n, vr??t?? metoda hodnotu true. Pokud z??znam neodstran?? (proto??e neexistovala firma s touto identifikac??), vr??t?? metoda hodnotu false.
    bool cancelCompany(const std::string& taxID) {
        auto companiesTaxIDPos = _findCompaniesTaxIDIter(taxID);

        // couldn't find company
        if (companiesTaxIDPos == _companiesTaxID.end())
            return false;

        auto company = *companiesTaxIDPos.base();
        auto companiesPos = _findCompaniesIter(company->name, company->address);

        _companies.erase(companiesPos);
        _companiesTaxID.erase(companiesTaxIDPos);
        delete company;

        return true;
    }

    // Metody invoice (name, addr, amount) / invoice (id, amount) zaznamenaj?? p????jem ve v????i amount. Varianty jsou dv?? - firma je bu?? identifikov??na sv??m jm??nem a adresou, nebo da??ov??m identifik??torem. Pokud metoda usp??je, vrac?? true, pro ne??sp??ch vrac?? false (neexistuj??c?? firma).
    bool invoice(const std::string& name, const std::string& addr, unsigned int amount) {
        return _invoice(_findCompanyNameAddress(name, addr), amount);
    }

    // Metody invoice (name, addr, amount) / invoice (id, amount) zaznamenaj?? p????jem ve v????i amount. Varianty jsou dv?? - firma je bu?? identifikov??na sv??m jm??nem a adresou, nebo da??ov??m identifik??torem. Pokud metoda usp??je, vrac?? true, pro ne??sp??ch vrac?? false (neexistuj??c?? firma).
    bool invoice(const std::string& taxID, unsigned int amount) {
        return _invoice(_findCompanyTaxID(taxID), amount);
    }

    // Metoda audit (name, addr, sum) / audit (id, sum) vyhled?? sou??et p????jm?? pro firmu se zadan??m jm??nem a adresou nebo firmu zadanou da??ov??m identifik??torem. Nalezen?? sou??et ulo???? do v??stupn??ho parametru sum. Metoda vr??t?? true pro ??sp??ch, false pro selh??n?? (neexistuj??c?? firma).
    bool audit(const std::string& name, const std::string& addr, unsigned int& sumIncome) const {
        return _audit(_findCompanyNameAddress(name, addr), sumIncome);
    }

    // Metoda audit (name, addr, sum) / audit (id, sum) vyhled?? sou??et p????jm?? pro firmu se zadan??m jm??nem a adresou nebo firmu zadanou da??ov??m identifik??torem. Nalezen?? sou??et ulo???? do v??stupn??ho parametru sum. Metoda vr??t?? true pro ??sp??ch, false pro selh??n?? (neexistuj??c?? firma).
    bool audit(const std::string& taxID, unsigned int& sumIncome) const {
        return _audit(_findCompanyTaxID(taxID), sumIncome);
    }

    // Metody firstCompany (name, addr) / nextCompany (name, addr) slou???? k proch??zen?? existuj??c??ho seznamu firem v na???? datab??zi. Firmy jsou proch??zen?? v abecedn??m po??ad?? podle jejich jm??na. Pokud maj?? dv?? firmy stejn?? jm??na, rozhoduje o po??ad?? jejich adresa. Metoda firstCompany nalezne prvn?? firmu. Pokud je seznam firem pr??zdn??, vr??t?? metoda hodnotu false. V opa??n??m p????pad?? vr??t?? metoda hodnotu true a vypln?? v??stupn?? parametry name a addr.
    bool firstCompany(std::string& name, std::string& addr) const {
        if (_companies.size() == 0)
            return false;

        auto company = _companies[0];
        name = company->name;
        addr = company->address;

        return true;
    }

    // Metody firstCompany (name, addr) / nextCompany (name, addr) slou???? k proch??zen?? existuj??c??ho seznamu firem v na???? datab??zi. Firmy jsou proch??zen?? v abecedn??m po??ad?? podle jejich jm??na. Pokud maj?? dv?? firmy stejn?? jm??na, rozhoduje o po??ad?? jejich adresa. Metoda nextCompany funguje obdobn??, nalezne dal???? firmu, kter?? v seznamu n??sleduje za firmou ur??enou parametry. Pokud za name a addr ji?? v seznamu nen?? dal???? firma, metoda vrac?? hodnotu false. V opa??n??m p????pad?? metoda vr??t?? true a p??ep????e parametry name a addr jm??nem a adresou n??sleduj??c?? firmy.
    bool nextCompany(std::string& name, std::string& addr) const {
        auto companyPos = _findCompaniesIter(name, addr);

        // move to the next company
        std::advance(companyPos, 1);

        // no next company
        if (companyPos == _companies.end())
            return false;

        auto company = *companyPos.base();
        name = company->name;
        addr = company->address;

        return true;
    }

    // Metoda medianInvoice () vyhled?? medi??n hodnoty faktury. Do vypo??ten??ho medi??nu se zapo??tou v??echny ??sp????n?? zpracovan?? faktury zadan?? vol??n??m invoice. Tedy nezapo????t??vaj?? se faktury, kter?? ne??lo p??i??adit (vol??n?? invoice selhalo), ale zapo????t??vaj?? se v??echny dosud registrovan?? faktury, tedy p??i v??mazu firmy se neodstra??uj?? jej?? faktury z v??po??tu medi??nu. Pokud je v syst??mu zadan?? sud?? po??et faktur, vezme se vy?????? ze dvou prost??edn??ch hodnot. Pokud syst??m zat??m nezpracoval ????dnou fakturu, bude vr??cena hodnota 0.
    unsigned int medianInvoice() const {
        return _invoices.median();
    }

    // print company vectors
    void printCompanies() const {
        std::cout << "_companies: ";
        for (const Company* company : _companies)
            std::cout << company->name << " (" << company->address << "), ";
        std::cout << "\b\b" << std::endl;

        std::cout << "_companiesID: ";
        for (const Company* company : _companiesTaxID)
            std::cout << company->taxID << ", ";
        std::cout << "\b\b" << std::endl;
    }

private:
    // Metody invoice (name, addr, amount) / invoice (id, amount) zaznamenaj?? p????jem ve v????i amount. Varianty jsou dv?? - firma je bu?? identifikov??na sv??m jm??nem a adresou, nebo da??ov??m identifik??torem. Pokud metoda usp??je, vrac?? true, pro ne??sp??ch vrac?? false (neexistuj??c?? firma).
    bool _invoice(Company* company, unsigned int amount) {
        if (!company)
            return false;

        company->invoicesSum += amount;
        _invoices.add(amount);

        return true;
    }

    // Metoda audit (name, addr, sum) / audit (id, sum) vyhled?? sou??et p????jm?? pro firmu se zadan??m jm??nem a adresou nebo firmu zadanou da??ov??m identifik??torem. Nalezen?? sou??et ulo???? do v??stupn??ho parametru sum. Metoda vr??t?? true pro ??sp??ch, false pro selh??n?? (neexistuj??c?? firma).
    bool _audit(const Company* company, unsigned int& sumIncome) const {
        if (!company)
            return false;

        sumIncome = company->invoicesSum;

        return true;
    }

    // find company by name and address, returns _companies iterator
    std::vector<CVATRegister::Company*>::const_iterator _findCompaniesIter(const std::string& name, const std::string& address) const {
        if (_companies.size() == 0)
            return _companies.end();

        Company key(name, address);
        auto companyPos = std::lower_bound(_companies.begin(), _companies.end(), &key, Company::ltNameAddress);

        // position not found or not matching company at position
        if (companyPos == _companies.end() || !Company::eqNameAddress(&key, *companyPos.base()))
            return _companies.end();

        return companyPos;
    }

    // find company by taxID, returns _companiesTaxID iterator
    std::vector<CVATRegister::Company*>::const_iterator _findCompaniesTaxIDIter(const std::string& taxID) const {
        if (_companiesTaxID.size() == 0)
            return _companiesTaxID.end();

        Company key(taxID);
        auto companyPos = std::lower_bound(_companiesTaxID.begin(), _companiesTaxID.end(), &key, Company::ltTaxID);

        // position not found or not matching company at position
        if (companyPos == _companiesTaxID.end() || !Company::eqTaxID(&key, *companyPos.base()))
            return _companiesTaxID.end();

        return companyPos;
    }

    // find company by name and address
    Company* _findCompanyNameAddress(const std::string& name, const std::string& address) const {
        auto companyPos = _findCompaniesIter(name, address);

        // company not found
        if (companyPos == _companies.end())
            return nullptr;

        return *companyPos.base();
    }

    // find company by taxID
    Company* _findCompanyTaxID(const std::string& taxID) const {
        auto companyPos = _findCompaniesTaxIDIter(taxID);

        // company not found
        if (companyPos == _companiesTaxID.end())
            return nullptr;

        return *companyPos.base();
    }
};
