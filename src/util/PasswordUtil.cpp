#include "util/PasswordUtil.h"
#include <sodium.h>
#include <stdexcept>
#include <mutex>

namespace dhivagar::dhivagarmart::util {

static std::once_flag g_sodium_init_flag;

bool PasswordUtil::Initialize() {
    bool success = true;
    std::call_once(g_sodium_init_flag, [&]() {
        if (sodium_init() < 0) {
            success = false;
        }
    });
    return success;
}

std::string PasswordUtil::HashPassword(const std::string& plaintext_password) {
    if (!Initialize()) {
        throw std::runtime_error("Failed to initialize libsodium for password hashing");
    }

    char hashed_password[crypto_pwhash_STRBYTES];
    if (crypto_pwhash_str(
            hashed_password,
            plaintext_password.c_str(),
            plaintext_password.length(),
            crypto_pwhash_OPSLIMIT_INTERACTIVE,
            crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
        throw std::runtime_error("Argon2id password hashing failed (out of memory)");
    }

    return std::string(hashed_password);
}

bool PasswordUtil::VerifyPassword(const std::string& hash, const std::string& plaintext_password) {
    if (!Initialize()) {
        throw std::runtime_error("Failed to initialize libsodium for password verification");
    }

    if (hash.empty() || plaintext_password.empty()) {
        return false;
    }

    return crypto_pwhash_str_verify(
        hash.c_str(),
        plaintext_password.c_str(),
        plaintext_password.length()) == 0;
}

} // namespace dhivagar::dhivagarmart::util
