#include "vat_sentinel/Graph/GraphRepository.hpp"

namespace Graph {

void GraphRepository::addNode(const Core::Company& company) {
    nodes[company.nip] = company;
}

void GraphRepository::addEdge(const Core::Transaction& transaction) {
    adjacencyList[transaction.senderNip].push_back(transaction);
}

bool GraphRepository::nodeExists(const std::string& nip) const {
    return nodes.find(nip) != nodes.end();
}

Core::Company GraphRepository::getNode(const std::string& nip) const {
    auto it = nodes.find(nip);
    if (it != nodes.end()) {
        return it->second;
    }
    return Core::Company();
}

std::vector<Core::Transaction> GraphRepository::getOutgoingTransactions(const std::string& nip) const {
    auto it = adjacencyList.find(nip);
    if (it != adjacencyList.end()) {
        return it->second;
    }
    return {};
}

size_t GraphRepository::getNodeCount() const {
    return nodes.size();
}

size_t GraphRepository::getEdgeCount() const {
    size_t totalEdges = 0;
    for (const auto& [nip, transactions] : adjacencyList) {
        totalEdges += transactions.size();
    }
    return totalEdges;
}

} // namespace Graph