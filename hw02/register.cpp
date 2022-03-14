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

bool compareStringsCI(const std::string& stringA, const std::string& stringB) {
    if (stringA.size() != stringB.size())
        return false;

    for (size_t i = 0; i < stringA.size(); i++) {
        if (std::tolower(stringA[i]) != std::tolower(stringB[i]))
            return false;
    }

    return true;
}

class CVATRegister {
    class Company {
    public:
        std::string name;
        std::string address;
        std::string taxID;
        std::vector<unsigned int> invoices;

        Company(const std::string& name, const std::string& address, const std::string& taxID)
            : name(name), address(address), taxID(taxID) {
        }

        bool operator<(const Company& other) const {
            if (name == other.name)
                return address < other.address;

            return name < other.name;
        }
    };

    std::vector<Company> _companies;
    std::vector<unsigned int> _invoicesSum;

public:
    CVATRegister() {
    }

    ~CVATRegister() {
    }

    // Metoda newCompany(name, addr, id) přidá do existující databáze další záznam. Parametry name a addr reprezentují jméno a adresu, parametr id udává daňový identifikátor. Metoda vrací hodnotu true, pokud byl záznam přidán, nebo hodnotu false, pokud přidán nebyl (protože již v databázi existoval záznam se stejným jménem a adresou, nebo záznam se stejným id).
    bool newCompany(const std::string& name, const std::string& addr, const std::string& taxID) {
        for (const Company& company : _companies) {
            if ((compareStringsCI(company.name, name) && compareStringsCI(company.address, addr)) || company.taxID == taxID)
                return false;
        }

        Company newCompany(name, addr, taxID);

        _companies.insert(
            std::upper_bound(_companies.begin(), _companies.end(), newCompany),
            newCompany);

        return true;
    }

    // Metody cancelCompany (name, addr) / cancelCompany (id) odstraní záznam z databáze. Parametrem je jednoznačná identifikace pomocí jména a adresy (první varianta) nebo pomocí daňového identifikátoru (druhá varianta). Pokud byl záznam skutečně odstraněn, vrátí metoda hodnotu true. Pokud záznam neodstraní (protože neexistovala firma s touto identifikací), vrátí metoda hodnotu false.
    bool cancelCompany(const std::string& name, const std::string& addr) {
        // todo

        return false;
    }

    // Metody cancelCompany (name, addr) / cancelCompany (id) odstraní záznam z databáze. Parametrem je jednoznačná identifikace pomocí jména a adresy (první varianta) nebo pomocí daňového identifikátoru (druhá varianta). Pokud byl záznam skutečně odstraněn, vrátí metoda hodnotu true. Pokud záznam neodstraní (protože neexistovala firma s touto identifikací), vrátí metoda hodnotu false.
    bool cancelCompany(const std::string& taxID) {
        // todo

        return false;
    }

    // Metody invoice (name, addr, amount) / invoice (id, amount) zaznamenají příjem ve výši amount. Varianty jsou dvě - firma je buď identifikována svým jménem a adresou, nebo daňovým identifikátorem. Pokud metoda uspěje, vrací true, pro neúspěch vrací false (neexistující firma).
    bool invoice(const std::string& taxID, unsigned int amount) {
        // todo

        return false;
    }

    // Metody invoice (name, addr, amount) / invoice (id, amount) zaznamenají příjem ve výši amount. Varianty jsou dvě - firma je buď identifikována svým jménem a adresou, nebo daňovým identifikátorem. Pokud metoda uspěje, vrací true, pro neúspěch vrací false (neexistující firma).
    bool invoice(const std::string& name, const std::string& addr, unsigned int amount) {
        // todo

        return false;
    }

    // Metoda audit (name, addr, sum) / audit (id, sum) vyhledá součet příjmů pro firmu se zadaným jménem a adresou nebo firmu zadanou daňovým identifikátorem. Nalezený součet uloží do výstupního parametru sum. Metoda vrátí true pro úspěch, false pro selhání (neexistující firma).
    bool audit(const std::string& name, const std::string& addr, unsigned int& sumIncome) const {
        // todo

        return false;
    }
    // Metoda audit (name, addr, sum) / audit (id, sum) vyhledá součet příjmů pro firmu se zadaným jménem a adresou nebo firmu zadanou daňovým identifikátorem. Nalezený součet uloží do výstupního parametru sum. Metoda vrátí true pro úspěch, false pro selhání (neexistující firma).
    bool audit(const std::string& taxID, unsigned int& sumIncome) const {
        // todo

        return false;
    }

    // Metody firstCompany (name, addr) / nextCompany (name, addr) slouží k procházení existujícího seznamu firem v naší databázi. Firmy jsou procházené v abecedním pořadí podle jejich jména. Pokud mají dvě firmy stejná jména, rozhoduje o pořadí jejich adresa. Metoda firstCompany nalezne první firmu. Pokud je seznam firem prázdný, vrátí metoda hodnotu false. V opačném případě vrátí metoda hodnotu true a vyplní výstupní parametry name a addr. Metoda nextCompany funguje obdobně, nalezne další firmu, která v seznamu následuje za firmou určenou parametry. Pokud za name a addr již v seznamu není další firma, metoda vrací hodnotu false. V opačném případě metoda vrátí true a přepíše parametry name a addr jménem a adresou následující firmy.
    bool firstCompany(std::string& name, std::string& addr) const {
        // todo

        return false;
    }

    // Metody firstCompany (name, addr) / nextCompany (name, addr) slouží k procházení existujícího seznamu firem v naší databázi. Firmy jsou procházené v abecedním pořadí podle jejich jména. Pokud mají dvě firmy stejná jména, rozhoduje o pořadí jejich adresa. Metoda firstCompany nalezne první firmu. Pokud je seznam firem prázdný, vrátí metoda hodnotu false. V opačném případě vrátí metoda hodnotu true a vyplní výstupní parametry name a addr. Metoda nextCompany funguje obdobně, nalezne další firmu, která v seznamu následuje za firmou určenou parametry. Pokud za name a addr již v seznamu není další firma, metoda vrací hodnotu false. V opačném případě metoda vrátí true a přepíše parametry name a addr jménem a adresou následující firmy.
    bool nextCompany(std::string& name, std::string& addr) const {
        // todo

        return false;
    }

    // Metoda medianInvoice () vyhledá medián hodnoty faktury. Do vypočteného mediánu se započtou všechny úspěšně zpracované faktury zadané voláním invoice. Tedy nezapočítávají se faktury, které nešlo přiřadit (volání invoice selhalo), ale započítávají se všechny dosud registrované faktury, tedy při výmazu firmy se neodstraňují její faktury z výpočtu mediánu. Pokud je v systému zadaný sudý počet faktur, vezme se vyšší ze dvou prostředních hodnot. Pokud systém zatím nezpracoval žádnou fakturu, bude vrácena hodnota 0.
    unsigned int medianInvoice() const {
        // todo

        return 0;
    }

private:
    // todo
};
