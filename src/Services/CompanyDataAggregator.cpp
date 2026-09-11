#include "vat_sentinel/Services/CompanyDataAggregator.hpp"
#include <iostream>

namespace Services {

void CompanyDataAggregator::addEnricher(std::unique_ptr<ICompanyEnricher> enricher) {
    enrichers.push_back(std::move(enricher));
}

Core::Company CompanyDataAggregator::buildCompanyProfile(const std::string& nip) {
    Core::Company company;
    company.nip = nip;

    std::cout << "[Aggregator] Rozpoczecie budowy profilu dla NIP: " << nip << "...\n";

    for (auto& enricher : enrichers) {
        enricher->enrich(company);
    }

    std::cout << "[Aggregator] Proces agregacji zakonczony.\n";
    return company;
}

} // namespace Services