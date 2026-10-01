# Practical 6 – FIFO Client–Server and POSIX Signal Handling

## Objective

This practical demonstrates:

1. Client–server communication using Named Pipes (FIFOs).
2. Communication between multiple clients and a single server.
3. Separate response FIFOs for individual clients.
4. POSIX signal handling using `sigaction()`.
5. Handling of SIGINT, SIGTERM, and SIGUSR1.

---

# Part 1 – Client–Server Application Using Named Pipes

## Description

The client–server application uses Named Pipes (FIFOs) for inter-process communication.

The server creates a common FIFO:

```text
/tmp/server_fifo
