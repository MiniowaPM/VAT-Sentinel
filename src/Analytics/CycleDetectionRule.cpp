#include "vat_sentinel/Analytics/CycleDetectionRule.hpp"
#include <iostream>

namespace Analytics {

CycleDetectionRule::CycleDetectionRule(int maxDepth, int penaltyPoints)
    : maxDepth(maxDepth), penaltyPoints(penaltyPoints) {}

int CycleDetectionRule::evaluate(const Core::Company& company,
                                 const Graph::GraphRepository& graph) const {
    std::unordered_set<std::string> visited;

    bool cycleFound = hasCycle(company.nip, company.nip, graph, visited, 0);

    if (cycleFound) {
        std::cout << "[Risk Rule: DFS] WYKRYTO ZAMKNIETY CYKL FINANSOWY dla NIP: " << company.nip
                  << "!\n";
        return penaltyPoints;
    }

    return 0;
}

bool CycleDetectionRule::hasCycle(const std::string& currentNip, const std::string& startNip,
                                  const Graph::GraphRepository& graph,
                                  std::unordered_set<std::string>& visited,
                                  int currentDepth) const {

    if (currentDepth >= maxDepth) {
        return false;
    }

    auto outgoingTxs = graph.getOutgoingTransactions(currentNip);

    for (const auto& tx : outgoingTxs) {
        const std::string& receiverNip = tx.receiverNip;

        if (receiverNip == startNip) {
            return true;
        }

        if (visited.find(receiverNip) == visited.end()) {
            visited.insert(receiverNip);

            if (hasCycle(receiverNip, startNip, graph, visited, currentDepth + 1)) {
                return true;
            }
        }
    }

    return false;
}

} // namespace Analytics