# 🛡️ VAT Sentinel (RegTech/AML Engine)

**Automating financial anomaly detection. Built to secure systems against VAT carousel fraud and money laundering.**

![C++](https://img.shields.io/badge/Core-C%2B%2B_17-blue)
![CMake](https://img.shields.io/badge/Build-CMake-green)
![Network](https://img.shields.io/badge/Network-cpr-orange)
![Data](https://img.shields.io/badge/Data-nlohmann%2Fjson-yellow)
![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)

## 🚀 About The Project

**VAT Sentinel** was born out of a deep dive into how financial crimes, specifically VAT carousels and money laundering, operate at a structural level. Detecting these anomalies in massive datasets of transactions is like finding a needle in a haystack—if the needle was constantly moving and hiding behind shell companies.

This project tackles the problem using a data-driven, modular RegTech architecture. Built purely in C++ for maximum performance, the application ingests massive CSV transaction logs, constructs an in-memory financial graph, and automatically cross-references entities with real-time Polish Government APIs (KRS, MF White List) to detect suspicious patterns and calculate accurate risk scores.

## ✨ Key Features

### 🔌 OSINT Data Pipeline

- **Real-time Enrichment:** Automatically fetches live entity data using the Polish Ministry of Finance (Biała Lista VAT) and Ministry of Justice (KRS) APIs.
- **Fault Tolerance:** Robust handling of missing data, API rate limits, and network errors without interrupting the analysis flow.
- **Data Parsing:** Custom parsers for converting complex Polish financial formats and dynamic currency strings into machine-readable data.

### 🕸️ High-Performance Graph Engine

- **In-Memory Graph:** Models complex financial networks using an optimized Adjacency List (`std::unordered_map`), linking `Company` nodes with `Transaction` edges.
- **Massive Ingestion:** Fast, custom-built CSV parser designed to load and map synthetic AML datasets into core memory structures.

### ⚖️ Rule-Based Risk Engine

- **Cycle Detection:** Utilizes Depth-First Search (DFS) algorithms to identify closed-loop transaction chains typical of VAT carousels.
- **Cross-Checking:** Simulates OGNIVO-like systems to verify if transactions are originating from officially registered bank accounts.

## 🏗️ Architecture & Tech Stack

The project consists of a highly optimized C++ backend utilizing modern software engineering design patterns to ensure scalability and maintainability.

- **Language:** Modern C++ (C++17 standard)
- **Build System:** CMake (3.15+)
- **Dependencies:** `cpr` (C++ Requests), `nlohmann/json`
- **Design Patterns:** Dependency Inversion (Interfaces), Aggregator (Pipeline), Strategy (Rule Engine).

### Project Structure

```text
VAT-Sentinel/
├── data/                   # Synthetic CSV datasets (transactions, external mocks)
├── include/vat_sentinel/   # Header files (.hpp)
│   ├── Core/               # Data models (Company, Transaction)
│   ├── Data/               # File parsers (CSV Reader)
│   ├── Graph/              # In-memory graph structures
│   └── Services/           # API clients & Enrichment Pipeline
├── src/                    # Source files (.cpp)
└── CMakeLists.txt          # Build configuration
```

## 📥 Installation

### Prerequisites

    - C++17 compatible compiler (MSVC, GCC, or Clang)

    - CMake (3.15+)

    - Git

### Setup

```Bash
# Clone the repository

git clone [https://github.com/twoj-profil/VAT-Sentinel.git](https://github.com/twoj-profil/VAT-Sentinel.git)
cd VAT-Sentinel

# Create a build directory

mkdir build
cd build

# Generate build files using CMake

cmake ..

# Compile the project

cmake --build .
```

## 💻 Usage Example

Once compiled, run the executable. The engine will automatically ingest data from data/transactions.csv, build the graph, and query live APIs.

```Bash
./VATSentinel
```

Expected Console Output:

```Plaintext
=== VAT Sentinel: System Initialization ===

--- STEP 1: Ingesting Transactions ---
[CSV Reader] Loaded 15000 transactions.
[CSV Reader] Extracted 432 unique entities.

--- STEP 2: OSINT Data Fetching ---

> > > Processing NIP: 7740001454 <<<
> > > [WlApiClient] Querying Ministry of Finance...
> > > [KrsApiClient] Found share capital: 1451177561.25 PLN
> > > [KrsApiClient] Extracted 5 board members.

=== FINAL REPORT ===
NIP: 7740001454
Name: ORLEN SPOLKA AKCYJNA
Capital: 1451177561.25 PLN
VAT Status: [ACTIVE]
Risk Score: 0
```

## 🗺️ Roadmap

    [x] Implement Core Data Models

    [x] Build API Aggregator Pipeline (WlApiClient, KrsApiClient)

    [x] Create Custom CSV Transaction Parser

    [ ] Implement GraphRepository (In-Memory Adjacency List)

    [ ] Add DFS Algorithm for VAT Carousel Detection

    [ ] Integrate Mock-OGNIVO Cross-Checking System

    [ ] Future: Machine Learning (ONNX) Integration for anomaly scoring

## 📸 Gallery

(Miejsce na zrzuty ekranu z Twojego terminala. Możesz podmienić linki, gdy zrobisz screeny działającego programu)

## 📄 License

Distributed under the MIT License. See LICENSE for more information.

## ⚠️ Disclaimer

For educational and research purposes only. This tool was created as a programming challenge to explore C++ performance, graph theory, and RegTech architecture. The data processed should be synthetic or anonymized. The author assumes no liability for the misuse of this software in actual financial auditing or compliance without proper legal and regulatory oversight.
