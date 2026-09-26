#include "util/Environment.h"
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <algorithm>

namespace dhivagar::dhivagarmart::util {

std::unordered_map<std::string, std::string>& Environment::GetCache() {
    static std::unordered_map<std::string, std::string> cache;
    return cache;
}

void Environment::LoadEnvFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        file.open("../" + filepath);
        if (!file.is_open()) {
            return;
        }
    }

    std::string line;
    while (std::getline(file, line)) {
        // Strip leading whitespace
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));

        // Ignore empty lines or comments
        if (line.empty() || line[0] == '#') {
            continue;
        }

        auto eq_pos = line.find('=');
        if (eq_pos != std::string::npos) {
            std::string key = line.substr(0, eq_pos);
            std::string val = line.substr(eq_pos + 1);

            // Trim key
            while (!key.empty() && std::isspace(static_cast<unsigned char>(key.back()))) {
                key.pop_back();
            }

            // Trim value
            while (!val.empty() && std::isspace(static_cast<unsigned char>(val.back()))) {
                val.pop_back();
            }
            if (!val.empty() && (val.front() == '"' || val.front() == '\'') && val.front() == val.back()) {
                val = val.substr(1, val.length() - 2);
            }

            GetCache()[key] = val;
        }
    }
}

std::string Environment::Get(const std::string& key, const std::string& default_value) {
    const char* env_val = std::getenv(key.c_str());
    if (env_val != nullptr && std::string(env_val).length() > 0) {
        return std::string(env_val);
    }

    auto& cache = GetCache();
    auto it = cache.find(key);
    if (it != cache.end()) {
        return it->second;
    }

    return default_value;
}

int Environment::GetInt(const std::string& key, int default_value) {
    std::string val = Get(key);
    if (val.empty()) {
        return default_value;
    }
    try {
        return std::stoi(val);
    } catch (...) {
        return default_value;
    }
}

bool Environment::GetBool(const std::string& key, bool default_value) {
    std::string val = Get(key);
    if (val.empty()) {
        return default_value;
    }
    std::string lower = val;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower == "true" || lower == "1" || lower == "yes" || lower == "on";
}

} // namespace dhivagar::dhivagarmart::util
