#pragma once
#include <string>

namespace Core {

struct Transaction {
    std::string transactionId;
    std::string senderNip;
    std::string receiverNip;

    double amount;
    std::string currency;

    // Format z IBM AML
    std::string paymentFormat;
    std::string timestamp;
    std::string title;

    Transaction() = default;
};

} // namespace Core