#include <cstdint>
#include "repository/ReviewRepository.h"
#include "plugin/DatabasePlugin.h"
#include <spdlog/spdlog.h>

namespace dhivagar::dhivagarmart::repository {

model::Review ReviewRepository::Create(const model::Review& review, const std::string& request_id) {
    spdlog::debug("[{}] ReviewRepository::Create product_id={} user_id={} rating={}", 
                  request_id, review.product_id, review.user_id, review.rating);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::work tx(*conn);

    auto res = tx.exec_params(
        "INSERT INTO reviews (product_id, user_id, rating, comment) "
        "VALUES ($1, $2, $3, $4) "
        "RETURNING id, product_id, user_id, rating, comment, created_at;",
        review.product_id, review.user_id, review.rating, review.comment
    );
    tx.commit();

    if (res.empty()) {
        throw std::runtime_error("Failed to insert review");
    }

    const auto& row = res[0];
    model::Review created;
    created.id = row["id"].as<int64_t>();
    created.product_id = row["product_id"].as<int64_t>();
    created.user_id = row["user_id"].as<int64_t>();
    created.rating = row["rating"].as<int32_t>();
    created.comment = row["comment"].as<std::string>();
    created.created_at = row["created_at"].as<std::string>();
    created.user_name = review.user_name;
    return created;
}

std::vector<model::Review> ReviewRepository::FindByProductId(int64_t product_id, const std::string& request_id) {
    spdlog::debug("[{}] ReviewRepository::FindByProductId product_id={}", request_id, product_id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::nontransaction ntx(*conn);

    auto res = ntx.exec_params(
        "SELECT r.id, r.product_id, r.user_id, r.rating, r.comment, r.created_at, "
        "       u.name AS user_name "
        "FROM reviews r "
        "JOIN users u ON r.user_id = u.id "
        "WHERE r.product_id = $1 "
        "ORDER BY r.id DESC;",
        product_id
    );

    std::vector<model::Review> reviews;
    reviews.reserve(res.size());
    for (const auto& row : res) {
        model::Review rev;
        rev.id = row["id"].as<int64_t>();
        rev.product_id = row["product_id"].as<int64_t>();
        rev.user_id = row["user_id"].as<int64_t>();
        rev.rating = row["rating"].as<int32_t>();
        rev.comment = row["comment"].as<std::string>();
        rev.created_at = row["created_at"].as<std::string>();
        rev.user_name = row["user_name"].as<std::string>();
        reviews.push_back(rev);
    }
    return reviews;
}

bool ReviewRepository::HasUserReviewedProduct(int64_t user_id, int64_t product_id, const std::string& request_id) {
    spdlog::debug("[{}] ReviewRepository::HasUserReviewedProduct user_id={} product_id={}", 
                  request_id, user_id, product_id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::nontransaction ntx(*conn);

    auto res = ntx.exec_params(
        "SELECT 1 FROM reviews WHERE user_id = $1 AND product_id = $2 LIMIT 1;",
        user_id, product_id
    );
    return !res.empty();
}

} // namespace dhivagar::dhivagarmart::repository
