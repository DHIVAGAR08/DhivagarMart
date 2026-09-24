#pragma once

#include <string>
#include <unordered_map>

namespace dhivagar::dhivagarmart::util {

class Environment {
public:
    static void LoadEnvFile(const std::string& filepath = ".env");
    static std::string Get(const std::string& key, const std::string& default_value = "");
    static int GetInt(const std::string& key, int default_value);
    static bool GetBool(const std::string& key, bool default_value);

private:
    static std::unordered_map<std::string, std::string>& GetCache();
};

} // namespace dhivagar::dhivagarmart::util
