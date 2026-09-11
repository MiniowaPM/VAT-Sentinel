#pragma once
#include "vat_sentinel/Core/Company.hpp"
#include "vat_sentinel/Services/ICompanyEnricher.hpp"
#include <string>

namespace Services {

class WlApiClient : public ICompanyEnricher {
  private:
    std::string baseUrl;

  public:
    WlApiClient();

    void enrich(Core::Company& company) override;
};

} // namespace Services