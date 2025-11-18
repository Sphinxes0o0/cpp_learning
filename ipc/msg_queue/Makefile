CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
BINARIES = producer consumer
OBJECTS = producer.o consumer.o

all: $(BINARIES)

producer: producer.c msg_common.h
	$(CC) $(CFLAGS) -o producer producer.c

consumer: consumer.c msg_common.h
	$(CC) $(CFLAGS) -o consumer consumer.c

clean:
	rm -f $(BINARIES) $(OBJECTS)

.PHONY: all clean

# 运行示例
run-producer: producer
	./producer

run-consumer: consumer
	./consumer

# 创建并运行完整示例
run-example: all
	@echo "Starting consumer in background..."
	./consumer &
	@echo "Waiting 1 second..."
	sleep 1
	@echo "Starting producer..."
	./producer
	@echo "Example completed."