#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "vat_sentinel/Analytics/CycleDetectionRule.hpp"
#include "vat_sentinel/Analytics/RiskAnalyzer.hpp"
#include "vat_sentinel/Core/Company.hpp"
#include "vat_sentinel/Core/Transaction.hpp"
#include "vat_sentinel/Data/CsvTransactionReader.hpp"
#include "vat_sentinel/Graph/GraphRepository.hpp"
#include "vat_sentinel/Services/CompanyDataAggregator.hpp"
#include "vat_sentinel/Services/KrsApiClient.hpp"
#include "vat_sentinel/Services/WlApiClient.hpp"

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    std::cout << "=== VAT Sentinel: Inicjalizacja Systemu ===\n\n";

    Services::CompanyDataAggregator aggregator;
    aggregator.addEnricher(std::make_unique<Services::WlApiClient>());
    aggregator.addEnricher(std::make_unique<Services::KrsApiClient>());

    Data::CsvTransactionReader csvReader;
    std::string csvPath = "../data/transactions.csv";

    std::cout << "--- ETAP 1: Wczytywanie transakcji ---\n";
    auto transactions = csvReader.read(csvPath);

    if (transactions.empty()) {
        std::cerr << "[SYSTEM] Brak transakcji do analizy. Koniec programu.\n";
        return 1;
    }

    auto uniqueNips = csvReader.getUniqueNips(transactions);

    std::cout << "\n--- ETAP 2: Pobieranie danych o podmiotach (OSINT) ---\n";
    std::vector<Core::Company> companyProfiles;

    for (const auto& nip : uniqueNips) {
        std::cout << "\n>>> Przetwarzanie NIP: " << nip << " <<<\n";
        Core::Company profile = aggregator.buildCompanyProfile(nip);
        companyProfiles.push_back(profile);
    }

    std::cout << "\n--- ETAP 3: Budowa Sieci Finansowej (Graph Engine) ---\n";
    Graph::GraphRepository graph;

    for (const auto& profile : companyProfiles) {
        graph.addNode(profile);
    }

    for (const auto& tx : transactions) {
        graph.addEdge(tx);
    }

    std::cout << "[Graph Engine] Inicjalizacja pamieci RAM zakonczona!\n";
    std::cout << "[Graph Engine] Liczba wezlow (zbadanych firm): " << graph.getNodeCount() << "\n";
    std::cout << "[Graph Engine] Liczba krawedzi (przelewow):      " << graph.getEdgeCount()
              << "\n";

    std::cout << "\n--- ETAP 4: Analiza Ryzyka AML (Risk Engine) ---\n";
    Analytics::RiskAnalyzer analyzer;

    analyzer.addRule(std::make_unique<Analytics::CycleDetectionRule>(5, 100));

    std::cout << "\n=== RAPORT KONCOWY ===\n";
    for (const auto& nip : uniqueNips) {
        Core::Company profile = graph.getNode(nip);

        analyzer.analyze(profile, graph);

        auto outgoingTxs = graph.getOutgoingTransactions(nip);

        std::cout << "NIP:\t\t" << profile.nip << "\n";
        std::cout << "Nazwa:\t\t" << (profile.name.empty() ? "BRAK DANYCH" : profile.name) << "\n";
        std::cout << "Kapital:\t" << std::fixed << std::setprecision(2) << profile.shareCapital
                  << " PLN\n";
        std::cout << "Przelewy wych.:\t" << outgoingTxs.size() << " transakcji\n";

        if (profile.riskScore > 0) {
            std::cout << "Risk Score:\t[ !!! " << profile.riskScore
                      << " pkt !!! ] (WYSOKIE RYZYKO)\n";
        } else {
            std::cout << "Risk Score:\t" << profile.riskScore << " pkt (Brak flag)\n";
        }
        std::cout << "--------------------------------------\n";
    }

    return 0;
}