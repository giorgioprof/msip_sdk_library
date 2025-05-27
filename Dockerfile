# Stage 1: Build the base image with .so files
FROM debian:bookworm AS builder

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential cmake git curl libssl-dev pkg-config scons python3-pip python3-distro \
    libgsf-1-dev libsecret-1-dev freeglut3-dev libcpprest-dev libcurl4-openssl-dev uuid-dev

# Set up working directory
WORKDIR /app

# Copy your source code and CMakeLists.txt
COPY . .

# Build the project
WORKDIR /app/sdk_file/msip_file
RUN scons

# Stage 2: Create the final Python image
FROM python:3.12-slim

# Install minimal system dependencies
RUN apt-get update && apt-get install -y \
    wget curl gnupg lsb-release procps \
    libssl3 libsecret-1-0 libcurl4 libgsf-1-114 libcpprest \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Create directories
RUN mkdir -p /app/lib

COPY --from=builder /app/sdk_file/bins/release/x86_64/*.so /app/lib/


# Install Python dependencies efficiently
WORKDIR /app
COPY --from=ghcr.io/astral-sh/uv:0.5.20 /uv /uvx /bin/
COPY requirements.txt .
RUN --mount=type=cache,target=/root/.cache/pip \
    uv pip install -r requirements.txt --system

# Copy application code
COPY ./app /app/app
COPY pytest.ini /app/

# Environment variables
ENV PYTHONPATH=/app/:/app/app:$PYTHONPATH
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/app/lib

# Set the entrypoint
ENTRYPOINT ["python", "/app/app/entrypoint.py"]