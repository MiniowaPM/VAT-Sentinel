#include "vat_sentinel/Core/Company.hpp"
#include "vat_sentinel/Services/CompanyDataAggregator.hpp"
#include "vat_sentinel/Services/KrsApiClient.hpp"
#include "vat_sentinel/Services/WlApiClient.hpp"
#include <iomanip>
#include <iostream>
#include <memory>

int main() {
    std::cout << "=== VAT Sentinel: Inicjalizacja Systemu ===\n\n";

    Services::CompanyDataAggregator aggregator;

    aggregator.addEnricher(std::make_unique<Services::WlApiClient>());
    aggregator.addEnricher(std::make_unique<Services::KrsApiClient>());

    // Definiujemy NIP do zbadania (7780001414 to PKN Orlen)
    std::string targetNip = "7740001454";

    Core::Company profile = aggregator.buildCompanyProfile(targetNip);

    std::cout << "\n--- Wynik Analizy (Entity Profile) ---\n";
    std::cout << "NIP:\t\t " << profile.nip << "\n";
    std::cout << "Nazwa:\t\t " << (profile.name.empty() ? "BRAK DANYCH" : profile.name) << "\n";
    std::cout << "KRS:\t\t " << (profile.krsNumber.empty() ? "BRAK" : profile.krsNumber) << "\n";
    std::cout << "Status VAT:\t "
              << (profile.isActiveVat ? "[CZTYNNY]" : "[NIEAKTYWNY / WYKRESLONY - RYZYKO]") << "\n";
    std::cout << "Data Rejestracji:" << profile.registrationDate << "\n";
    std::cout << "Kapital Zakladowy:\t " << std::fixed << std::setprecision(2)
              << profile.shareCapital << " PLN\n";
    std::cout << "Risk Score:\t " << profile.riskScore << "\n";
    std::cout << "--------------------------------------\n";

    return 0;
}