# Changelog

All notable changes to the **Dhivagar Mart** project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.1.0] - 2026-09-24
### Added
- Integrated AI Shopping Assistant (`/api/v1/ai/chat`) supporting Gemini and Mock providers.
- Per-session sliding rate limiting (10 requests/minute) and FAQ caching.
- Floating responsive chat widget with quick query chips.
- Final security regression checks and AddressSanitizer/UBSan validation.

## [1.0.0] - 2026-09-21
### Added
- Complete Admin dashboard for catalog moderation, user inspection, and revenue statistics.
- Full product review and 5-star rating system with verified-purchase enforcement.
- Product search by keyword and category filters.
- Production deployment configuration, Nginx reverse proxy template, and systemd unit.

## [0.2.0] - 2026-08-30
### Added
- Seller inventory management API and dashboard (`/api/v1/seller/products`).
- Multi-seller order management and status progression (`PENDING` -> `CONFIRMED` -> `SHIPPED` -> `DELIVERED`).
- Session-based authentication and role-based access control filters.

## [0.1.0] - 2026-08-10
### Added
- Initial MVP release.
- Core database schema, migration runner, and seed data.
- User registration and login with Argon2id password hashing.
- Product browsing catalog and cart management.
- Transactional checkout flow with mock payment confirmation and stock deduction.
