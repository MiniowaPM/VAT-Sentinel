#pragma once
#include <string>
#include <vector>
#include "vat_sentinel/Core/Transaction.hpp"

namespace Data {

class CsvTransactionReader {
public:
    CsvTransactionReader() = default;

    std::vector<Core::Transaction> read(const std::string& filepath);

    std::vector<std::string> getUniqueNips(const std::vector<Core::Transaction>& transactions);
};

} // namespace Data