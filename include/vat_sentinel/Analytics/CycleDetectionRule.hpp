#pragma once
#include "vat_sentinel/Analytics/IRiskRule.hpp"
#include <string>
#include <unordered_set>

namespace Analytics {

class CycleDetectionRule : public IRiskRule {
  private:
    int maxDepth;
    int penaltyPoints;

    // DFS
    bool hasCycle(const std::string& currentNip, const std::string& startNip,
                  const Graph::GraphRepository& graph, std::unordered_set<std::string>& visited,
                  int currentDepth) const;

  public:
    CycleDetectionRule(int maxDepth = 5, int penaltyPoints = 80);

    int evaluate(const Core::Company& company, const Graph::GraphRepository& graph) const override;
};

} // namespace Analytics