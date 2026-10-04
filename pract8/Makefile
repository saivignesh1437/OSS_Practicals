CC = gcc
CFLAGS = -Wall -Wextra -g

TARGETS = dynamic_memory cow_demo

all: $(TARGETS)

dynamic_memory: dynamic_memory.c
	$(CC) $(CFLAGS) -o dynamic_memory dynamic_memory.c

cow_demo: cow_demo.c
	$(CC) $(CFLAGS) -o cow_demo cow_demo.c

clean:
	rm -f $(TARGETS)
