# OSSP Practical Session 9

## File Copy Using Low-Level Linux I/O and Standard Library I/O + I/O Redirection Using dup2()

---

## Objective

To implement and compare file copying using:

- Low-level Linux system calls:

  - `open()`

  - `read()`

  - `write()`

  - `lseek()`

  - `close()`

- C standard library I/O:

  - `fopen()`

  - `fread()`

  - `fwrite()`

  - `fseek()`

  - `ftell()`

  - `fclose()`

- Standard input/output redirection using:

  - `dup2()`

  - `STDIN_FILENO`

  - `STDOUT_FILENO`

The practical also compares the execution time of low-level and standard-library file copying and demonstrates how standard input and output can be redirected using file descriptors.

---

# Part 1: File Copy and I/O Performance Comparison

## Description

Two file-copy programs were implemented.

### 1. Low-Level I/O

`copy_lowlevel.c` uses:

- `open()`

- `read()`

- `write()`

- `lseek()`

- `close()`

The program works directly with Linux file descriptors.

### 2. Standard Library I/O

`copy_stdio.c` uses:

- `fopen()`

- `fread()`

- `fwrite()`

- `fseek()`

- `ftell()`

- `fclose()`

The program uses C `FILE *` streams.

Both programs use an 8192-byte buffer and were compiled using the same optimization level for a fair comparison.

---

## Low-Level File Copy Program

### File

```text

copy_lowlevel.c

```

### Compilation

```bash

gcc -Wall -Wextra -O2 copy_lowlevel.c -o copy_lowlevel

```

### Execution

```bash

./copy_lowlevel test_input.txt copied_lowlevel.txt

```

### Actual Output

```text

Source file size: 122 bytes

File copied successfully.

```

### Verification

```bash

cmp test_input.txt copied_lowlevel.txt

```

No output was produced by `cmp`, confirming that both files are identical.

SHA-256 checksum:

```text

3a56f87fee4ba7d870a866d44c70d89b7d43b3f81fca6a8b3fe8d07ea8373220

```

The same checksum was obtained for both the source and copied file.

---

# Standard Library File Copy Program

## File

```text

copy_stdio.c

```

## Compilation

```bash

gcc -Wall -Wextra -O2 copy_stdio.c -o copy_stdio

```

## Execution

```bash

./copy_stdio test_input.txt copied_stdio.txt

```

## Actual Output

```text

Source file size: 122 bytes

File copied successfully.

```

## Verification

```bash

cmp test_input.txt copied_stdio.txt

```

No output was produced by `cmp`, confirming that both files are identical.

SHA-256 checksum:

```text

3a56f87fee4ba7d870a866d44c70d89b7d43b3f81fca6a8b3fe8d07ea8373220

```

---

# Performance Comparison

A 100 MB test file was created using:

```bash

dd if=/dev/zero of=large_input.bin bs=1M count=100

```

The file size was:

```text

100M

```

Each program was executed three times using the `time` command.

## Low-Level I/O Results

### Run 1

```text

real    0m0.092s

user    0m0.004s

sys     0m0.067s

```

### Run 2

```text

real    0m0.157s

user    0m0.004s

sys     0m0.098s

```

### Run 3

```text

real    0m0.134s

user    0m0.002s

sys     0m0.095s

```

Average real time:

```text

0.128 seconds

```

---

## Standard Library I/O Results

### Run 1

```text

real    0m0.114s

user    0m0.007s

sys     0m0.101s

```

### Run 2

```text

real    0m0.123s

user    0m0.005s

sys     0m0.087s

```

### Run 3

```text

real    0m0.096s

user    0m0.005s

sys     0m0.072s

```

Average real time:

```text

0.111 seconds

```

---

## Performance Summary

| Implementation | Run 1 | Run 2 | Run 3 | Average |

|---|---:|---:|---:|---:|

| Low-Level I/O | 0.092 s | 0.157 s | 0.134 s | 0.128 s |

| Standard Library I/O | 0.114 s | 0.123 s | 0.096 s | 0.111 s |

In this particular experiment, the standard-library implementation had a slightly lower average real execution time.

The observed timing is specific to this test environment and can vary depending on system load, filesystem caching, storage, and other factors.

---

## Large File Verification

The generated 100 MB copies were checked using:

```bash

cmp large_input.bin large_lowlevel.bin

cmp large_input.bin large_stdio.bin

```

Both commands produced no output, confirming that the copied files are identical.

SHA-256 checksum for all three files:

```text

20492a4d0d84f8beb1767f6616229f85d44c2827b64bdbfb260ee12fa1109e0e

```

Files verified:

```text

large_input.bin

large_lowlevel.bin

large_stdio.bin

```

---

# Part 2: I/O Redirection Using dup2()

## Description

Linux processes normally use three standard file descriptors:

```text

0 → Standard Input

1 → Standard Output

2 → Standard Error

```

The `dup2()` system call can redirect one file descriptor to another.

The practical demonstrates:

```c

dup2(fd, STDOUT_FILENO);

```

for output redirection and:

```c

dup2(fd, STDIN_FILENO);

```

for input redirection.

---

# Redirect Standard Output

## File

```text

redirect_output.c

```

The program opens `output.txt` and redirects standard output to the file using:

```c

dup2(fd, STDOUT_FILENO);

```

After the redirection, calls to `printf()` write into `output.txt` instead of displaying the text on the terminal.

## Compilation

```bash

gcc -Wall -Wextra -O2 redirect_output.c -o redirect_output

```

## Execution

```bash

./redirect_output

```

No program output appeared on the terminal because standard output was redirected.

## Verification

```bash

cat output.txt

```

Actual output:

```text

This output is redirected to output.txt

Standard output has been successfully redirected.

```

File verification:

```bash

ls -l output.txt

```

Actual result:

```text

-rw-r--r-- 1 sai-vignesh sai-vignesh 90 Oct 4 22:19 output.txt

```

This confirms that standard output was successfully redirected to a file.

---

# Redirect Standard Input

## File

```text

redirect_input.c

```

An input file was created containing:

```text

Operating Systems

Linux File I/O

dup2 system call

```

The program opens `input.txt` using:

```c

open("input.txt", O_RDONLY);

```

and redirects standard input using:

```c

dup2(fd, STDIN_FILENO);

```

The program then uses `fgets()` to read from `stdin`.

## Compilation

```bash

gcc -Wall -Wextra -g redirect_input.c -o redirect_input

```

The program was also successfully rebuilt through the Makefile using:

```bash

gcc -Wall -Wextra -O2 redirect_input.c -o redirect_input

```

## Execution

```bash

./redirect_input

```

Actual output:

```text

Reading from redirected standard input:

Operating Systems

Linux File I/O

dup2 system call

```

No keyboard input was required because standard input was redirected to `input.txt`.

---

# Makefile

The project contains a Makefile for compiling all four programs.

## Build All Programs

```bash

make

```

The following programs are compiled:

```text

copy_lowlevel

copy_stdio

redirect_output

redirect_input

```

## Clean Build Files

```bash

make clean

```

The clean command removes:

```text

copy_lowlevel

copy_stdio

redirect_output

redirect_input

```

The clean operation was verified using `ls`, which confirmed that the executables were removed.

The programs were then successfully rebuilt using:

```bash

make

```

---

# Compilation Verification

All programs were compiled successfully without compiler errors or warnings.

Programs:

```text

copy_lowlevel.c

copy_stdio.c

redirect_output.c

redirect_input.c

```

Build command:

```bash

make

```

Successful compilation commands:

```text

gcc -Wall -Wextra -O2 copy_lowlevel.c -o copy_lowlevel

gcc -Wall -Wextra -O2 copy_stdio.c -o copy_stdio

gcc -Wall -Wextra -O2 redirect_output.c -o redirect_output

gcc -Wall -Wextra -O2 redirect_input.c -o redirect_input

```

---

# Project Structure

```text

pract9/

├── .gitignore

├── Makefile

├── README.md

├── copy_lowlevel.c

├── copy_stdio.c

├── redirect_output.c

└── redirect_input.c

```

Generated executables and test data files are excluded using `.gitignore`.

Ignored files include:

```text

copy_lowlevel

copy_stdio

redirect_output

redirect_input

test_input.txt

copied_lowlevel.txt

copied_stdio.txt

large_input.bin

large_lowlevel.bin

large_stdio.bin

input.txt

output.txt

```

---

# Result

The Practical Session 9 objectives were successfully completed.

1. A file-copy utility was implemented using low-level Linux I/O system calls.

2. A second file-copy utility was implemented using C standard-library I/O.

3. Both implementations successfully copied files without data corruption.

4. File contents were verified using `cmp`.

5. File integrity was verified using SHA-256 checksums.

6. A 100 MB file was used to compare the execution time of both approaches.

7. In the recorded experiment, standard-library I/O had a slightly lower average real execution time.

8. Standard output was successfully redirected to `output.txt` using `dup2()`.

9. Standard input was successfully redirected from `input.txt` using `dup2()`.

10. A Makefile was created to build and clean all Practical 9 programs.

11. `make clean` and `make` were successfully tested.

12. All programs compiled successfully using GCC with warning flags enabled.
