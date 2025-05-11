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
- Uses `fgets`, and sanitizes stdin from garbage.
- Basic error handling and logging.
- Rejects garbage input.
- Optional filtered character input (`getCharInputFiltered("yn")`).
- Easy to extend, easy to read.

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

---

## How to Build

You need `cmake` and `make` or whatever other shit you use ( like `ninja` if you're based ):

```bash
mkdir build
cd build
cmake ..
make
```
To build debug or release builds, replace the `cmake ..` command with one of the commands below

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

Then run the executable:

```bash
./weedmate
```

---

## Disclaimer

This project is intended for use in whatever-the-fuck-you-want.
Take my code, put your name on it, sell it to North Korea, idgaf.
This is free, no strings attached.