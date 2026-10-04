CC = gcc
CFLAGS = -Wall -Wextra -O2

PROGRAMS = copy_lowlevel copy_stdio redirect_output redirect_input

all: $(PROGRAMS)

copy_lowlevel: copy_lowlevel.c
	$(CC) $(CFLAGS) copy_lowlevel.c -o copy_lowlevel

copy_stdio: copy_stdio.c
	$(CC) $(CFLAGS) copy_stdio.c -o copy_stdio

redirect_output: redirect_output.c
	$(CC) $(CFLAGS) redirect_output.c -o redirect_output

redirect_input: redirect_input.c
	$(CC) $(CFLAGS) redirect_input.c -o redirect_input

clean:
	rm -f $(PROGRAMS)
