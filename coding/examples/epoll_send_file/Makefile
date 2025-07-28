CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
LDFLAGS = 

# Targets
TARGETS = server client

.PHONY: all clean

all: $(TARGETS)

server: server.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

client: client.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGETS) *.o *.dat

install: all
	cp $(TARGETS) /usr/local/bin/

.PHONY: test
test: all
	@echo "Starting server in background..."
	./server &
	@sleep 1
	@echo "Testing file transfer..."
	echo "Hello World" > test.txt
	./client 127.0.0.1 test.txt
	@sleep 1
	pkill server
	@echo "Test completed. Check for *_file.dat" 