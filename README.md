# WeedMate

*The stupidest concept for a CLI app ever made.*

---

## What is WeedMate?

WeedMate is a terminal-based C application designed to make learning how to build CLI apps more fun.
Code is meant to be readable and noob friendly.

It's also a clean, modular project demonstrating good practices for beginners learning modern C:

- Organized multi-file layout
- Terminal UI with alternate-screen buffer
- Input validation and handling
- Basic encapsulation

---

## Custom Input Library

weedmate comes with a **fully modular input library** that replaces C's shitty `scanf()` with a safer alternative.

Features:
- Uses `getchar()` in a byte for byte input loop, giving us airtight control. no libc roulette.
- Basic error handling and logging.
- Garbage-resistant — rejects malformed input.
- Filtered character input support: getCharInputFiltered("yn") lets you restrict responses with zero effort.
- EModular and readable — drop-in safe I/O that won’t stab you in production.

`scanf()` can chordle my balls.

---

## Save File Support

weedmate supports persistent strain names, and prices.

- Save file path: `.weedrc` (created in same directory as binary)
- ini style formatting

### Behavior

- On startup, if `.weedrc` exists, program loads strain names and prices from file.
- If line exceeds max length, file is considered corrupt and deleted.
- Updating prices and names update save file.

You can let the program handle it, or manually edit it.

## CLI Arguments

WeedMate supports a few **standard CLI arguments**.

### Usage

```bash
./weedmate [OPTION]
```

### Options

- `-r`, `--remove`  
  Delete save file.

- `-c`, `--create`  
  Create save file.
  
- `-h`, `--help`  
  Show help message.

- `-v`, `--version`  
  Print current version, license, and author.


---

## How to Build

You need a C compiler, `cmake` and `make`.
The build script and Makefile defaults to Ninja and Clang (gcc on ARM) with build type set to release.
I recommend you use Ninja, as it has faster buildtimes.
To build the project, simply run:

```bash
make

```
This will run the interactive script on the first run.
Once you've ran it once and build folder has been made, you can simply run `make` again to build normally.
This has all been set up to be as easy as possbile for anyone to use.

For non-interactive build:

```bash
make build

```

For debug build:

```bash
make debug
```

The script also generates 'compile_commands.json' for 'clangd'.

Then run the executable:

```bash
./weedmate
```

---

## Disclaimer

This project is intended for use in whatever-the-fuck-you-want.
Take my code, put your name on it, sell it to North Korea, idgaf.
This is free, no strings attached.