# Medulla
---
A lightweight, concurrent C++ networking library focused on security and flexibility.

## Features
 - [cite_start]**Concurrency**: Process-based scaling via `fork()` [cite: 34-36].
 - [cite_start]**Security**: Uses memory-aligned `SecureBuffer` with RAII-based `secureWipe` [cite: 45-56, 58-59].
 - [cite_start]**Protocol**: Reliable 4-byte length-prefixed messaging in Network Byte Order (`ntohl`/`htonl`) [cite: 17-21, 26].

## Quick Start
1. **Compile**: `g++ -std=c++17 src/main.cpp -I include -o server`
2. **Run**: `./server &`
3. **Test**: `python3 scripts/Client.py`

## Project Context
This library was developed to unify requirements for Computer Security and Computer Networks coursework. By decoupling the **Transport Layer** (TCP socket management) from the **Application Layer** (Command Execution/Cryptography), the core "Iron" remains generic and reusable.

## License
This library is free to use and edit for any purpose.