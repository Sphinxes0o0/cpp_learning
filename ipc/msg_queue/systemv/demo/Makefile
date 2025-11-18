CC  = cc
CFLAGS = -Wall -std=c99

all: producer consumer

producer: producer.c msg_common.h
	$(CC) $(CFLAGS) -o $@ $<

consumer: consumer.c msg_common.h
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f producer consumer