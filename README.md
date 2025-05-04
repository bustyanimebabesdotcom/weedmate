# WeedMate

*Your friendly neighborhood weed pricing CLI app, my starter project for my C learning journey*

---

## What is WeedMate?

WeedMate is a terminal-based C application designed to make learning how to build CLI apps more fun ang engaging.
The code is meant to be noob friendly, and readable. Comments for clarity are provided, mostly around less noob friendly function calls.

It's also a clean, modular project demonstrating good practices for beginners learning modern C:

- Proper multi-file organization
- Clean headers, structs, and macros
- Terminal UI with alternate-screen buffer
- Input validation and handling
- Basic encapsulation

Do note, I'm a noob, so you may find bugs, or areas that can be improved. I'm by no means a professional programmer.

---

## 📥 Custom Input Library

WeedMate comes with a **fully modular input library** that replaces C's cursed `scanf()` with a safer, no-bullshit alternative.
It's designed for beginners who understand the issues with `scanf()` but are not yet ready to write their own parsing logic from scratch.

Features:
- Uses `fgets` + `strtol` / `strtof` / `strtod` internally — no garbage left in stdin.
- Graceful handling of invalid input with helpful error messages.
- Rejects junk like `"abc123"` when expecting numbers.
- Cleanly parses the most common types of input.
- Optional filtered character input (`getCharInputFiltered("yn")` style).
- Easy to extend, dead simple to read.

`scanf()` can chordle my balls. This is for people who don’t want to spend three hours debugging why their `scanf("%d")` keeps skipping lines.

---

## 🛠️ How to Build

You need `cmake` and `make`:

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

## License

This project is proudly licensed under the [Unlicense](LICENSE). Use it, sell it, put your dick in it. I don't give a fuck.

---

## 💡 Disclaimer

WeedMate is intended for use in whatever-the-fuck-you-want. Take my code, put your name on it, sell it to the government, whatever you feel like.
This is free, no strings attached, and I’m not responsible if it gets you rich or gets you in jail. 