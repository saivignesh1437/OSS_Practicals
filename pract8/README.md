# Practical 8 -- Dynamic Memory Allocation, Valgrind and Copy-on-Write

## Objective

To understand dynamic memory allocation in C using `malloc()`,
`calloc()`, `realloc()` and `free()`, analyze memory leaks using
Valgrind, and study Copy-on-Write (COW) behavior after `fork()` using
Linux `/proc` memory information.

------------------------------------------------------------------------

## Description

This practical consists of two major parts:

1.  **Dynamic Memory Allocation and Valgrind**
    -   Demonstration of `malloc()`
    -   Demonstration of `calloc()`
    -   Demonstration of `realloc()`
    -   Demonstration of `free()`
    -   Memory leak detection using Valgrind
    -   Deliberate memory leak experiment
    -   Fixing and verifying the memory leak
2.  **Copy-on-Write after fork()**
    -   Allocate and initialize 100 MB of memory
    -   Create a child process using `fork()`
    -   Observe memory usage of parent and child processes
    -   Modify one byte every 4096 bytes in the child
    -   Observe Copy-on-Write behavior using `/proc/<PID>/status` and
        `/proc/<PID>/smaps`

------------------------------------------------------------------------

# Part 1 -- Dynamic Memory Allocation and Valgrind

## File

``` text
dynamic_memory.c
```

## Program Description

The program demonstrates the four main dynamic memory allocation
functions in C:

-   `malloc()` -- allocates memory without initialization.
-   `calloc()` -- allocates memory and initializes the allocated memory
    to zero.
-   `realloc()` -- changes the size of previously allocated memory.
-   `free()` -- releases dynamically allocated memory.

The program first allocates space for five integers using `malloc()` and
stores:

``` text
10 20 30 40 50
```

It then uses `calloc()` to allocate five integers and displays the
initialized values:

``` text
0 0 0 0 0
```

The memory allocated using `malloc()` is then resized from five integers
to ten integers using `realloc()`.

The final values are:

``` text
10 20 30 40 50 60 70 80 90 100
```

Finally, all dynamically allocated memory is released using `free()` and
the pointers are set to `NULL`.

------------------------------------------------------------------------

## Compilation

``` bash
gcc -Wall -Wextra -g dynamic_memory.c -o dynamic_memory
```

## Execution

``` bash
./dynamic_memory
```

## Output

``` text
1. malloc() demonstration
Memory allocated using malloc():
10 20 30 40 50

2. calloc() demonstration
Memory allocated using calloc():
0 0 0 0 0

3. realloc() demonstration
Memory after realloc():
10 20 30 40 50 60 70 80 90 100

4. free() demonstration
Allocated memory successfully released.
```

------------------------------------------------------------------------

# Valgrind Memory Analysis

Valgrind was used to check whether the dynamically allocated memory was
properly released.

## Command

``` bash
valgrind --leak-check=full ./dynamic_memory
```

## Clean Memory Result

The program produced:

``` text
HEAP SUMMARY:
    in use at exit: 0 bytes in 0 blocks
    total heap usage: 4 allocs, 4 frees, 1,104 bytes allocated

All heap blocks were freed -- no leaks are possible

ERROR SUMMARY: 0 errors from 0 contexts
```

This confirms that all dynamically allocated memory was successfully
released.

------------------------------------------------------------------------

# Deliberate Memory Leak Experiment

To understand how Valgrind detects memory leaks, the following lines
were temporarily commented out:

``` c
// free(calloc_ptr);
// calloc_ptr = NULL;
```

The program was recompiled and executed using:

``` bash
gcc -Wall -Wextra -g dynamic_memory.c -o dynamic_memory
```

``` bash
valgrind --leak-check=full --show-leak-kinds=all ./dynamic_memory
```

## Valgrind Leak Detection Output

``` text
in use at exit: 20 bytes in 1 blocks

20 bytes in 1 blocks are definitely lost

LEAK SUMMARY:
    definitely lost: 20 bytes in 1 blocks
    indirectly lost: 0 bytes in 0 blocks
    possibly lost: 0 bytes in 0 blocks
    still reachable: 0 bytes in 0 blocks

ERROR SUMMARY: 1 errors from 1 contexts
```

The 20-byte leak corresponds to the five integers allocated using
`calloc()`.

The `free()` operation was then restored:

``` c
free(calloc_ptr);
calloc_ptr = NULL;
```

The program was recompiled and tested again.

## Final Valgrind Result

``` text
in use at exit: 0 bytes in 0 blocks
total heap usage: 4 allocs, 4 frees, 1,104 bytes allocated

All heap blocks were freed -- no leaks are possible

ERROR SUMMARY: 0 errors from 0 contexts
```

Therefore, the memory leak was successfully identified and fixed.

------------------------------------------------------------------------

# Part 2 -- Copy-on-Write After fork()

## File

``` text
cow_demo.c
```

## Program Description

The program demonstrates the Copy-on-Write mechanism used by Linux after
a process calls `fork()`.

The program first allocates:

``` text
100 MB
```

of memory and initializes every byte.

The parent process then displays its PID and waits for the user to press
Enter.

After `fork()`:

-   The parent and child have their own virtual address spaces.
-   The physical memory pages are initially shared.
-   The child waits before modifying the memory.
-   The child then modifies one byte every 4096 bytes.
-   The modified pages are copied and become private because of
    Copy-on-Write.

The loop used for modification is:

``` c
for (size_t i = 0; i < SIZE; i += 4096)
{
    data[i] = 2;
}
```

Since:

``` text
4096 bytes = 4 KB
```

the program touches approximately:

``` text
100 MB / 4 KB = 25,600 pages
```

------------------------------------------------------------------------

## Compilation

``` bash
gcc -Wall -Wextra -g cow_demo.c -o cow_demo
```

## Execution

``` bash
./cow_demo
```

## Observed Process IDs

During the experiment, the following PIDs were observed:

``` text
Parent PID: 7357
Child PID: 7363
```

The child displayed:

``` text
Child PID: 7363
Parent PID: 7357
Press Enter to modify memory...
```

------------------------------------------------------------------------

# Monitoring Process Memory

Linux process memory information was inspected using:

``` text
/proc/<PID>/status
```

and:

``` text
/proc/<PID>/smaps
```

The following fields were observed:

``` text
VmSize
VmRSS
VmData
VmExe
VmStk
```

The following fields from `smaps` were also examined:

``` text
Rss
Pss
Shared_Dirty
Private_Dirty
```

------------------------------------------------------------------------

# Memory Observation Before COW Modification

## Parent Process

``` text
VmSize:  106556 kB
VmRSS:   103784 kB
VmData:  104424 kB
VmStk:      132 kB
VmExe:        4 kB
```

For the large memory region:

``` text
Rss:              102404 kB
Pss:               51202 kB
Shared_Dirty:     102404 kB
Private_Dirty:         0 kB
```

## Child Process

``` text
VmSize:  106556 kB
VmRSS:   103264 kB
VmData:  104424 kB
VmStk:      132 kB
VmExe:        4 kB
```

For the large memory region:

``` text
Rss:              102404 kB
Pss:               51202 kB
Shared_Dirty:     102404 kB
Private_Dirty:         0 kB
```

The similar memory values and high `Shared_Dirty` value demonstrate that
the parent and child were sharing the physical memory pages after
`fork()`.

The `Pss` value was approximately half of the shared region because the
physical pages were shared between the two processes.

------------------------------------------------------------------------

# Memory Observation After COW Modification

The child modified one byte every 4096 bytes.

After the modification, the large memory region showed:

``` text
Rss:              102404 kB
Pss:              102402 kB
Shared_Dirty:          4 kB
Private_Dirty:    102400 kB
```

The important change was:

``` text
Shared_Dirty:     102404 kB  ->  4 kB
Private_Dirty:         0 kB  ->  102400 kB
```

This demonstrates the Copy-on-Write behavior.

When the child writes to the shared pages, Linux creates private copies
of the affected pages for the writing process.

The virtual memory values remained approximately the same:

``` text
VmSize: 106556 kB
VmData: 104424 kB
```

This is because Copy-on-Write changes the physical page ownership rather
than simply doubling the virtual address-space size.

------------------------------------------------------------------------

# Copy-on-Write Stages

## Before fork()

``` text
Parent
   |
   |---- Physical pages
   |
   +---- 100 MB memory
```

The parent process owns the allocated memory.

## Immediately after fork()

``` text
Parent ─────┐
            ├── Shared physical pages
Child  ─────┘
```

The parent and child have separate virtual mappings, while the physical
pages can initially be shared.

## After Child Writes

``` text
Parent ─────── Original pages

Child  ─────── Private copied pages
```

The pages modified by the child are copied and become private.

This is the main behavior demonstrated by the experiment.

------------------------------------------------------------------------

# Makefile

The project contains a Makefile to simplify compilation.

``` makefile
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
```

------------------------------------------------------------------------

# Make Commands

## Compile all programs

``` bash
make
```

## Remove compiled executables

``` bash
make clean
```

## Compile again

``` bash
make
```

The Makefile builds:

``` text
dynamic_memory
cow_demo
```

------------------------------------------------------------------------

# Project Structure

``` text
pract8/
│
├── dynamic_memory.c
├── cow_demo.c
├── Makefile
├── .gitignore
├── dynamic_memory
└── cow_demo
```

The compiled executables are ignored using `.gitignore`:

``` text
dynamic_memory
cow_demo
```

------------------------------------------------------------------------

# Concepts Demonstrated

## Dynamic Memory Allocation

-   `malloc()`
-   `calloc()`
-   `realloc()`
-   `free()`
-   NULL pointer checking
-   Memory initialization
-   Safe memory reallocation

## Memory Debugging

-   Valgrind
-   Heap usage analysis
-   Memory leak detection
-   Definitely lost memory
-   Error summary
-   Verification of released memory

## Process Memory

-   `fork()`
-   Parent and child processes
-   Virtual memory
-   Physical memory pages
-   Copy-on-Write

## Linux `/proc`

-   `/proc/<PID>/status`
-   `/proc/<PID>/smaps`
-   `VmSize`
-   `VmRSS`
-   `VmData`
-   `VmExe`
-   `VmStk`
-   `RSS`
-   `PSS`
-   `Shared_Dirty`
-   `Private_Dirty`

------------------------------------------------------------------------

# Result

The practical was successfully completed.

The dynamic memory allocation program demonstrated `malloc()`,
`calloc()`, `realloc()` and `free()`.

Valgrind was successfully used to verify a clean program, deliberately
detect a 20-byte memory leak, and confirm that the leak was fixed.

The Copy-on-Write experiment successfully demonstrated memory sharing
after `fork()` and the transition from shared pages to private pages
when the child modified one byte every 4096 bytes.

The `/proc/<PID>/status` and `/proc/<PID>/smaps` interfaces were used to
observe the memory behavior of the parent and child processes.
