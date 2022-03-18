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
            if (stringA.size() != stringB.size())
                return stringA.size() < stringB.size();

            for (size_t i = 0; i < stringA.size(); i++) {
                auto lowerA = std::tolower(stringA[i]);
                auto lowerB = std::tolower(stringB[i]);
                if (lowerA != lowerB)
                    return lowerA < lowerB;
            }

            return false;
        }
    };

    std::vector<Company*> _companies;       // sorted by name and address
    std::vector<Company*> _companiesTaxID;  // sorted by id
    std::vector<unsigned int> _invoices;    // sorted by value

public:
    CVATRegister() {
        _companies.reserve(1000);
        _companiesTaxID.reserve(1000);
        _invoices.reserve(1000);
    }

    ~CVATRegister() {
        for (const Company* company : _companies)
            delete company;
    }

    // Metoda newCompany(name, addr, id) přidá do existující databáze další záznam. Parametry name a addr reprezentují jméno a adresu, parametr id udává daňový identifikátor. Metoda vrací hodnotu true, pokud byl záznam přidán, nebo hodnotu false, pokud přidán nebyl (protože již v databázi existoval záznam se stejným jménem a adresou, nebo záznam se stejným id).
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

    // Metody cancelCompany (name, addr) / cancelCompany (id) odstraní záznam z databáze. Parametrem je jednoznačná identifikace pomocí jména a adresy (první varianta) nebo pomocí daňového identifikátoru (druhá varianta). Pokud byl záznam skutečně odstraněn, vrátí metoda hodnotu true. Pokud záznam neodstraní (protože neexistovala firma s touto identifikací), vrátí metoda hodnotu false.
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

    // Metody cancelCompany (name, addr) / cancelCompany (id) odstraní záznam z databáze. Parametrem je jednoznačná identifikace pomocí jména a adresy (první varianta) nebo pomocí daňového identifikátoru (druhá varianta). Pokud byl záznam skutečně odstraněn, vrátí metoda hodnotu true. Pokud záznam neodstraní (protože neexistovala firma s touto identifikací), vrátí metoda hodnotu false.
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

    // Metody invoice (name, addr, amount) / invoice (id, amount) zaznamenají příjem ve výši amount. Varianty jsou dvě - firma je buď identifikována svým jménem a adresou, nebo daňovým identifikátorem. Pokud metoda uspěje, vrací true, pro neúspěch vrací false (neexistující firma).
    bool invoice(const std::string& name, const std::string& addr, unsigned int amount) {
        return _invoice(_findCompanyNameAddress(name, addr), amount);
    }

    // Metody invoice (name, addr, amount) / invoice (id, amount) zaznamenají příjem ve výši amount. Varianty jsou dvě - firma je buď identifikována svým jménem a adresou, nebo daňovým identifikátorem. Pokud metoda uspěje, vrací true, pro neúspěch vrací false (neexistující firma).
    bool invoice(const std::string& taxID, unsigned int amount) {
        return _invoice(_findCompanyTaxID(taxID), amount);
    }

    // Metoda audit (name, addr, sum) / audit (id, sum) vyhledá součet příjmů pro firmu se zadaným jménem a adresou nebo firmu zadanou daňovým identifikátorem. Nalezený součet uloží do výstupního parametru sum. Metoda vrátí true pro úspěch, false pro selhání (neexistující firma).
    bool audit(const std::string& name, const std::string& addr, unsigned int& sumIncome) const {
        return _audit(_findCompanyNameAddress(name, addr), sumIncome);
    }

    // Metoda audit (name, addr, sum) / audit (id, sum) vyhledá součet příjmů pro firmu se zadaným jménem a adresou nebo firmu zadanou daňovým identifikátorem. Nalezený součet uloží do výstupního parametru sum. Metoda vrátí true pro úspěch, false pro selhání (neexistující firma).
    bool audit(const std::string& taxID, unsigned int& sumIncome) const {
        return _audit(_findCompanyTaxID(taxID), sumIncome);
    }

    // Metody firstCompany (name, addr) / nextCompany (name, addr) slouží k procházení existujícího seznamu firem v naší databázi. Firmy jsou procházené v abecedním pořadí podle jejich jména. Pokud mají dvě firmy stejná jména, rozhoduje o pořadí jejich adresa. Metoda firstCompany nalezne první firmu. Pokud je seznam firem prázdný, vrátí metoda hodnotu false. V opačném případě vrátí metoda hodnotu true a vyplní výstupní parametry name a addr.
    bool firstCompany(std::string& name, std::string& addr) const {
        if (_companies.size() == 0)
            return false;

        auto company = _companies[0];
        name = company->name;
        addr = company->address;

        return true;
    }

    // Metody firstCompany (name, addr) / nextCompany (name, addr) slouží k procházení existujícího seznamu firem v naší databázi. Firmy jsou procházené v abecedním pořadí podle jejich jména. Pokud mají dvě firmy stejná jména, rozhoduje o pořadí jejich adresa. Metoda nextCompany funguje obdobně, nalezne další firmu, která v seznamu následuje za firmou určenou parametry. Pokud za name a addr již v seznamu není další firma, metoda vrací hodnotu false. V opačném případě metoda vrátí true a přepíše parametry name a addr jménem a adresou následující firmy.
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

    // Metoda medianInvoice () vyhledá medián hodnoty faktury. Do vypočteného mediánu se započtou všechny úspěšně zpracované faktury zadané voláním invoice. Tedy nezapočítávají se faktury, které nešlo přiřadit (volání invoice selhalo), ale započítávají se všechny dosud registrované faktury, tedy při výmazu firmy se neodstraňují její faktury z výpočtu mediánu. Pokud je v systému zadaný sudý počet faktur, vezme se vyšší ze dvou prostředních hodnot. Pokud systém zatím nezpracoval žádnou fakturu, bude vrácena hodnota 0.
    unsigned int medianInvoice() const {
        if (_invoices.size() == 0)
            return 0;

        return _invoices[_invoices.size() / 2];
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
    // Metody invoice (name, addr, amount) / invoice (id, amount) zaznamenají příjem ve výši amount. Varianty jsou dvě - firma je buď identifikována svým jménem a adresou, nebo daňovým identifikátorem. Pokud metoda uspěje, vrací true, pro neúspěch vrací false (neexistující firma).
    bool _invoice(Company* company, unsigned int amount) {
        if (!company)
            return false;

        company->invoicesSum += amount;
        _invoices.insert(
            std::upper_bound(_invoices.begin(), _invoices.end(), amount),
            amount);

        return true;
    }

    // Metoda audit (name, addr, sum) / audit (id, sum) vyhledá součet příjmů pro firmu se zadaným jménem a adresou nebo firmu zadanou daňovým identifikátorem. Nalezený součet uloží do výstupního parametru sum. Metoda vrátí true pro úspěch, false pro selhání (neexistující firma).
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
