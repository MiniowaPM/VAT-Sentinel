#pragma once
#include "vat_sentinel/Core/Company.hpp"
#include "vat_sentinel/Services/ICompanyEnricher.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Services {

class CompanyDataAggregator {
  private:
    std::vector<std::unique_ptr<ICompanyEnricher>> enrichers;

  public:
    CompanyDataAggregator() = default;

    void addEnricher(std::unique_ptr<ICompanyEnricher> enricher);

    Core::Company buildCompanyProfile(const std::string& nip);
};

} // namespace Services