#include "vat_sentinel/Data/CsvTransactionReader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_set>

namespace Data {

std::vector<Core::Transaction> CsvTransactionReader::read(const std::string& filepath) {
    std::vector<Core::Transaction> transactions;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "[CSV Reader] BŁĄD: Nie mozna otworzyc pliku: " << filepath << "\n";
        return transactions;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string token;
        Core::Transaction tx;

        // Zakładamy format CSV: TransactionID,SenderNIP,ReceiverNIP,Amount,Currency,PaymentFormat,Timestamp,Title
        std::getline(ss, tx.transactionId, ',');
        std::getline(ss, tx.senderNip, ',');
        std::getline(ss, tx.receiverNip, ',');
        
        std::getline(ss, token, ',');
        try {
            tx.amount = std::stod(token);
        } catch (...) {
            tx.amount = 0.0;
        }

        std::getline(ss, tx.currency, ',');
        std::getline(ss, tx.paymentFormat, ',');
        std::getline(ss, tx.timestamp, ',');
        std::getline(ss, tx.title, ',');

        transactions.push_back(tx);
    }

    std::cout << "[CSV Reader] Wczytano " << transactions.size() << " transakcji z pliku.\n";
    return transactions;
}

std::vector<std::string> CsvTransactionReader::getUniqueNips(const std::vector<Core::Transaction>& transactions) {
    std::unordered_set<std::string> uniqueSet;
    
    for (const auto& tx : transactions) {
        uniqueSet.insert(tx.senderNip);
        uniqueSet.insert(tx.receiverNip);
    }

    std::cout << "[CSV Reader] Znaleziono " << uniqueSet.size() << " unikalnych podmiotow.\n";
    return std::vector<std::string>(uniqueSet.begin(), uniqueSet.end());
}

} // namespace Data