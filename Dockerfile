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
# Includes uuid-dev and libuuid1 required by Drogon FindUUID.cmake
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

# Create unofficial-sodium CMake wrapper so CMake finds system libsodium
RUN mkdir -p /usr/local/lib/cmake/unofficial-sodium && \
    printf 'add_library(unofficial-sodium::sodium UNKNOWN IMPORTED)\nset_target_properties(unofficial-sodium::sodium PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libsodium.so INTERFACE_INCLUDE_DIRECTORIES /usr/include)\n' > /usr/local/lib/cmake/unofficial-sodium/unofficial-sodiumConfig.cmake

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
    libsodium23 \
    libspdlog1.12 \
    ca-certificates \
    curl \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy compiled executable and assets
COPY --from=builder /workspace/build/DhivagarMart /app/DhivagarMart
COPY --from=builder /workspace/frontend /app/frontend
COPY --from=builder /workspace/db /app/db
COPY --from=builder /workspace/.env.example /app/.env.example

EXPOSE 8080

# Run Dhivagar Mart production binary
ENTRYPOINT ["/app/DhivagarMart"]
