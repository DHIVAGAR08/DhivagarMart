# Contributing to Dhivagar Mart

Welcome to **🛒 Dhivagar Mart**! This project is a native C++20 marketplace built with the Drogon web framework and PostgreSQL.

---

## 1. Prerequisites

- **C++ Compiler**: GCC 12+ / Clang 15+ / MSVC 2022 (supporting C++20)
- **CMake**: >= 3.25
- **Package Manager**: [vcpkg](https://github.com/microsoft/vcpkg)
- **Database**: PostgreSQL 15+ (PostgreSQL 18 included and configured)
- **Build Tool**: Ninja or GNU Make

---

## 2. Environment Setup

### Clone Repository
```bash
git clone https://github.com/dhivagar/dhivagarmart.git
cd dhivagarmart
```

### Install Dependencies via vcpkg
Ensure `VCPKG_ROOT` is set in your environment:
```bash
# On Windows / Linux:
vcpkg install --triplet x64-mingw-dynamic
```

### Configure Environment File
Copy the example environment configuration:
```bash
cp .env.example .env
```
Edit `.env` to ensure your database connection parameters match your local PostgreSQL server:
```ini
DATABASE_HOST=127.0.0.1
DATABASE_PORT=5433
DATABASE_NAME=dhivagarmart
DATABASE_USER=postgres
DATABASE_PASSWORD=
APP_PORT=8080
```

---

## 3. Build Instructions

### Configure CMake
```bash
cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
```

### Build Executable & Tests
```bash
cmake --build build --config Release
```

---

## 4. Database Setup & Migrations

The application features a built-in migration service (`MigrationService`) that automatically verifies and applies all numbered SQL migrations from `db/migrations/` upon startup.

To manually seed demo data:
```bash
psql -h 127.0.0.1 -p 5433 -U postgres -d dhivagarmart -f db/seed.sql
```

---

## 5. Running the Application

```bash
# From repository root:
./build/DhivagarMart
```
Open your browser at: `http://localhost:8080`

---

## 6. Running Tests

```bash
ctest --test-dir build --output-on-failure
```

---

## 7. Coding Standards & Git Workflow

- Google C++ Style Guide:
  - Types: `PascalCase`
  - Functions: `PascalCase`
  - Member Variables: `snake_case_`
  - Constants: `kCamelCase`
  - Namespaces: `dhivagar::dhivagarmart`
- All SQL statements MUST use parameterized queries via `libpqxx::exec_params` or prepared statements. Never interpolate strings into SQL.
- Passwords must be hashed using Argon2id via libsodium.
- Use RAII everywhere with smart pointers (`std::unique_ptr` / `std::shared_ptr`).
