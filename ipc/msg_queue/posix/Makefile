CC  = cc
CFLAGS = -Wall -std=c99
LDFLAGS = -pthread   # 移除 -lrt 因为 macOS 不支持

all: producer consumer

producer: producer.c common.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

consumer: consumer.c common.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f producer consumer