#pragma once
#include "vat_sentinel/Analytics/IRiskRule.hpp"
#include "vat_sentinel/Core/Company.hpp"
#include "vat_sentinel/Graph/GraphRepository.hpp"
#include <memory>
#include <vector>

namespace Analytics {

class RiskAnalyzer {
  private:
    std::vector<std::unique_ptr<IRiskRule>> rules;

  public:
    RiskAnalyzer() = default;

    void addRule(std::unique_ptr<IRiskRule> rule);

    void analyze(Core::Company& company, const Graph::GraphRepository& graph) const;
};

} // namespace Analytics