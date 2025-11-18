CC = cc
CFLAGS = -Wall -Wextra -std=c17

all: shm_r shm_w

shm_r: shm_r.c
	$(CC) $(CFLAGS) -o shm_r shm_r.c

shm_w: shm_w.c
	$(CC) $(CFLAGS) -o shm_w shm_w.c

clean:
	rm -f shm_r shm_w