#pragma once
#include "vat_sentinel/Core/Company.hpp"
#include "vat_sentinel/Core/Transaction.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace Graph {

class GraphRepository {
  private:
    // Węzły: NIP -> Profil Firmy
    std::unordered_map<std::string, Core::Company> nodes;

    // Krawędzie (Lista sąsiedztwa): NIP Nadawcy -> Lista Transakcji Wychodzących
    std::unordered_map<std::string, std::vector<Core::Transaction>> adjacencyList;

  public:
    GraphRepository() = default;

    // Dodawanie elementów do grafu
    void addNode(const Core::Company& company);
    void addEdge(const Core::Transaction& transaction);

    // Pobieranie danych i sprawdzanie powiązań
    bool nodeExists(const std::string& nip) const;
    Core::Company getNode(const std::string& nip) const;
    std::vector<Core::Transaction> getOutgoingTransactions(const std::string& nip) const;

    // Statystyki
    size_t getNodeCount() const;
    size_t getEdgeCount() const;
};

} // namespace Graph