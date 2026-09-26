# 🛒 Dhivagar Mart - Native C++20 Electronics Marketplace

A high-performance, secure, and modern electronics & technology e-commerce marketplace web application built natively in **C++20** using the **Drogon** web framework, **PostgreSQL** (via `libpqxx`), and modern HTML5/CSS3/vanilla JavaScript frontend.

Strictly engineered in compliance with the **R2025_Sem3_C++_CapstoneProject** specification.

---

## 1. Project Overview
**Dhivagar Mart** is a native C++ multi-seller electronics and consumer technology marketplace designed for India (with native Indian Rupee `₹` pricing). It provides role-based access for Buyers, Sellers, and Administrators. Buyers can search, filter, add hardware to cart and wishlist, place orders through an atomic ACID transaction with Cash on Delivery (COD) or mock payment, track orders, and submit verified reviews. Sellers manage inventory and fulfill customer orders. Administrators have full moderation control over listings, user accounts, and financial metrics. The platform also features an intelligent server-side AI Shopping Assistant.

---

## 2. Features
- **Authentication & Roles**: Secure Argon2id password hashing via libsodium; Drogon server-side session management with automatic pre-auth session invalidation. Roles: `BUYER`, `SELLER`, `ADMIN`.
- **Product Catalog**: Multi-category electronics catalog (Mobiles, Laptops, Electrical, Audio, Gaming, Cameras, Smart Watches, Accessories) with full-text keyword search, real-time category filters, and authentic Indian Rupee (`₹`) pricing.
- **Shopping Cart & Wishlist**: Database-backed cart and wishlist (`cart_items`, `wishlist_items`), real-time stock validation, wishlist-to-cart migration, and running subtotal calculation using an immutable `Money` value type.
- **ACID Checkout & Payment**: Single-transaction atomic checkout that verifies inventory, supports Cash on Delivery (COD), Mock UPI, and Mock Card, creates order items, reduces stock, and clears the cart—with 100% rollback guarantee on any failure.
- **Verified Product Reviews**: 1 to 5 star ratings and reviews restricted strictly to verified buyers who have completed an order containing the product.
- **Seller Portal**: Dedicated dashboard for sellers to manage inventory (CRUD), view incoming customer orders, and update fulfillment status (`CONFIRMED` → `SHIPPED` → `DELIVERED`).
- **Admin Control Center**: System metrics dashboard (users, products, orders, gross revenue), user directory, global order monitoring, and catalog moderation.
- **AI Shopping Assistant**: Server-side AI assistant using the Strategy pattern (`GeminiChatProvider` and `MockChatProvider`), equipped with sliding rate limiting (10 msg/min), domain-restricted prompt guardrails, in-memory question caching, and timeout fallbacks.
- **Security & Integrity**: 100% parameterized SQL queries via `libpqxx` (no string interpolation), HTML output escaping, and masked error messages that prevent leaking database internals or stack traces.

---

## 3. Architecture
The system strictly enforces the **Separation of Concerns** and **Dependency Inversion** principles:

```
Browser (HTML5, CSS3, Vanilla JS + fetch API)
  │
  ▼ HTTP Request
Filter Layer (LoggingFilter [request_id] ──► CorsFilter ──► AuthFilter)
  │
  ▼ Drogon Built-in Router
Controller Layer (Thin HTTP orchestration, JSON serialization, HTTP status codes)
  │
  ▼ Dependency Injection (Constructor Injection)
Service Layer (Business rules, Money arithmetic, validation, transaction boundaries)
  │
  ▼ Abstract Interfaces (IUserRepository, IProductRepository, etc.)
Repository Layer (ALL SQL lives here, strictly parameterized via libpqxx)
  │
  ▼ Connection Pool
DatabasePlugin (Singleton pool, RAII acquisition, startup verification SELECT 1)
  │
  ▼
PostgreSQL 15+ (dhivagarmart)
```

---

## 4. Folder Structure
```
dhivagarmart/
├── CMakeLists.txt              # Root CMake build configuration
├── vcpkg.json                  # Manifest listing all C++ dependencies
├── README.md                   # Comprehensive documentation
├── CONTRIBUTING.md             # Developer contribution guide
├── CHANGELOG.md                # Semantic version release notes
├── RETRO.md                    # Sprint retrospectives
├── .gitignore                  # Git ignore rules
├── .env.example                # Sample environment configuration
│
├── src/
│   ├── main.cpp                # Application entrypoint & server bootstrap
│   ├── controller/             # Drogon HTTP controllers (thin layer)
│   ├── service/                # Domain logic, validation & transactions
│   ├── repository/             # Abstract interfaces & libpqxx SQL implementations
│   ├── model/                  # Domain models (User, Product, Order, Money)
│   ├── dto/                    # Request and response data transfer objects
│   ├── filter/                 # Drogon filters (Auth, Logging, CORS)
│   ├── plugin/                 # DatabasePlugin connection pool & MigrationPlugin
│   ├── util/                   # PasswordUtil, ValidationUtil, JsonUtil, Environment
│   └── exception/              # AppException hierarchy & GlobalExceptionHandler
│
├── db/
│   ├── schema.sql              # Complete baseline PostgreSQL schema
│   ├── seed.sql                # Seed data for default platform accounts & electronics catalog
│   └── migrations/             # Numbered transactional migrations (V1 through V6)
│
├── frontend/                   # Responsive web application (HTML5/CSS3/Vanilla JS)
│   ├── index.html              # Marketplace home & featured catalog
│   ├── products.html           # Full electronics catalog with search & filters
│   ├── product-details.html    # Product specs, verified reviews & ratings
│   ├── cart.html               # Shopping cart & running totals
│   ├── checkout.html           # Delivery address & payment method selection
│   ├── wishlist.html           # Saved items and move-to-cart workflow
│   ├── login.html              # Sign in with registered credentials
│   ├── register.html           # Buyer and seller account registration
│   ├── orders.html             # Customer order history & tracking invoices
│   ├── seller.html             # Seller inventory & fulfillment dashboard
│   ├── admin.html              # Admin statistics, user directory & moderation
│   ├── css/style.css           # Modern technology aesthetic stylesheet
│   └── js/                     # Modular API client, auth, and view controllers
│
└── test/
    ├── unit/                   # Money, Validation, and Service unit tests (GMock)
    ├── repository/             # Live PostgreSQL integration repository tests
    └── api/                    # End-to-end API scenario tests
```

---

## 5. Required Software
- **C++ Compiler**: GCC 12+, Clang 15+, or MSVC 2022 supporting ISO C++20
- **CMake**: >= 3.25
- **Build System**: Ninja or GNU Make
- **Package Manager**: [vcpkg](https://github.com/microsoft/vcpkg)
- **Database**: PostgreSQL 15 or higher (PostgreSQL 18 tested)

---

## 6. vcpkg Setup
Clone and bootstrap vcpkg if not already installed:
```bash
git clone https://github.com/microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh       # On Linux/macOS
.\vcpkg\bootstrap-vcpkg.bat      # On Windows
```
Set the `VCPKG_ROOT` environment variable:
```powershell
$env:VCPKG_ROOT = "C:\path\to\vcpkg"
```

---

## 7. PostgreSQL Installation (Windows & Linux)
### Windows (pgAdmin / Standalone)
1. Download PostgreSQL 15+ from the official site: https://www.postgresql.org/download/windows/
2. Run the installer and note your superuser password (or keep default port `5432` / `5433`).
3. Add PostgreSQL binary folder to PATH:
   ```powershell
   $env:Path = "C:\Program Files\PostgreSQL\18\bin;$env:Path"
   ```

### Linux (Ubuntu / Debian)
```bash
sudo apt update
sudo apt install postgresql postgresql-contrib
sudo systemctl enable --now postgresql
```

---

## 8. PostgreSQL Database Creation
Run `psql` to create the project database:
```bash
psql -U postgres -h localhost -p 5432 -c "CREATE DATABASE dhivagarmart;"
```

---

## 9. Environment Setup
Copy the sample environment file:
```bash
cp .env.example .env
```
Configure your credentials in `.env`:
```ini
DATABASE_HOST=localhost
DATABASE_PORT=5432
DATABASE_NAME=dhivagarmart
DATABASE_USER=postgres
DATABASE_PASSWORD=your_password_here

APP_HOST=0.0.0.0
APP_PORT=8080
SESSION_IDLE_TIMEOUT_SEC=1800

AI_CHATBOT_PROVIDER=mock
AI_API_KEY=your_gemini_api_key_here
```

---

## 10. Migration Setup
Dhivagar Mart includes an automatic `MigrationService`. On every application startup, `MigrationPlugin` checks `db/migrations/` and safely executes any pending migration files inside an atomic transaction:
- `V1__init_schema.sql`: Core schema (users, products, orders, items, cart)
- `V2__add_reviews_table.sql`: Verified customer reviews table
- `V3__add_indexes.sql`: Performance indexes on all foreign keys and search columns
- `V4__update_electronics_catalog.sql`: Electronics marketplace catalog upgrade
- `V5__add_order_details_and_wishlist.sql`: Order contact/address attributes and wishlist support
- `V6__realistic_electronics_pricing.sql`: Realistic Indian Rupee market pricing

You can also apply them manually via `psql`:
```bash
psql -U postgres -h localhost -p 5432 -d dhivagarmart -f db/migrations/V1__init_schema.sql
psql -U postgres -h localhost -p 5432 -d dhivagarmart -f db/migrations/V2__add_reviews_table.sql
psql -U postgres -h localhost -p 5432 -d dhivagarmart -f db/migrations/V3__add_indexes.sql
psql -U postgres -h localhost -p 5432 -d dhivagarmart -f db/migrations/V4__update_electronics_catalog.sql
psql -U postgres -h localhost -p 5432 -d dhivagarmart -f db/migrations/V5__add_order_details_and_wishlist.sql
psql -U postgres -h localhost -p 5432 -d dhivagarmart -f db/migrations/V6__realistic_electronics_pricing.sql
```

---

## 11. Seed Setup
To populate the database with default administrative accounts and verified electronics products:
```bash
psql -U postgres -h localhost -p 5432 -d dhivagarmart -f db/seed.sql
```

---

## 12. Build Instructions
Configure CMake using the vcpkg toolchain:
```bash
cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
```
Compile the application:
```bash
cmake --build build --config Release
```

---

## 13. Running the Server
Launch the compiled standalone executable:
```bash
./build/DhivagarMart
```
Upon startup, the console displays:
```
============================================================
           🛒 DHIVAGAR MART - NATIVE C++20 BACKEND
============================================================
  Web Framework:       Drogon HTTP Server
  Language Standard:   ISO C++20
  Database:            PostgreSQL 15+ (libpqxx)
  Security Engine:     libsodium Argon2id
  Listening Address:   http://0.0.0.0:8080
  Document Root:       ./frontend
============================================================
PostgreSQL connected successfully
```

---

## 14. Frontend Access
Open your web browser at:
```
http://localhost:8080
```
The responsive UI supports desktop and mobile viewports with no external framework dependencies.

---

## 15. API Endpoints List

### Health
- `GET /api/v1/health` - Liveness probe checking `SELECT 1`

### Authentication
- `POST /api/v1/auth/register` - Create Buyer or Seller account
- `POST /api/v1/auth/login` - Authenticate and establish fresh session
- `POST /api/v1/auth/logout` - Invalidate session
- `GET  /api/v1/auth/me` - Current session profile

### Products Catalog
- `GET /api/v1/products` - Filter by `category` and `search` query
- `GET /api/v1/products/{id}` - Single product details

### Shopping Cart
- `GET    /api/v1/cart` - View items and running total
- `POST   /api/v1/cart` - Add or increment item
- `PUT    /api/v1/cart/{productId}` - Modify quantity
- `DELETE /api/v1/cart/{productId}` - Remove item

### Orders & Checkout
- `POST /api/v1/orders` - Atomic checkout transaction
- `GET  /api/v1/orders` - Buyer's order history
- `GET  /api/v1/orders/{id}` - Specific order details

### Wishlist
- `GET    /api/v1/wishlist` - View customer wishlist
- `POST   /api/v1/wishlist` - Save product to wishlist
- `DELETE /api/v1/wishlist/{productId}` - Remove product from wishlist
- `POST   /api/v1/wishlist/{productId}/move-to-cart` - Transfer saved item directly into cart

### Reviews & Ratings
- `GET  /api/v1/products/{id}/reviews` - List product reviews
- `POST /api/v1/products/{id}/reviews` - Post 1–5 star rating (verified buyers only)

### Seller Portal
- `GET    /api/v1/seller/products` - Own product listings
- `POST   /api/v1/seller/products` - Create new listing
- `PUT    /api/v1/seller/products/{id}` - Update own listing
- `DELETE /api/v1/seller/products/{id}` - Delete own listing
- `GET    /api/v1/seller/orders` - Incoming store orders
- `PUT    /api/v1/seller/orders/{id}/status` - Advance order status

### Admin Center
- `GET    /api/v1/admin/users` - User directory
- `GET    /api/v1/admin/products` - Global product listings
- `DELETE /api/v1/admin/products/{id}` - Moderate/remove listing
- `GET    /api/v1/admin/orders` - All marketplace orders
- `PUT    /api/v1/admin/orders/{id}/status` - Override order status
- `GET    /api/v1/admin/stats` - Marketplace financial and user analytics

### AI Chatbot
- `POST /api/v1/ai/chat` - Query shopping assistant (`{ "message": "..." }`)

---

## 16. Test Commands
Run the automated test suite covering unit, repository, and API integration:
```bash
ctest --test-dir build --output-on-failure
```
Or execute the test binary directly:
```bash
./build/DhivagarMartTests
```

---

## 17. Seeded Platform Accounts (Evaluation & Viva Defense)
| Role | Email | Password | Permissions |
| :--- | :--- | :--- | :--- |
| **Admin** | `admin@dhivagarmart.com` | `Admin@123` | Full governance, moderation, user management, telemetry |
| **Seller** | `seller@dhivagarmart.com` | `Seller@123` | Manage own products, fulfill customer orders |
| **Buyer** | `buyer@dhivagarmart.com` | `Buyer@123` | Browse catalog, add to cart, checkout, write reviews |

---

## 18. AI Chatbot Setup
To use Google Gemini LLM:
1. Obtain an API key from Google AI Studio.
2. In `.env`, set:
   ```ini
   AI_CHATBOT_PROVIDER=gemini
   AI_API_KEY=AIzaSy...
   ```
3. Restart the server. The chatbot will automatically query Gemini. If the key is empty or outbound network is unreachable, it automatically degrades gracefully to the built-in `MockChatProvider`.

---

## 19. Production Deployment

### Linux systemd Service
Create `/etc/systemd/system/dhivagarmart.service`:
```ini
[Unit]
Description=Dhivagar Mart Native C++ Service
After=network.target postgresql.service

[Service]
Type=simple
User=www-data
WorkingDirectory=/opt/dhivagarmart
ExecStart=/opt/dhivagarmart/DhivagarMart
Restart=always
RestartSec=5

[Install]
WantedBy=multi-user.target
```
Enable and start:
```bash
sudo systemctl daemon-reload
sudo systemctl enable --now dhivagarmart
```

### Nginx Reverse Proxy with TLS
```nginx
server {
    listen 80;
    server_name mart.example.com;
    return 301 https://$host$request_uri;
}

server {
    listen 443 ssl http2;
    server_name mart.example.com;

    ssl_certificate /etc/letsencrypt/live/mart.example.com/fullchain.pem;
    ssl_certificate_key /etc/letsencrypt/live/mart.example.com/privkey.pem;

    location / {
        proxy_pass http://127.0.0.1:8080;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto $scheme;
    }
}
```

---

## 20. Troubleshooting
- **Database Connection Refused**:
  - Verify PostgreSQL is running: `Get-Service *postgres*` on Windows or `systemctl status postgresql` on Linux.
  - Verify port in `.env` matches your database port (`5432` or `5433`).
- **401 Unauthenticated Error**:
  - Ensure cookies are allowed in the browser. Drogon sessions rely on the `JSESSIONID` cookie.
- **Out of Memory during Build**:
  - Run with fewer parallel jobs: `ninja -j2` or `cmake --build build --parallel 2`.
