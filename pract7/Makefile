CC = gcc
CFLAGS = -Wall -Wextra -g

TARGETS = memory_layout memory_demo

all: $(TARGETS)

memory_layout: memory_layout.c
	$(CC) $(CFLAGS) -o memory_layout memory_layout.c

memory_demo: memory_demo.c
	$(CC) $(CFLAGS) -o memory_demo memory_demo.c

clean:
	rm -f $(TARGETS)
