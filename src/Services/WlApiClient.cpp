#include "vat_sentinel/Services/WlApiClient.hpp"
#include <chrono>
#include <cpr/cpr.h>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>

using json = nlohmann::json;

namespace Services {

WlApiClient::WlApiClient() { baseUrl = "https://wl-api.mf.gov.pl/api/search/nip/"; }

void WlApiClient::enrich(Core::Company& company) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_c);

    std::stringstream dateStream;
    dateStream << std::put_time(now_tm, "%Y-%m-%d");
    std::string currentDate = dateStream.str();

    std::string targetUrl = baseUrl + company.nip + "?date=" + currentDate;

    std::cout << "[WlApiClient] Odpytywanie Ministerstwa Finansow (stan na " << currentDate
              << ")...\n";
    cpr::Response r = cpr::Get(cpr::Url{targetUrl});

    if (r.status_code == 200) {
        try {
            json response_json = json::parse(r.text);
            auto subject = response_json["result"]["subject"];

            if (!subject.is_null()) {
                company.name = subject.value("name", "Brak nazwy");
                company.krsNumber = subject.value("krs", "");

                std::string statusVat = subject.value("statusVat", "");
                company.isActiveVat = (statusVat == "Czynny");

                company.registrationDate = subject.value("registrationDate", "");
            } else {
                std::cout << "[WlApiClient] UWAGA: Podmiot nie figuruje w rejestrze VAT!\n";
                company.isActiveVat = false;
            }
        } catch (const json::exception& e) {
            std::cerr << "[WlApiClient] Blad parsowania JSON: " << e.what() << "\n";
        }
    } else {
        std::cerr << "[WlApiClient] Blad sieci API. Kod HTTP: " << r.status_code << "\n";
    }
}

} // namespace Services