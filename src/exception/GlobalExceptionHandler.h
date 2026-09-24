#pragma once

#include <drogon/HttpResponse.h>
#include <exception>
#include <string>

namespace dhivagar::dhivagarmart::exception {

class GlobalExceptionHandler {
public:
    static drogon::HttpResponsePtr HandleException(const std::exception& e, const std::string& request_id = "");
    static drogon::HttpResponsePtr HandleUnknownException(const std::string& request_id = "");
};

} // namespace dhivagar::dhivagarmart::exception
