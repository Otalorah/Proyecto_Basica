# syntax=docker/dockerfile:1

FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
    g++-mingw-w64-x86-64 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

# Use this image as a compiler container with a mounted project folder.
# Static linking avoids runtime dependency on host MinGW DLL versions.
CMD ["x86_64-w64-mingw32-g++", "-std=c++17", "-O2", "-municode", "-mwindows", "-static", "-static-libgcc", "-static-libstdc++", "-o", "game.exe", "main.cpp", "files.cpp"]
