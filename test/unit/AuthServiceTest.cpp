#include <cstdint>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "service/AuthService.h"
#include "repository/IUserRepository.h"
#include "exception/AppException.h"
#include "util/PasswordUtil.h"

using namespace dhivagar::dhivagarmart;
using ::testing::_;
using ::testing::Return;

class MockUserRepository : public repository::IUserRepository {
public:
    MOCK_METHOD(model::User, Create, (const model::User& user, const std::string& request_id), (override));
    MOCK_METHOD(std::optional<model::User>, FindById, (int64_t id, const std::string& request_id), (override));
    MOCK_METHOD(std::optional<model::User>, FindByEmail, (const std::string& email, const std::string& request_id), (override));
    MOCK_METHOD(std::vector<model::User>, FindAll, (const std::string& request_id), (override));
};

TEST(AuthServiceTest, RegisterSuccess) {
    auto mock_repo = std::make_shared<MockUserRepository>();
    service::AuthService auth_service(mock_repo);

    dto::RegisterRequestDto req{
        .name = "Alice Green",
        .email = "alice@example.com",
        .password = "SecretPass123",
        .role = "BUYER"
    };

    EXPECT_CALL(*mock_repo, FindByEmail("alice@example.com", _))
        .WillOnce(Return(std::nullopt));

    model::User created_user{
        .id = 10,
        .name = "Alice Green",
        .email = "alice@example.com",
        .password_hash = "argon_hash",
        .role = model::UserRole::kBuyer,
        .created_at = "2026-09-24 12:00:00"
    };

    EXPECT_CALL(*mock_repo, Create(_, _))
        .WillOnce(Return(created_user));

    auto res = auth_service.Register(req);
    EXPECT_EQ(res.id, 10);
    EXPECT_EQ(res.name, "Alice Green");
    EXPECT_EQ(res.email, "alice@example.com");
    EXPECT_EQ(res.role, "BUYER");
}

TEST(AuthServiceTest, RegisterDuplicateEmailThrowsConflict) {
    auto mock_repo = std::make_shared<MockUserRepository>();
    service::AuthService auth_service(mock_repo);

    dto::RegisterRequestDto req{
        .name = "Bob Smith",
        .email = "bob@example.com",
        .password = "SecretPass123",
        .role = "BUYER"
    };

    model::User existing{
        .id = 5,
        .name = "Existing Bob",
        .email = "bob@example.com",
        .password_hash = "hash",
        .role = model::UserRole::kBuyer
    };

    EXPECT_CALL(*mock_repo, FindByEmail("bob@example.com", _))
        .WillOnce(Return(existing));

    EXPECT_THROW(auth_service.Register(req), exception::ConflictException);
}

TEST(AuthServiceTest, LoginSuccess) {
    auto mock_repo = std::make_shared<MockUserRepository>();
    service::AuthService auth_service(mock_repo);

    std::string raw_pass = "BuyerPass123!";
    std::string hash = util::PasswordUtil::HashPassword(raw_pass);

    model::User existing{
        .id = 7,
        .name = "Carol White",
        .email = "carol@example.com",
        .password_hash = hash,
        .role = model::UserRole::kBuyer,
        .created_at = "2026-09-24 10:00:00"
    };

    EXPECT_CALL(*mock_repo, FindByEmail("carol@example.com", _))
        .WillOnce(Return(existing));

    dto::LoginRequestDto login_req{
        .email = "carol@example.com",
        .password = raw_pass
    };

    auto user = auth_service.Login(login_req);
    EXPECT_EQ(user.id, 7);
    EXPECT_EQ(user.email, "carol@example.com");
}

TEST(AuthServiceTest, LoginWrongPasswordThrowsAuthenticationException) {
    auto mock_repo = std::make_shared<MockUserRepository>();
    service::AuthService auth_service(mock_repo);

    std::string hash = util::PasswordUtil::HashPassword("CorrectPass123");

    model::User existing{
        .id = 8,
        .name = "David Brown",
        .email = "david@example.com",
        .password_hash = hash,
        .role = model::UserRole::kBuyer
    };

    EXPECT_CALL(*mock_repo, FindByEmail("david@example.com", _))
        .WillOnce(Return(existing));

    dto::LoginRequestDto login_req{
        .email = "david@example.com",
        .password = "WrongPassword"
    };

    EXPECT_THROW(auth_service.Login(login_req), exception::AuthenticationException);
}
