#include <cstdint>
#include "service/SellerService.h"
#include "exception/AppException.h"
#include "util/ValidationUtil.h"
#include <spdlog/spdlog.h>

namespace dhivagar::dhivagarmart::service {

SellerService::SellerService(std::shared_ptr<repository::IProductRepository> product_repo,
                             std::shared_ptr<repository::IOrderRepository> order_repo)
    : product_repo_(std::move(product_repo)),
      order_repo_(std::move(order_repo)) {}

std::vector<dto::ProductResponseDto> SellerService::GetSellerProducts(int64_t seller_id, const std::string& request_id) {
    auto products = product_repo_->FindBySellerId(seller_id, request_id);
    std::vector<dto::ProductResponseDto> dtos;
    dtos.reserve(products.size());
    for (const auto& prod : products) {
        dtos.push_back(dto::ProductResponseDto::FromModel(prod));
    }
    return dtos;
}

dto::ProductResponseDto SellerService::CreateProduct(int64_t seller_id, const dto::ProductRequestDto& req, const std::string& request_id) {
    spdlog::info("[{}] SellerService::CreateProduct seller={}: {}", request_id, seller_id, req.name);

    std::string clean_name = util::ValidationUtil::Trim(req.name);
    std::string clean_category = util::ValidationUtil::Trim(req.category);

    if (clean_name.empty()) {
        throw exception::ValidationException("Product name cannot be empty");
    }
    if (clean_category.empty()) {
        throw exception::ValidationException("Product category cannot be empty");
    }
    if (req.price_cents < 0) {
        throw exception::ValidationException("Product price must be non-negative");
    }
    if (req.stock_qty < 0) {
        throw exception::ValidationException("Product stock quantity must be non-negative");
    }

    model::Product product;
    product.seller_id = seller_id;
    product.name = clean_name;
    product.description = util::ValidationUtil::Trim(req.description);
    product.price = model::Money::FromCents(req.price_cents);
    product.stock_qty = req.stock_qty;
    product.category = clean_category;
    product.image_url = util::ValidationUtil::Trim(req.image_url);

    auto created = product_repo_->Create(product, request_id);
    return dto::ProductResponseDto::FromModel(created);
}

dto::ProductResponseDto SellerService::UpdateProduct(
    int64_t seller_id, int64_t product_id, 
    const dto::ProductRequestDto& req, 
    const std::string& request_id) {

    spdlog::info("[{}] SellerService::UpdateProduct seller={} product={}", request_id, seller_id, product_id);

    if (product_id <= 0) {
        throw exception::ValidationException("Invalid product ID");
    }

    auto existing_opt = product_repo_->FindById(product_id, request_id);
    if (!existing_opt.has_value()) {
        throw exception::NotFoundException("Product not found");
    }

    if (existing_opt->seller_id != seller_id) {
        throw exception::AuthorizationException("You are not authorized to modify products belonging to another seller");
    }

    std::string clean_name = util::ValidationUtil::Trim(req.name);
    std::string clean_category = util::ValidationUtil::Trim(req.category);

    if (clean_name.empty()) {
        throw exception::ValidationException("Product name cannot be empty");
    }
    if (clean_category.empty()) {
        throw exception::ValidationException("Product category cannot be empty");
    }
    if (req.price_cents < 0) {
        throw exception::ValidationException("Product price must be non-negative");
    }
    if (req.stock_qty < 0) {
        throw exception::ValidationException("Product stock quantity must be non-negative");
    }

    model::Product updated;
    updated.id = product_id;
    updated.seller_id = seller_id;
    updated.name = clean_name;
    updated.description = util::ValidationUtil::Trim(req.description);
    updated.price = model::Money::FromCents(req.price_cents);
    updated.stock_qty = req.stock_qty;
    updated.category = clean_category;
    updated.image_url = util::ValidationUtil::Trim(req.image_url);

    auto saved = product_repo_->Update(updated, request_id);
    return dto::ProductResponseDto::FromModel(saved);
}

void SellerService::DeleteProduct(int64_t seller_id, int64_t product_id, const std::string& request_id) {
    spdlog::info("[{}] SellerService::DeleteProduct seller={} product={}", request_id, seller_id, product_id);

    if (product_id <= 0) {
        throw exception::ValidationException("Invalid product ID");
    }

    auto existing_opt = product_repo_->FindById(product_id, request_id);
    if (!existing_opt.has_value()) {
        throw exception::NotFoundException("Product not found");
    }

    if (existing_opt->seller_id != seller_id) {
        throw exception::AuthorizationException("You are not authorized to delete products belonging to another seller");
    }

    product_repo_->Delete(product_id, seller_id, request_id);
}

std::vector<dto::OrderResponseDto> SellerService::GetSellerOrders(int64_t seller_id, const std::string& request_id) {
    auto orders = order_repo_->FindBySellerId(seller_id, request_id);
    std::vector<dto::OrderResponseDto> dtos;
    dtos.reserve(orders.size());
    for (const auto& order : orders) {
        dtos.push_back(dto::OrderResponseDto::FromModel(order));
    }
    return dtos;
}

dto::OrderResponseDto SellerService::UpdateOrderStatus(
    int64_t seller_id, int64_t order_id, 
    const std::string& new_status, 
    const std::string& request_id) {

    spdlog::info("[{}] SellerService::UpdateOrderStatus seller={} order={} status={}", 
                 request_id, seller_id, order_id, new_status);

    if (order_id <= 0) {
        throw exception::ValidationException("Invalid order ID");
    }

    auto order_opt = order_repo_->FindById(order_id, request_id);
    if (!order_opt.has_value()) {
        throw exception::NotFoundException("Order not found");
    }

    // Verify order contains products from this seller
    bool has_seller_items = false;
    for (const auto& item : order_opt->items) {
        auto prod = product_repo_->FindById(item.product_id, request_id);
        if (prod && prod->seller_id == seller_id) {
            has_seller_items = true;
            break;
        }
    }
    if (!has_seller_items) {
        throw exception::AuthorizationException("This order does not contain products from your store");
    }

    model::OrderStatus target_status = model::StringToOrderStatus(new_status);
    order_repo_->UpdateStatus(order_id, target_status, request_id);

    auto updated = order_repo_->FindById(order_id, request_id);
    return dto::OrderResponseDto::FromModel(*updated);
}

SellerStatsDto SellerService::GetSellerStats(int64_t seller_id, const std::string& request_id) {
    spdlog::info("[{}] SellerService::GetSellerStats seller_id={}", request_id, seller_id);
    auto products = product_repo_->FindBySellerId(seller_id, request_id);
    auto orders = order_repo_->FindBySellerId(seller_id, request_id);

    std::unordered_set<int64_t> seller_product_ids;
    for (const auto& p : products) {
        seller_product_ids.insert(p.id);
    }

    SellerStatsDto stats;
    stats.total_products = static_cast<int64_t>(products.size());
    stats.total_orders = static_cast<int64_t>(orders.size());

    int64_t rev_cents = 0;
    int64_t items_sold = 0;

    for (const auto& order : orders) {
        if (order.status != model::OrderStatus::kCancelled) {
            for (const auto& item : order.items) {
                if (seller_product_ids.count(item.product_id)) {
                    items_sold += item.quantity;
                    rev_cents += (item.unit_price * item.quantity).GetCents();
                }
            }
        }
    }

    stats.total_items_sold = items_sold;
    stats.total_revenue_cents = rev_cents;
    stats.total_revenue_formatted = model::Money::FromCents(rev_cents).ToString();

    return stats;
}

} // namespace dhivagar::dhivagarmart::service
