#pragma once
#include <string>
#include <vector>

namespace Core {

struct BoardMember {
    std::string firstName;
    std::string lastName;
    std::string personalId;
    std::string role;

    BoardMember() = default;
};

struct Address {
    std::string street;
    std::string buildingNumber;
    std::string city;
    std::string postalCode;

    Address() = default;
};

struct Company {
    std::string nip;
    std::string krsNumber;
    std::string name;
    Address registeredAddress;

    // Z Białej Listy VAT
    bool isActiveVat = false;
    std::vector<std::string> registeredBankAccounts;
    std::string registrationDate;

    // Z KRS
    double shareCapital = 0.0; 
    bool isInBankruptcy = false;
    std::vector<BoardMember> boardMembers;

    // Z REGON
    std::string mainPkdCode;

    int riskScore = 0;

    Company() = default;
};

} // namespace Core