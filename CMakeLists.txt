cmake_minimum_required(VERSION 3.20)
project(VATSentinel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Force static libraries" FORCE)

include(FetchContent)

message(STATUS "Pobieranie biblioteki CPR (HTTP Client)...")

FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.11.1
)
FetchContent_MakeAvailable(cpr)

message(STATUS "Pobieranie biblioteki nlohmann/json...")
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

# Definicja naszego programu

add_executable(VATSentinel 
    src/main.cpp
    src/Services/WlApiClient.cpp
    src/Services/CompanyDataAggregator.cpp
    src/Services/KrsApiClient.cpp
)

target_include_directories(VATSentinel PUBLIC include)

target_link_libraries(VATSentinel PRIVATE 
    cpr::cpr 
    nlohmann_json::nlohmann_json
)