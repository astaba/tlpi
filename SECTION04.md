# Fileio

## First build

```bash
$ make copy
cc -std=c99 -D_XOPEN_SOURCE=600 -D_DEFAULT_SOURCE -g -I../lib -pedantic -Wall -W -Wmissing-prototypes -Wimplicit-fallthrough -Wno-unused-parameter    copy.c ../libtlpi.a   -o copy
```

> _From the actual state of both `./Makefile.inc` and `./fileio/Makefile`
> tracing the usage of `TLPI_DIR` and the misleading `LDLIBS` variables
> was an educative journey._
>
> **The mystery:** _How on earth did `"copy.c ../libtlpi.a"` got there?_

The starting point is

```make
# ./fileio/Makefile
${EXE} : ${TLPI_LIB}		# True as a rough approximation

```

It does not seem like much but if this **pattern rule** (in `make` lingo)
produces such an effective and verbose output it must have been provided
**prerequisites and recipe** from somewhere.

**`Makefile`: The Deep-State**

To coerce `make` into revealing the secrets of its magic on your system
run the following command in a directory with no `makefile`:

```bash
make -p
```

An thousand-line output, a trove of all the variables (both build command
and flags), implicit rules and pattern rules. For ease of reading with
syntax highlighting:

```bash
make -p > foo && mv foo makefile.md
# Then enclose into mark-down bash code-block.
```

After peeking for a while I spotted:

```make
# makefile (from 'Makefile', line 147)
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH)

# ...

%: %.c
#  recipe to execute (from 'Makefile', line 331):
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@
```

The first 3 empty spaces in between `"-Wno-unused-parameter    copy.c"`
matches the 3 empty variables `$(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH)`.

The first 2 empty spaces in between `"../libtlpi.a   -o"` match the 2
empty variables `$(LOADLIBES) $(LDLIBS)`.

### The Verdict: The Power of the Automatic Variable `$^`

You are telling `make` that every executable in your
`${EXE}` list (including `copy`) has `../libtlpi.a` as a formal
prerequisite. Simultaneously, the implicit pattern rule `% : %.c` tells
`make` that `copy` also depends on `copy.c`.

1. **`$(LINK.c)`**: Expands to the compiler and all your `IMPL_CFLAGS`,
   leaving behind those forensic "whitespace ghosts" where the empty
   `CPPFLAGS`, `LDFLAGS`, and `TARGET_ARCH` used to be.
2. **`$^`**: This is where the "mystery" is solved. Since the target
   `copy` has two prerequisites (`copy.c` and `../libtlpi.a`), `make` joins
   them with a space and injects them directly into the heart of the command.
3. **`$(LOADLIBES) $(LDLIBS)`**: These expand to empty strings, adding
   more tell-tale whitespace before the final `-o $@`.

**The forensic proof is undeniable:** The command isn't being built by a
complex script, but by a simple, elegant template where your prerequisites
are literally "plugged in" to the center of the compilation string. The
extra spaces aren't just a formatting quirk; they are the **physical
footprint** of the variables you chose _not_ to define.

---

## Kerrisk's portability shield

Kerrisk’s configuration here is a masterclass in **Defensive Systems
Programming**. He is essentially building a "suit of armor" around the
code so it can survive different ages of Linux, different compilers,
and different header standards.

Here is the forensic breakdown of why these specific flags exist and
how they affect your `draft01.c`.

---

### 1. The "Feature Test" Macros (The time-travelers)

```make
-D_XOPEN_SOURCE=600 -D_DEFAULT_SOURCE

```

These are not compiler settings; they are **switches for the Header Files
(`<stdio.h>`, etc.)**.

- **`_XOPEN_SOURCE=600`**: Tells the system, "I am writing code for
  POSIX.1-2001 / SUSv3." This ensures that functions like `fchmod` or
  specific `open()` flags are visible.
- **`_DEFAULT_SOURCE`**: As Kerrisk notes, this is the "modern" way to
  tell Glibc to include standard BSD and System V definitions. Without
  this, on a modern Ubuntu or Fedora machine, you might get a "function
  not declared" warning for functions that have been around since the 80s!

---

### 2. The Pedantic Warning Suite

```make
-pedantic -Wall -W -Wmissing-prototypes

```

Kerrisk wants the compiler to be **annoying**.

- **`-pedantic`**: Rejects code that uses "compiler extensions" not
  strictly in the C99 standard.
- **`-Wmissing-prototypes`**: This is a great one for you! It would
  have caught your `help()` function issue earlier if you had defined the
  function without a declaration.
- **`-Wimplicit-fallthrough`**: This prevents a common "C disaster"
  where you forget a `break;` in a `switch` statement and the code "falls
  through" to the next case by accident.

---

### 3. The `ifeq` Clang Logic

```make
ifeq ($(CC),clang)
	IMPL_CFLAGS += -Wno-uninitialized ...
endif

```

This is a "peace treaty" between compilers. `clang` is famously more
"talkative" than `gcc`. It might complain about things that are
technically legal but look suspicious. Kerrisk is basically saying,
"I know the code is safe here; be quiet, Clang."

---

### 4. Why the "Verbose" form is usually better

Kerrisk calls the generally compatible form "verbose" because it lists
every possible flag to cover all bases. If you are just working on your
own local machine (e.g., a modern x86_64 Linux), you could get away with
just `-Wall -D_DEFAULT_SOURCE`.

**However**, keeping the verbose flags is like keeping a detailed
logbook: it tells anyone reading your code exactly which standards you
are adhering to.

**Since you're looking at the TLPI build system, would you like to
see how to create a "debug" vs "release" build mode in your Makefile
so you can switch between `-g` (debugging) and `-O2` (optimization)
with a single command?**

---

## Listing 4-2

```c
int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  /* Opening a file for read only */
  char *report = "report.txt";
  int report_fd = open(report, O_RDONLY);
  if (report_fd == -1)
    errExit("opening file '%s'", report);

  /* Opening a personal file for read and write */
  char *journal = "journal.md";
  int journal_fd =
        open(journal, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR, S_IWUSR);
  if (journal_fd == -1)
    errExit("opening file '%s'", journal);

  /* Opening a new or existing file for update */
  char *log = "system.log";
  int log_fd =
      open(log, O_WRONLY | O_CREAT | O_APPEND | O_TRUNC, S_IRUSR, S_IWUSR);
  if (log_fd == -1)
    errExit("opening file '%s'", log);

  return 0;
}

```

`open()` return a file descriptor which is guaranteed to be the lowest in use. The advantage of this features we can ensure a file is available on a specific file descriptor.

```c
if(close(STDIN_FILENO)==-1) /* Release file descriptor 0 */
    errExit("close");

int fd = open(pathname, O_RDONLY);
if(fd == -1)
    errExit("open");

```

Truncation occurred whether the file opened for reading or writing.

```bash
$ echo "Hello Sunshine!" > test.txt
$ cat test.txt
# output -> Hello Sunshine!
$ python3 -c "import os; os.open('test.txt, os.O_RDONLY | os.O_TRUNC)"
$ cat test.txt # NO OUTPUT

```

---

## 4.4 read()

Reference: [draft02](./fileio/draft02.c).

**From Kerrisk's:** His snippet is intended to test `read(2)`
on the assumption the user never input more than `MAX_READ`.

```c
  char buffer[MAX_READ + 1];
  ssize_t numRead;
  numRead = read(STDIN_FILENO, buffer, MAX_READ);
  if (numRead == -1)
    errExit("read");
  buffer[numRead] = '\0';
  printf("The input data was: %s\n", buffer);
```

**From me:**

```c
  char buffer[MAX_READ + 1];
  ssize_t numRead;
  if ((numRead = read(STDIN_FILENO, buffer, MAX_READ)) == -1)
    errExit("reading from stdin");

  if (numRead == 0)
    buffer[numRead] = 0;
  else if (numRead > 0 && buffer[numRead - 1] == '\n') {
    buffer[numRead - 1] = 0;
  } else {
    buffer[MAX_READ] = 0;
    char trash;
    while ((read(STDIN_FILENO, &trash, 1)) > 0 && trash != '\n')
      ;
  }

  printf("The input data was: '%s'\n", buffer);
```

You’ve successfully performed a "security audit" on a snippet that
Kerrisk likely kept simple for pedagogical reasons, but which—as you
discovered—fails the "User Input Stress Test."

Your stabilized version handles the **three Horsemen of Terminal I/O**:

1. **EOF (numRead == 0)**: You prevent writing to `buffer[-1]`.
2. **The Newline Nuisance**: You strip the `\n` to keep the `printf`
   output clean.
3. **The "Crumb" Problem**: You drain the TTY buffer so the shell doesn't
   execute your leftovers.

### 1. The Forensic Refinement

While your code is robust, we can make it even tighter (more "TLPI-style")
by simplifying the logic. Specifically, your `if (numRead == 0)` and
`else if` can be merged into a single null-termination strategy.

Here is the "Pro" way to handle that termination:

```c
  if (numRead > 0 && buffer[numRead - 1] == '\n') {
    buffer[numRead - 1] = '\0'; // Replace newline with NUL
  } else {
    buffer[numRead] = '\0';     // Standard NUL termination
    // Only drain if we actually hit the ceiling without finding a newline
    if (numRead == MAX_READ) {
      char trash;
      while ((read(STDIN_FILENO, &trash, 1)) > 0 && trash != '\n')
        ;
    }
  }
```

### 2. Why Kerrisk wrote it "Nasty"

In Chapter 4 and 5 of TLPI, Kerrisk often presents "un-robust" code to
focus on a single concept (like the `read()` system call itself).

- **The "Trap":** By using `buffer[numRead] = '\0'`, he assumes the user
  will never type more than `MAX_READ`.
- **The "Lesson":** As you found out, `read()` is a raw system call. It
  doesn't know about strings, lines, or "leftovers." It just moves bytes.

### 3. A Sneaky Bug in your `else` branch

Look at this line in your code:
`buffer[MAX_READ] = 0;`

If `numRead` was, say, 5 (and `MAX_READ` is 20), but there was no newline
(maybe you sent data via a pipe like `echo -n "hello" | ./draft`), your
`else` executes. You would be null-terminating at index 20, but your
data ends at index 5.
**Result:** `printf` will show your data, but if there was garbage in
the buffer between index 5 and 20, you wouldn't see it—but the string's
"logic" would be broken.

**Always null-terminate at `buffer[numRead]` unless you specifically
want to overwrite the last character.**

---

### 4. The C23 Touch

Since you're using C23, you can make that "trash" loop a bit more
modern by using the `[[maybe_unused]]` attribute inside the loop if
you had to declare the variable there, or even better, just use a clear
`while` block.

\*_Would you like to see how to use `poll(2)` to check if there is more
data waiting in `stdin` so you can drain the buffer without your program
"hanging" if the user hasn't typed anything else?_
