#include "vat_sentinel/Core/Company.hpp"
#include "vat_sentinel/Core/Transaction.hpp"
#include "vat_sentinel/Services/CompanyDataAggregator.hpp"
#include "vat_sentinel/Services/KrsApiClient.hpp"
#include "vat_sentinel/Services/WlApiClient.hpp"
#include "vat_sentinel/Data/CsvTransactionReader.hpp"

#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#endif

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif
    std::cout << "=== VAT Sentinel: Inicjalizacja Systemu ===\n\n";

Services::CompanyDataAggregator aggregator;
    aggregator.addEnricher(std::make_unique<Services::WlApiClient>());
    aggregator.addEnricher(std::make_unique<Services::KrsApiClient>());

Data::CsvTransactionReader csvReader;


// C:/Users/miko/source/repos/VAT Sentinel/data/transactions.csv
std::string csvPath = "../data/transactions.csv"; 
    
    std::cout << "--- Wczytywanie transakcji ---\n";
    auto transactions = csvReader.read(csvPath);
    
    if (transactions.empty()) {
        std::cerr << "[SYSTEM] Brak transakcji do analizy. Koniec programu.\n";
        return 1;
    }

auto uniqueNips = csvReader.getUniqueNips(transactions);

std::cout << "\n--- Pobieranie danych o podmiotach ---\n";
    std::vector<Core::Company> companyProfiles;
    
    for (const auto& nip : uniqueNips) {
        std::cout << "\n>>> Przetwarzanie NIP: " << nip << " <<<\n";
        Core::Company profile = aggregator.buildCompanyProfile(nip);
        companyProfiles.push_back(profile);
    }

std::cout << "\n=== RAPORT KONCOWY ===\n";
    for (const auto& profile : companyProfiles) {
        std::cout << "NIP:\t\t" << profile.nip << "\n";
        std::cout << "Nazwa:\t\t" << (profile.name.empty() ? "BRAK DANYCH" : profile.name) << "\n";
        std::cout << "Kapital:\t" << std::fixed << std::setprecision(2) << profile.shareCapital << " PLN\n";
        std::cout << "Status VAT:\t" << (profile.isActiveVat ? "[CZTYNNY]" : "[NIEAKTYWNY / WYKRESLONY - RYZYKO]") << "\n";
        std::cout << "--------------------------------------\n";
    }
    return 0;
}