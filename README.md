# Simple Multi-Client Chat Server in C++
This is a basic multi-client chat server written in C++ using POSIX sockets and threads. It allows multiple clients to connect, set their usernames, and exchange messages in real-time. The server broadcasts each client's message to all other connected clients.

I mainly made this to expand my knowledge on network programming. POSIX sockets and mulithreading.

# Features
Multi-client support with thread-per-client model
Broadcast messages to all connected clients except the sender
Handles client disconnections gracefully
Thread-safe client list management with mutex lockin

#How to compile
For server run
```bash
g++ -pthread -o server server.cpp
```
Then
```bash
./server
```

For client run
```bash
g++ -std=c++11 -o client client.cpp
```
Then
```bash
./client
```



