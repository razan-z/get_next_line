_This project has been created as part of the 42 curriculum by rzbib._

# get_next_line

## Description

**`get_next_line`** is a C programming project in the 42 curriculum focused on file I/O, buffer management, and static memory persistence. The goal is to implement a function, `char *get_next_line(int fd)`, that reads and returns one line at a time from a given file descriptor (`fd`), including the terminating newline character (`\n`) when present, until reaching the end of the file (`NULL`).

### Key Objectives

- **Persistent State Without Globals:** Use a `static` variable to retain unread leftover bytes across consecutive function calls.
- **Buffered File Reading:** Handle arbitrary `BUFFER_SIZE` values defined at compile time (from `1` byte to several megabytes) using the `read()` system call.
- **Strict Memory Safety:** Dynamically allocate, concatenate, slice, and free heap memory (`malloc` / `free`) with zero memory leaks or dangling pointers.
- **Multi-FD Support (Bonus):** Manage multiple open file descriptors simultaneously using a single static array of pointers without losing the reading position of any stream.

---

## Instructions

### Prerequisites

- A C compiler (`cc`, `gcc`, or `clang`)
- Standard C headers (`<stdlib.h>`, `<unistd.h>`)

### Compilation

Per the project guidelines, no `Makefile` is included in this repository. Compile the source files directly alongside your test `main.c` using `-Wall -Wextra -Werror` and specify the desired `BUFFER_SIZE` via the `-D` flag (defaults to `42` if omitted).

**Mandatory Part:**

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
```

**Bonus Part (Multiple File Descriptors):**

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main_bonus.c -o gnl_bonus
```

### Execution & Usage Example

Include `"get_next_line.h"` (or `"get_next_line_bonus.h"`) in your source file and call `get_next_line(fd)` inside a loop until it returns `NULL`. Each returned line is heap-allocated and must be freed by the caller.

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("test.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}
```

Run the compiled program:

```bash
./gnl
```

---

## Algorithm & Technical Choices

### Selected Algorithm: Static Stash & Four-Stage Pipeline

Because `read()` retrieves fixed chunks of `BUFFER_SIZE` bytes without awareness of line boundaries—and because file descriptors cannot be rewound—the implementation uses a **persistent accumulation buffer (`static char *stash`)** paired with a four-stage processing pipeline on every call:

1. **Read & Accumulate (`read_to_stash`):**
   - Allocates a temporary buffer of `BUFFER_SIZE + 1` bytes on the **heap** rather than the stack to prevent stack overflow when testing very large `BUFFER_SIZE` values (e.g., `10,000,000`).
   - Loops `read(fd, buf, BUFFER_SIZE)` and appends each chunk to `stash` via an auto-freeing `ft_strjoin` until `stash` contains a `\n` character or `read()` returns `0` (EOF).
   - If `stash` already contains a `\n` from a previous over-read, the loop skips `read()` entirely and proceeds straight to extraction.
2. **Line Extraction (`extract_line`):**
   - Scans `stash` up to and including the first `\n` (or `\0` if EOF is reached without a trailing newline).
   - Allocates an exact-sized string for the line, copies the characters, and returns it to the caller.
3. **Stash Trimming (`clean_stash`):**
   - Copies all remaining bytes located after the first `\n` into a newly allocated string, frees the previous `stash` buffer to prevent memory leaks, and updates `stash` for the next call.
   - When EOF is reached (`!stash[i]`), frees `stash` and returns `NULL` so no memory remains allocated after the file ends.
4. **Dangling Pointer Protection:**
   - Whenever `extract_line` returns `NULL` (at EOF or on allocation failure), `get_next_line` explicitly frees `stash` and resets `stash = NULL` to prevent use-after-free bugs on subsequent calls.

### Justification of Data Structures

- **Mandatory (`static char *stash`):** Provides $O(1)$ access to leftover bytes across calls while keeping state encapsulated inside `get_next_line` without global variables.
- **Bonus (`static char *stash[FD_MAX]`):** Uses an array of pointers indexed directly by file descriptor (`FD_MAX = 4096`). Direct array indexing achieves **$O(1)$ lookup time** per `fd` and avoids the code complexity, extra allocations, and fragmentation overhead of maintaining a dynamic linked list of open file descriptors.

---

## Resources

### Documentation & References

- **42 Curriculum Subject:** _get_next_line — Reading a line from a fd is way too tedious_ (42 Intranet)
- **42 Cursus Guide (`get_next_line`):** [42 Cursus GitBook — get_next_line](https://42-cursus.gitbook.io/guide/1-rank-01/get_next_line)
- **Project Walkthrough & Architecture:** [Medium — GNL Explained by lannur-s](https://medium.com/@lannur-s/gnl-c3cff1ee552b)
- **File Descriptors & System I/O:** [DEV Community — GET NEXT LINE: A 42 Project to Learn How to Deal with File Descriptors and I/O of System](https://dev.to/aerrfig/get-next-line-a-42-project-to-learn-how-to-deal-with-file-descriptors-and-io-of-system-3652)
- **Static Variables in C:** [GeeksforGeeks — Static Variables in C](https://www.geeksforgeeks.org/c/static-variables-in-c/)
* **Tripouille's `gnlTester`:** [GitHub — Tripouille/gnlTester](https://github.com/Tripouille/gnlTester) (used to verify mandatory and bonus edge cases, malloc sizing, and leak checks).
* **Valgrind:** [Valgrind User Manual](https://valgrind.org/docs/manual/manual.html) (used to verify zero memory leaks and invalid memory reads across varying `BUFFER_SIZE` values).

### Use of AI

- **Concept Review & Edge-Case Analysis:** AI was used to clarify C memory lifecycle behavior (such as `static` storage duration, pointer-by-value behavior in `free()`, and stack vs. heap allocation limits for large `BUFFER_SIZE` values).
- **Testing & Documentation:** AI assisted in designing multi-file descriptor test harnesses for Valgrind verification and structuring this `README.md` document to meet the 42 curriculum specifications. All C source code, helper utilities, and Norminette compliance were written and validated manually.
