CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
BINARIES = producer consumer producer_multi consumer_multi
OBJECTS = producer.o consumer.o producer_multi.o consumer_multi.o

all: $(BINARIES)

producer: producer.c msg_common.h
	$(CC) $(CFLAGS) -o producer producer.c

consumer: consumer.c msg_common.h
	$(CC) $(CFLAGS) -o consumer consumer.c

producer_multi: producer_multi.c msg_common.h
	$(CC) $(CFLAGS) -o producer_multi producer_multi.c

consumer_multi: consumer_multi.c msg_common.h
	$(CC) $(CFLAGS) -o consumer_multi consumer_multi.c

clean:
	rm -f $(BINARIES) $(OBJECTS)

.PHONY: all clean

# 运行示例
run-producer: producer
	./producer

run-consumer: consumer
	./consumer

run-producer-multi: producer_multi
	./producer_multi

run-consumer-multi: consumer_multi
	./consumer_multi

# 创建并运行完整示例
run-example: all
	@echo "Starting consumer in background..."
	./consumer &
	@echo "Waiting 1 second..."
	sleep 1
	@echo "Starting producer..."
	./producer
	@echo "Example completed."

# 多生产者多消费者示例
run-multi-example: all
	@echo "Starting multiple consumers in background..."
	./consumer_multi 1 &
	./consumer_multi 2 &
	@echo "Waiting 1 second..."
	sleep 1
	@echo "Starting multiple producers..."
	./producer_multi 1 &
	./producer_multi 2 &
	@echo "Multi-producer/multi-consumer example started."
	@echo "Run 'killall producer_multi consumer_multi' to stop processes if needed."