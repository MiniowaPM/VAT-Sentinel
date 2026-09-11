#include "vat_sentinel/Services/KrsApiClient.hpp"
#include <algorithm>
#include <cctype>
#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Services {

KrsApiClient::KrsApiClient() {
    // Oficjalne API Ministerstwa Sprawiedliwości dla Rejestru Przedsiębiorców
    baseUrl = "https://api-krs.ms.gov.pl/api/krs/OdpisAktualny/";
}

void KrsApiClient::enrich(Core::Company& company) {
    if (company.krsNumber.empty()) {
        std::cout << "[KrsApiClient] Pomijam, brak numeru KRS dla podmiotu.\n";
        return;
    }

    std::string formattedKrs = company.krsNumber;
    while (formattedKrs.length() < 10) {
        formattedKrs = "0" + formattedKrs;
    }
    std::string targetUrl = baseUrl + formattedKrs + "?rejestr=P&format=json";

    std::cout << "[KrsApiClient] Odpytywanie rejestru KRS dla numeru: " << company.krsNumber
              << "...\n";
    cpr::Response r = cpr::Get(cpr::Url{targetUrl});

    if (r.status_code == 200) {
        try {
            json response = json::parse(r.text);
            auto odp = response["odpis"]["dane"];

            auto kapital = odp["dzial1"]["kapital"];
            if (!kapital.is_null() && !kapital["wysokoscKapitaluZakladowego"].is_null()) {
                std::string wartosc =
                    kapital["wysokoscKapitaluZakladowego"]["wartosc"].get<std::string>();
                std::cout << "[KrsApiClient] Surowy kapital z API: " << wartosc << " PLN\n";

                std::string cleanedValue = wartosc;

                cleanedValue.erase(
                    std::remove_if(cleanedValue.begin(), cleanedValue.end(), ::isspace),
                    cleanedValue.end());

                std::replace(cleanedValue.begin(), cleanedValue.end(), ',', '.');

                try {
                    company.shareCapital = std::stod(cleanedValue);
                    std::cout << "[KrsApiClient] Sparsowany kapital: " << company.shareCapital
                              << "\n";
                } catch (const std::exception& e) {
                    std::cerr << "[KrsApiClient] UWAGA: Nie udalo sie sparsowac kapitalu: "
                              << e.what() << "\n";
                    company.shareCapital = 0.0;
                }
            }

            auto reprezentacja = odp["dzial2"]["reprezentacja"];
            if (!reprezentacja.is_null()) {
                auto osoby = reprezentacja["osobyWchodzaceWSkladOrganu"];
                for (const auto& osoba : osoby) {
                    Core::BoardMember member;
                    member.firstName = osoba.value("imiona", "Brak imienia");
                    member.lastName = osoba.value("nazwisko", "Brak nazwiska");

                    auto id = osoba["identyfikator"];
                    if (!id.is_null()) {
                        member.personalId = id.value("pesel", "Brak PESEL");
                    }

                    member.role = osoba.value("funkcjaWOrganie", "Członek Zarządu");
                    company.boardMembers.push_back(member);
                }
                std::cout << "[KrsApiClient] Znaleziono " << company.boardMembers.size()
                          << " czlonkow zarzadu.\n";
            }
        } catch (const json::exception& e) {
            std::cerr << "[KrsApiClient] Blad parsowania JSON: " << e.what() << "\n";
        }
    } else {
        std::cerr << "[KrsApiClient] Blad sieci API KRS. Kod HTTP: " << r.status_code << "\n";
    }
}

} // namespace Services