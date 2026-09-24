cmake_minimum_required(VERSION 3.25)
project(DhivagarMart VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(ENABLE_TESTS "Build Dhivagar Mart unit and integration test suite" ON)
option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

# Common Compiler Warnings
if(MSVC)
    add_compile_options(/W4 /permissive- /Zc:__cplusplus)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-deprecated-declarations)
endif()

# Sanitizers configuration
if(ENABLE_SANITIZERS AND NOT MSVC)
    message(STATUS "Enabling AddressSanitizer and UndefinedBehaviorSanitizer")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

# Required Packages via vcpkg
find_package(Drogon CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)
find_package(unofficial-sodium CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(httplib CONFIG REQUIRED)

if(WIN32)
    if(TARGET Trantor::Trantor)
        get_target_property(_trantor_defs Trantor::Trantor INTERFACE_COMPILE_DEFINITIONS)
        if(_trantor_defs)
            list(REMOVE_ITEM _trantor_defs "_WIN32_WINNT=0x0601")
            set_target_properties(Trantor::Trantor PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${_trantor_defs}")
        endif()
    endif()
    if(TARGET httplib::httplib)
        get_target_property(_httplib_defs httplib::httplib INTERFACE_COMPILE_DEFINITIONS)
        if(_httplib_defs)
            list(FILTER _httplib_defs EXCLUDE REGEX "CPPHTTPLIB_USE_NON_BLOCKING_GETADDRINFO")
            set_target_properties(httplib::httplib PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${_httplib_defs}")
        endif()
    endif()
    add_compile_definitions(_WIN32_WINNT=0x0A00 WINVER=0x0A00)
endif()

# Common Core Sources
set(DHIVAGARMART_CORE_SOURCES
    src/util/Environment.cpp
    src/util/PasswordUtil.cpp
    src/util/ValidationUtil.cpp
    src/exception/GlobalExceptionHandler.cpp
    src/plugin/DatabasePlugin.cpp
    src/plugin/MigrationPlugin.cpp
    src/repository/UserRepository.cpp
    src/repository/ProductRepository.cpp
    src/repository/CartRepository.cpp
    src/repository/OrderRepository.cpp
    src/repository/ReviewRepository.cpp
    src/service/AuthService.cpp
    src/service/ProductService.cpp
    src/service/CartService.cpp
    src/service/OrderService.cpp
    src/service/ReviewService.cpp
    src/service/SellerService.cpp
    src/service/AdminService.cpp
    src/service/MigrationService.cpp
    src/service/ChatService.cpp
    src/filter/LoggingFilter.cpp
    src/filter/AuthFilter.cpp
    src/filter/CorsFilter.cpp
    src/controller/AuthController.cpp
    src/controller/ProductController.cpp
    src/controller/CartController.cpp
    src/controller/OrderController.cpp
    src/controller/ReviewController.cpp
    src/controller/SellerController.cpp
    src/controller/AdminController.cpp
    src/controller/ChatController.cpp
    src/controller/HealthController.cpp
)

# Main Executable: DhivagarMart
add_executable(DhivagarMart
    src/main.cpp
    ${DHIVAGARMART_CORE_SOURCES}
)

target_include_directories(DhivagarMart PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(DhivagarMart PRIVATE
    Drogon::Drogon
    libpqxx::pqxx
    unofficial-sodium::sodium
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    httplib::httplib
)

if(WIN32)
    target_link_libraries(DhivagarMart PRIVATE ws2_32 wsock32 crypt32)
endif()

# Automated Testing Suite: DhivagarMartTests
if(ENABLE_TESTS)
    find_package(GTest CONFIG REQUIRED)
    enable_testing()

    add_executable(DhivagarMartTests
        test/unit/MoneyTest.cpp
        test/unit/ValidationTest.cpp
        test/unit/AuthServiceTest.cpp
        test/unit/ProductServiceTest.cpp
        test/unit/CartServiceTest.cpp
        test/unit/OrderServiceTest.cpp
        test/repository/RepositoryTest.cpp
        test/api/ApiIntegrationTest.cpp
        ${DHIVAGARMART_CORE_SOURCES}
    )

    target_include_directories(DhivagarMartTests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    target_link_libraries(DhivagarMartTests PRIVATE
        GTest::gtest
        GTest::gmock
        GTest::gtest_main
        Drogon::Drogon
        libpqxx::pqxx
        unofficial-sodium::sodium
        nlohmann_json::nlohmann_json
        spdlog::spdlog
        httplib::httplib
    )

    if(WIN32)
        target_link_libraries(DhivagarMartTests PRIVATE ws2_32 wsock32 crypt32)
    endif()

    include(GoogleTest)
    gtest_discover_tests(DhivagarMartTests)
endif()
