# syntax=docker/dockerfile:1
# ============================================================
# DHIVAGAR MART — PRODUCTION MULTI-STAGE DOCKERFILE
# Conforms to Capstone Specification Section 8 (Deployment Specification)
# ============================================================

# ------------------------------------------------------------
# Stage 1: Build Stage (Full C++20 toolchain & dependencies)
# ------------------------------------------------------------
FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC

# Install system compiler, cmake, build tools and development libraries
# Includes uuid-dev and libuuid1 for Drogon, plus libpq-dev, libpqxx-dev, libcpp-httplib-dev
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    git \
    pkg-config \
    uuid-dev \
    libuuid1 \
    libjsoncpp-dev \
    libssl-dev \
    zlib1g-dev \
    libpq-dev \
    libpqxx-dev \
    libsodium-dev \
    libspdlog-dev \
    nlohmann-json3-dev \
    libcpp-httplib-dev \
    ca-certificates \
    curl \
    tar \
    unzip \
    zip \
    && rm -rf /var/lib/apt/lists/*

# Install Drogon web framework from release
WORKDIR /tmp
RUN git clone --depth 1 --branch v1.9.4 https://github.com/drogonframework/drogon.git \
    && cd drogon \
    && git submodule update --init \
    && mkdir build && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF .. \
    && make -j$(nproc) \
    && make install \
    && ldconfig \
    && rm -rf /tmp/drogon

# Create libpqxx, unofficial-sodium, and httplib CMake wrappers so find_package CONFIG succeeds on Linux
RUN mkdir -p /usr/local/lib/cmake/libpqxx /usr/lib/x86_64-linux-gnu/cmake/libpqxx /usr/share/cmake/libpqxx && \
    printf 'find_library(LIBPQXX_LIB NAMES pqxx libpqxx REQUIRED)\nfind_library(LIBPQ_LIB NAMES pq libpq REQUIRED)\nfind_path(LIBPQXX_INCLUDE_DIR pqxx/pqxx REQUIRED)\nif(NOT TARGET libpqxx::pqxx)\n    add_library(libpqxx::pqxx UNKNOWN IMPORTED)\n    set_target_properties(libpqxx::pqxx PROPERTIES IMPORTED_LOCATION "${LIBPQXX_LIB}" INTERFACE_INCLUDE_DIRECTORIES "${LIBPQXX_INCLUDE_DIR}" INTERFACE_LINK_LIBRARIES "${LIBPQ_LIB}")\nendif()\nset(libpqxx_FOUND TRUE)\nset(LIBPQXX_FOUND TRUE)\n' > /usr/local/lib/cmake/libpqxx/libpqxxConfig.cmake && \
    cp /usr/local/lib/cmake/libpqxx/libpqxxConfig.cmake /usr/local/lib/cmake/libpqxx/libpqxx-config.cmake && \
    cp /usr/local/lib/cmake/libpqxx/libpqxxConfig.cmake /usr/lib/x86_64-linux-gnu/cmake/libpqxx/libpqxxConfig.cmake && \
    cp /usr/local/lib/cmake/libpqxx/libpqxxConfig.cmake /usr/share/cmake/libpqxx/libpqxxConfig.cmake && \
    mkdir -p /usr/local/lib/cmake/unofficial-sodium /usr/lib/x86_64-linux-gnu/cmake/unofficial-sodium /usr/share/cmake/unofficial-sodium && \
    printf 'find_library(SODIUM_LIB NAMES sodium REQUIRED)\nfind_path(SODIUM_INCLUDE_DIR sodium.h REQUIRED)\nif(NOT TARGET unofficial-sodium::sodium)\n    add_library(unofficial-sodium::sodium UNKNOWN IMPORTED)\n    set_target_properties(unofficial-sodium::sodium PROPERTIES IMPORTED_LOCATION "${SODIUM_LIB}" INTERFACE_INCLUDE_DIRECTORIES "${SODIUM_INCLUDE_DIR}")\nendif()\nset(unofficial-sodium_FOUND TRUE)\n' > /usr/local/lib/cmake/unofficial-sodium/unofficial-sodiumConfig.cmake && \
    cp /usr/local/lib/cmake/unofficial-sodium/unofficial-sodiumConfig.cmake /usr/local/lib/cmake/unofficial-sodium/unofficial-sodium-config.cmake && \
    mkdir -p /usr/local/lib/cmake/httplib /usr/lib/x86_64-linux-gnu/cmake/httplib /usr/share/cmake/httplib && \
    printf 'find_path(HTTPLIB_INCLUDE_DIR httplib.h PATHS /usr/include /usr/local/include REQUIRED)\nfind_package(OpenSSL REQUIRED)\nfind_package(Threads REQUIRED)\nif(NOT TARGET httplib::httplib)\n    add_library(httplib::httplib INTERFACE IMPORTED)\n    set_target_properties(httplib::httplib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${HTTPLIB_INCLUDE_DIR}" INTERFACE_COMPILE_DEFINITIONS "CPPHTTPLIB_OPENSSL_SUPPORT" INTERFACE_LINK_LIBRARIES "OpenSSL::SSL;OpenSSL::Crypto;Threads::Threads")\nendif()\nset(httplib_FOUND TRUE)\nset(HTTPLIB_FOUND TRUE)\n' > /usr/local/lib/cmake/httplib/httplibConfig.cmake && \
    cp /usr/local/lib/cmake/httplib/httplibConfig.cmake /usr/local/lib/cmake/httplib/httplib-config.cmake && \
    cp /usr/local/lib/cmake/httplib/httplibConfig.cmake /usr/lib/x86_64-linux-gnu/cmake/httplib/httplibConfig.cmake && \
    cp /usr/local/lib/cmake/httplib/httplibConfig.cmake /usr/share/cmake/httplib/httplibConfig.cmake

# Build Dhivagar Mart C++20 application
WORKDIR /workspace
COPY . .

RUN rm -rf build && mkdir build && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=20 -DENABLE_TESTS=OFF .. \
    && make -j$(nproc)

# ------------------------------------------------------------
# Stage 2: Runtime Stage (Minimal base with runtime libraries)
# ------------------------------------------------------------
FROM ubuntu:24.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive
ENV APP_PORT=8080
ENV PORT=8080

RUN apt-get update && apt-get install -y --no-install-recommends \
    libuuid1 \
    libjsoncpp25 \
    libssl3 \
    zlib1g \
    libpq5 \
    libpqxx-dev \
    libsodium23 \
    libspdlog1.12 \
    ca-certificates \
    curl \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy compiled Drogon/Trantor shared libraries from builder
COPY --from=builder /usr/local/lib/libdrogon* /usr/local/lib/
COPY --from=builder /usr/local/lib/libtrantor* /usr/local/lib/
RUN ldconfig

# Copy compiled executable and assets
COPY --from=builder /workspace/build/DhivagarMart /app/DhivagarMart
COPY --from=builder /workspace/frontend /app/frontend
COPY --from=builder /workspace/db /app/db
COPY --from=builder /workspace/.env.example /app/.env.example

EXPOSE 8080

# Run Dhivagar Mart production binary
ENTRYPOINT ["/app/DhivagarMart"]
