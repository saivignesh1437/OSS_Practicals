# Practical 7 – Understanding Linux Process Address Space

## Objective

This practical demonstrates:

- Understanding the Linux process virtual address space
- Identifying Code/Text, Data, BSS, Heap, and Stack regions
- Printing addresses of different types of variables
- Analyzing `/proc/<PID>/maps`
- Understanding memory permissions
- Identifying shared libraries and memory-mapped regions
- Understanding Address Space Layout Randomization (ASLR)

---

# Part 1 – Linux Process Address Space

## Description

A Linux process is divided into different memory regions.

The program demonstrates the different regions by printing the addresses of:

- Code/Text
- Global variable
- Static variable
- BSS variable
- Heap variable
- Stack variable

The actual virtual addresses may vary between executions because of Address Space Layout Randomization (ASLR).

## Files

- `memory_layout.c` – Program to display addresses of different memory regions
- `memory_demo.c` – Program used to inspect the process address space
- `Makefile` – Compilation and cleanup
- `.gitignore` – Ignores generated executable files

---

# Memory Layout Program

The `memory_layout.c` program prints the addresses of variables and a function belonging to different regions of the Linux process address space.

The program contains:

1. Code/Text address
2. Initialized global variable
3. Static variable
4. Uninitialized global variable
5. Dynamically allocated heap variable
6. Local stack variable

---

# Code/Text Region

The Code/Text region contains the program instructions.

The address of a function is used to represent the Code/Text region.

Example:

```c
printf("Code address : %p\n", (void *)display_code_address);
```

---

# Global/Data Region

An initialized global variable is stored in the Data section.

Example:

```c
int global_var = 10;
```

Its address is printed to demonstrate the Data region.

---

# Static Region

An initialized static variable is stored in the Data section.

Example:

```c
static int static_var = 20;
```

Its address is printed separately in the program.

---

# BSS Region

An uninitialized global variable is stored in the BSS section.

Example:

```c
int global_uninit;
```

The address of this variable represents the BSS region.

---

# Heap Region

Dynamic memory is allocated using `malloc()`.

Example:

```c
int *heap_var = malloc(sizeof(int));
```

The address returned by `malloc()` represents the Heap region.

The allocated memory is released using:

```c
free(heap_var);
```

---

# Stack Region

Local variables inside a function are stored on the Stack.

Example:

```c
int stack_var = 30;
```

The address of the local variable represents the Stack region.

---

# Compilation

The program can be compiled using:

```bash
gcc -Wall -Wextra -g memory_layout.c -o memory_layout
```

Or all programs can be compiled using the Makefile:

```bash
make
```

---

# Execution

Run the memory layout program using:

```bash
./memory_layout
```

Example:

```text
===== Linux Process Address Space =====

Code address : 0xba9016240968
Global address : 0xba9016260010
Static address : 0xba9016260014
BSS address : 0xba901626001c
Heap address : 0xba904428e010
Stack address : 0xfffff8ffa5dc
```

The actual addresses can be different during different executions because Linux uses ASLR.

---

# Address Space Analysis

The output demonstrates the different regions of the Linux process address space.

```text
+-----------------------------+
|            Stack            |
|       Local Variables       |
+-----------------------------+
|                             |
|            Heap             |
|      Dynamic Allocation     |
+-----------------------------+
|                             |
|       Data / BSS            |
| Global & Static Variables   |
+-----------------------------+
|                             |
|        Code / Text          |
|     Program Instructions    |
+-----------------------------+
```

The exact virtual addresses depend on the system and ASLR.

---

# Part 2 – Inspecting `/proc/<PID>/maps`

## Description

The `memory_demo.c` program keeps a process running so that its virtual memory mappings can be inspected through the Linux `/proc` filesystem.

The program prints:

- Process ID (PID)
- Code address
- Global variable address
- Static variable address
- BSS variable address
- Heap address
- Stack address

After printing these addresses, the program continues running using:

```c
while (1)
{
    sleep(10);
}
```

This allows the process memory layout to be examined from another terminal.

---

# Compiling memory_demo.c

Using GCC:

```bash
gcc -Wall -Wextra -g memory_demo.c -o memory_demo
```

Or using the Makefile:

```bash
make
```

---

# Running memory_demo

Start the program:

```bash
./memory_demo
```

Example:

```text
Process ID (PID): 28745
Address of code : 0xb74b0c4f0000
Address of global : 0xb74b0c510000
Address of static : 0xb74b0c510004
Address of BSS : 0xb74b0c51000c
Address of heap : 0xb74b2f1fe010
Address of stack : 0xffffe79d0000

Process is running...
```

Keep the program running.

Open another terminal and use the PID printed by the program.

---

# Examining `/proc/<PID>/maps`

For example:

```bash
cat /proc/28745/maps
```

The `/proc/<PID>/maps` file displays the virtual memory mappings of the process.

Example output:

```text
b74b0c4f0000-b74b0c4f1000 r-xp 00000000 103:02 786365                    /home/sai-vignesh/Practicals/pract7/memory_demo
b74b0c50f000-b74b0c510000 r--p 0000f000 103:02 786365                    /home/sai-vignesh/Practicals/pract7/memory_demo
b74b0c510000-b74b0c511000 rw-p 00010000 103:02 786365                    /home/sai-vignesh/Practicals/pract7/memory_demo
b74b2f1fe000-b74b2f400000 rw-p 00000000 00:00 0                          [heap]
e2d651530000-e2d6516d9000 r-xp 00000000 103:02 1050852                   /usr/lib/aarch64-linux-gnu/libc.so.6
e2d6516d9000-e2d6516ed000 ---p 001a9000 103:02 1050852                   /usr/lib/aarch64-linux-gnu/libc.so.6
e2d6516ed000-e2d6516f0000 r--p 001ad000 103:02 1050852                   /usr/lib/aarch64-linux-gnu/libc.so.6
e2d6516f0000-e2d6516f2000 rw-p 001b0000 103:02 1050852                   /usr/lib/aarch64-linux-gnu/libc.so.6
e2d6516f2000-e2d6516fe000 rw-p 00000000 00:00 0
e2d651712000-e2d65173c000 r-xp 00000000 103:02 1050849                   /usr/lib/aarch64-linux-gnu/ld-linux-aarch64.so.1
e2d651746000-e2d65174a000 rw-p 00000000 00:00 0
e2d65174a000-e2d65174e000 r--p 00000000 00:00 0                          [vvar]
e2d65174e000-e2d651750000 r-xp 00000000 00:00 0                          [vdso]
e2d651750000-e2d651752000 r--p 0002e000 103:02 1050849                   /usr/lib/aarch64-linux-gnu/ld-linux-aarch64.so.1
e2d651752000-e2d651753000 rw-p 00030000 103:02 1050849                   /usr/lib/aarch64-linux-gnu/ld-linux-aarch64.so.1
e2d651753000-e2d651754000 rw-p 00000000 00:00 0
ffffe79d0000-ffffe79f1000 rw-p 00000000 00:00 0                          [stack]
```

---

# Understanding `/proc/<PID>/maps`

Each line in `/proc/<PID>/maps` contains information about one virtual memory mapping.

The general format is:

```text
start_address-end_address permissions offset device inode pathname
```

For example:

```text
b74b0c4f0000-b74b0c4f1000 r-xp 00000000 103:02 786365 memory_demo
```

The fields represent:

- Start address
- End address
- Memory permissions
- File offset
- Device
- Inode
- Mapping/path

---

# Memory Permissions

The permission field can contain:

```text
r
w
x
p
s
```

Meaning:

- `r` – Read permission
- `w` – Write permission
- `x` – Execute permission
- `p` – Private mapping
- `s` – Shared mapping

For example:

```text
r-xp
```

means:

- Readable
- Executable
- Not writable
- Private mapping

---

# Analysis of Major Memory Regions

## Code/Text Region

The program executable contains an executable mapping such as:

```text
r-xp
```

This region contains the program instructions.

The `x` permission allows the CPU to execute instructions from this region.

---

## Data Region

Initialized global and static variables are stored in the writable part of the program's memory mapping.

Example:

```text
rw-p
```

The `w` permission allows modification of the stored data.

---

## BSS Region

The BSS contains uninitialized global and static variables.

The BSS is part of the program's writable memory area and may not appear as a separate `[bss]` label in `/proc/<PID>/maps`.

---

## Heap Region

The heap is clearly identified by:

```text
[heap]
```

Example:

```text
b74b2f1fe000-b74b2f400000 rw-p 00000000 00:00 0 [heap]
```

Dynamic memory allocated using functions such as:

```c
malloc()
calloc()
realloc()
```

is associated with the heap.

---

## Stack Region

The stack is identified by:

```text
[stack]
```

Example:

```text
ffffe79d0000-ffffe79f1000 rw-p 00000000 00:00 0 [stack]
```

Local variables and function call information are stored on the stack.

---

## Shared Libraries

The process also contains mappings for shared libraries.

For example:

```text
/usr/lib/aarch64-linux-gnu/libc.so.6
```

`libc.so.6` provides standard C library functionality.

The dynamic linker is also mapped into the process:

```text
/usr/lib/aarch64-linux-gnu/ld-linux-aarch64.so.1
```

---

## Memory-Mapped Regions

The process may also contain special memory mappings such as:

```text
[vvar]
[vdso]
```

These are part of the Linux process virtual memory layout.

---

# Other Useful `/proc` Memory Files

## `/proc/<PID>/maps`

Displays the memory mappings of a process.

```bash
cat /proc/<PID>/maps
```

---

## `/proc/<PID>/smaps`

Provides detailed information about each memory mapping.

```bash
cat /proc/<PID>/smaps
```

---

## `/proc/<PID>/status`

Displays process information including memory usage.

```bash
grep -E "VmSize|VmRSS|VmData|VmStk|VmExe|VmLib" /proc/<PID>/status
```

Important fields include:

- `VmSize` – Virtual memory size
- `VmRSS` – Resident Set Size
- `VmData` – Data segment
- `VmStk` – Stack
- `VmExe` – Executable memory
- `VmLib` – Shared library memory

---

# Using pmap

The `pmap` command can also be used to examine process memory.

```bash
pmap <PID>
```

Example:

```bash
pmap 28745
```

It provides a summary of the memory mappings of the process.

---

# Memory Analysis Tools

The following Linux tools can be used to analyze process memory:

```text
/proc/<PID>/maps
/proc/<PID>/smaps
/proc/<PID>/status
pmap
free
top
htop
vmstat
gdb
readelf
size
```

---

# Useful Commands

Find the running process:

```bash
ps aux | grep memory_demo
```

Display memory mappings:

```bash
cat /proc/<PID>/maps
```

Display detailed memory mapping information:

```bash
cat /proc/<PID>/smaps
```

Display memory-related process information:

```bash
grep -E "VmSize|VmRSS|VmData|VmStk|VmExe|VmLib" /proc/<PID>/status
```

Display memory mapping summary:

```bash
pmap <PID>
```

Display ELF sections:

```bash
readelf -S memory_demo
```

Display size information:

```bash
size memory_demo
```

---

# Virtual Memory Organization

The addresses printed by the programs are virtual addresses.

For example:

```c
printf("%p", (void *)&global_var);
```

prints the virtual address of the variable in the process address space.

The CPU's Memory Management Unit (MMU) and page tables are responsible for translating virtual addresses to physical memory.

Therefore, the address printed by the program does not directly represent a physical RAM address.

---

# Role of ASLR

Linux uses Address Space Layout Randomization to randomize the locations of different memory regions.

Therefore, running:

```bash
./memory_layout
```

multiple times can produce different addresses.

For example:

```text
Run 1:
Code address : 0xba9016240968

Run 2:
Code address : different address
```

This behavior makes it harder for an attacker to predict fixed memory locations.

---

# Makefile

```makefile
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

Run the memory layout program:

```bash
./memory_layout
```

Run the memory demonstration program:

```bash
./memory_demo
```

---

# Project Structure

```text
pract7/
│
├── memory_layout.c
├── memory_demo.c
├── Makefile
├── README.md
└── .gitignore
```

---

# Result

The practical successfully demonstrates:

- Linux process virtual address space
- Code/Text region
- Data region
- BSS region
- Heap region
- Stack region
- Printing addresses of different memory regions
- Inspecting `/proc/<PID>/maps`
- Understanding memory permissions
- Identifying shared libraries
- Identifying memory-mapped regions
- Understanding virtual memory
- Understanding Address Space Layout Randomization (ASLR)
- Compilation and cleanup using a Makefile

The Linux process address space and `/proc/<PID>/maps` were successfully examined.
