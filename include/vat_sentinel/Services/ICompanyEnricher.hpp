#pragma once
#include "vat_sentinel/Core/Company.hpp"

namespace Services {

class ICompanyEnricher {
  public:
    virtual ~ICompanyEnricher() = default;

    virtual void enrich(Core::Company& company) = 0;
};

} // namespace Services