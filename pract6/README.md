# Practical 6 – Client–Server Application Using Named Pipes (FIFOs) and POSIX Signal Handling

## Objective

This practical demonstrates:

- Client–Server communication using Named Pipes (FIFOs)
- Communication between multiple clients and a server
- Separate response FIFO for each client
- POSIX signal handling using `sigaction()`
- Handling of `SIGINT`, `SIGTERM`, and `SIGUSR1`
- Graceful termination of a process

---

# Part 1 – Client–Server Application Using Named Pipes (FIFOs)

## Description

A client–server application is implemented using POSIX Named Pipes (FIFOs).

The server creates a common FIFO through which clients send requests.

Each client creates its own response FIFO. The server processes the request and sends the response through that client's FIFO.

This allows multiple clients to communicate with the same server.

## Files

- `fifo_server.c` – Server program
- `fifo_client.c` – Client program
- `signal_handler.c` – POSIX signal handling program
- `Makefile` – Compilation and cleanup
- `.gitignore` – Ignores generated executable files

---

# Server Program

The server:

1. Creates the common server FIFO.
2. Waits for client requests.
3. Reads the client PID and message.
4. Identifies the client's response FIFO.
5. Processes the message.
6. Sends the response to the client.
7. Continues waiting for more clients.

The server FIFO used is:

```text
/tmp/server_fifo
```

Each client's response FIFO follows this format:

```text
/tmp/client_<PID>_fifo
```

---

# Client Program

The client:

1. Creates its own response FIFO.
2. Reads a message from the user.
3. Sends the message and its PID to the server FIFO.
4. Waits for the server response.
5. Displays the response.
6. Removes its response FIFO after communication.

---

# Compilation

The programs can be compiled individually using:

```bash
gcc -Wall -Wextra -o fifo_server fifo_server.c
gcc -Wall -Wextra -o fifo_client fifo_client.c
gcc -Wall -Wextra -o signal_handler signal_handler.c
```

Or all programs can be compiled using the Makefile:

```bash
make
```

---

# Execution

## Terminal 1 – Start the Server

```bash
./fifo_server
```

Example:

```text
Server started...
Waiting for clients...
```

## Terminal 2 – Start Client 1

```bash
./fifo_client
```

Example:

```text
Client PID: 27171
Enter message: Hello Server
Message sent to server.
Server Response: Server processed: Hello Server
```

## Terminal 3 – Start Client 2

```bash
./fifo_client
```

Example:

```text
Client PID: 27191
Enter message: How are you?
Message sent to server.
Server Response: Server processed: How are you?
```

---

# Server Output

Example server output:

```text
Server started...
Waiting for clients...

Received from Client PID 27171: Hello Server
Response sent to Client PID 27171

Received from Client PID 27191: How are you?
Response sent to Client PID 27191
```

---

# Multiple Client Behavior

Multiple clients can communicate with the same server.

All clients write their requests to:

```text
/tmp/server_fifo
```

The server reads the requests and processes them sequentially.

Each client receives its response through its own FIFO:

```text
/tmp/client_<PID>_fifo
```

This prevents responses intended for one client from being received by another client.

The request contains:

```text
client_pid
message
```

The server uses the client PID to identify the correct response FIFO.

---

# FIFO Cleanup

After testing, the FIFOs can be removed using:

```bash
rm -f /tmp/server_fifo /tmp/client_*_fifo
```

---

# Part 2 – POSIX Signal Handling

## Description

This part demonstrates POSIX signal handling using the `sigaction()` system call.

The program handles:

- `SIGINT`
- `SIGTERM`
- `SIGUSR1`

The signal handler sets flags of type `volatile sig_atomic_t`.

The main program checks these flags and performs the required action.

---

# Signal Handler Program

File:

```text
signal_handler.c
```

The program waits for signals using:

```c
pause();
```

When a signal is received, the corresponding handler is executed.

---

# Signals Used

## SIGINT

`SIGINT` is normally generated when the user presses:

```text
Ctrl + C
```

The program handles the interrupt signal.

## SIGUSR1

`SIGUSR1` is a user-defined signal.

It can be sent using:

```bash
kill -SIGUSR1 <PID>
```

Example output:

```text
SIGUSR1 received!
User-defined event handled.
```

## SIGTERM

`SIGTERM` requests termination of the process.

It can be sent using:

```bash
kill -SIGTERM <PID>
```

Example output:

```text
SIGTERM received!
Termination requested.
Program terminating gracefully...
```

---

# Compiling Signal Handler

Using GCC:

```bash
gcc -Wall -Wextra -g signal_handler.c -o signal_handler
```

Or using the Makefile:

```bash
make
```

---

# Running Signal Handler

Start the program:

```bash
./signal_handler
```

Find its process ID using:

```bash
ps
```

or:

```bash
pgrep signal_handler
```

Then send signals using:

```bash
kill -SIGUSR1 <PID>
kill -SIGINT <PID>
kill -SIGTERM <PID>
```

---

# Example Signal Testing

For `SIGUSR1`:

```text
SIGUSR1 received!
User-defined event handled.
```

For `SIGINT`:

```text
SIGINT received!
Interrupt signal handled.
```

For `SIGTERM`:

```text
SIGTERM received!
Termination requested.
Program terminating gracefully...
```

---

# Makefile

```makefile
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
```

---

# Make Commands

Compile all programs:

```bash
make
```

Remove generated executables:

```bash
make clean
```

Compile again:

```bash
make
```

---

# Project Structure

```text
pract6/
│
├── fifo_server.c
├── fifo_client.c
├── signal_handler.c
├── Makefile
├── README.md
└── .gitignore
```

---

# Result

The practical successfully demonstrates:

- Client–server communication using Named Pipes (FIFOs)
- Communication between multiple clients and one server
- Client-specific response FIFOs
- POSIX signal handling using `sigaction()`
- Handling of `SIGINT`
- Handling of `SIGUSR1`
- Handling of `SIGTERM`
- Graceful process termination
- Compilation and cleanup using a Makefile

The FIFO client–server communication and POSIX signal handling were tested successfully.
