#include "vat_sentinel/Analytics/RiskAnalyzer.hpp"

namespace Analytics {

void RiskAnalyzer::addRule(std::unique_ptr<IRiskRule> rule) { rules.push_back(std::move(rule)); }

void RiskAnalyzer::analyze(Core::Company& company, const Graph::GraphRepository& graph) const {
    int totalScore = 0;

    for (const auto& rule : rules) {
        totalScore += rule->evaluate(company, graph);
    }

    company.riskScore = totalScore;
}

} // namespace Analytics