# Epoll File Transfer

A high-performance file transfer application using Linux epoll for handling multiple connections.

## Features
- Non-blocking I/O with epoll
- Multiple concurrent file transfers
- Client-server architecture

## Build
```bash
make
```

## Usage

### Server
```bash
./server
# Server listens on port 12345
```

### Client
```bash
./client <server_ip> <file_path>
# Example: ./client 127.0.0.1 test.txt
```

## Architecture
- Server uses epoll for handling multiple client connections
- Files are saved with client IP prefix (e.g., `192.168.1.100_file.dat`)
- Non-blocking I/O prevents server blocking on slow clients

## Known Issues
- Server doesn't properly handle partial reads in epoll loop
- Missing graceful shutdown mechanism
- Limited error recovery 