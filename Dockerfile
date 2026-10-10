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
# Includes uuid-dev and libuuid1 for Drogon, plus libpq-dev, libpqxx-dev, libsodium-dev, libcpp-httplib-dev, libssl-dev
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
    libfmt-dev \
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
    && cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF -DCMAKE_INSTALL_LIBDIR=lib .. \
    && make -j2 \
    && make install \
    && ldconfig \
    && rm -rf /tmp/drogon

# Build Dhivagar Mart C++20 application
WORKDIR /workspace
COPY . .

RUN rm -rf build && mkdir build && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=20 -DENABLE_TESTS=OFF .. \
    && make -j2

# ------------------------------------------------------------
# Stage 2: Runtime Stage (Minimal base with runtime libraries)
# ------------------------------------------------------------
FROM ubuntu:24.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive
ENV APP_PORT=8080
ENV PORT=8080

RUN apt-get update && apt-get install -y --no-install-recommends \
    libuuid1 \
    libjsoncpp-dev \
    libssl-dev \
    zlib1g \
    libpq5 \
    libpqxx-dev \
    libsodium23 \
    libspdlog-dev \
    libfmt-dev \
    libcpp-httplib-dev \
    ca-certificates \
    curl \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy compiled Drogon/Trantor shared libraries from builder
COPY --from=builder /usr/local/lib/ /usr/local/lib/
RUN ldconfig

# Copy compiled executable and assets
COPY --from=builder /workspace/build/DhivagarMart /app/DhivagarMart
COPY --from=builder /workspace/frontend /app/frontend
COPY --from=builder /workspace/db /app/db
COPY --from=builder /workspace/.env.example /app/.env.example
RUN mkdir -p /app/logs && chmod +x /app/DhivagarMart

EXPOSE 8080

# Run Dhivagar Mart production binary
ENTRYPOINT ["/app/DhivagarMart"]
