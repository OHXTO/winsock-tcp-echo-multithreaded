# Winsock TCP Echo (Multithreaded)
A minimal TCP client/server demo in C on Windows using Winsock.

The server listens on port `8080` and echoes back whatever each client sends.
Supported multiple client connections.

<img src="assets/demo.png" alt="demo" width="420">

## Features
- server: `bind` / `listen` / `accept` / `recv` / `send`
- Multi-client support via `CreateThread` (one thread per client connection)
- client that connects to `127.0.0.1:8080` and sends/echoback user input.

## Requirements
- Windows
- MinGW-w64 / gcc
- Winsock2 (`-lws2_32`)

## Build
```bash
gcc server.c -o server.exe -lws2_32
gcc client.c -o client.exe -lws2_32
```
> **NOTE:** With MSVC (Visual Studio), you can auto-link Winsock using
> ```
> #pragma comment(lib, "ws2_32.lib")
> ```
> With MinGW-w64 `gcc`(commonly used in VS Code), link it explicitly using `-lws2_32`.

## Run
1. Start the server:
```bash
./server.exe
```
> **Note (Windows Firewall):** When you run the server for the first time, Windows may prompt a firewall/security dialog.
> For local testing, allowing **Private networks**. For server reachable on public Wi-Fi, allowing **Public networks**.

2. In another terminal, start one or more clients:
```bash
./client.exe
```
3. Type messages in the client; the server will echo them back.


- This is a learning/demo project and does not implement message framing (TCP is a byte-stream; `recv` boundaries do not equal message boundaries).
- `send` may send partial bytes in real-world scenarios; production code should loop until all bytes are sent.
