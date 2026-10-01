CC = gcc
CFLAGS = -Wall -Wextra

TARGETS = fifo_server fifo_client signal_handler

all: $(TARGETS)

fifo_server: fifo_server.c
	$(CC) $(CFLAGS) -o fifo_server fifo_server.c

fifo_client: fifo_client.c
	$(CC) $(CFLAGS) -o fifo_client fifo_client.c

signal_handler: signal_handler.c
	$(CC) $(CFLAGS) -o signal_handler signal_handler.c

clean:
	rm -f $(TARGETS)
