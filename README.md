# WeedMate

*Your friendly neighborhood weed pricing CLI app, written purely to sharpen C skills and simulate sketchy cannabis economics.*

---

## What is WeedMate?

WeedMate is a terminal-based C application designed to simulate managing a cannabis dispensary. Adjust prices, browse your "strains," and immerse yourself in a CLI environment that's more educational than ethical.

It's also a clean, modular project demonstrating good practices for beginners learning modern C:

- Proper multi-file organization
- Clean headers, structs, and macros
- Terminal UI with alternate-screen buffer
- Input validation and handling
- Basic encapsulation

Do note, I'm not as of yet qualified to know whether any of this is true. Take it with a grain of salt.

---

## 📥 Custom Input Library

WeedMate comes with a **fully modular input library** that replaces C's cursed `scanf()` with a safer, no-bullshit alternative. It's designed for beginners who are sick of segfaults but not ready to write their own parsing logic from scratch.

Features:
- Uses `fgets` + `strtol` / `strtof` / `strtod` internally — no garbage left in stdin.
- Graceful handling of invalid input with helpful error messages.
- Rejects junk like `"abc123"` when expecting numbers.
- Cleanly parses `int`, `unsigned`, `float`, `long`, `double`, and `char`.
- Optional filtered character input (`getCharInputFiltered("yn")` style).
- Easy to extend, dead simple to read.

`scanf()` chordle my balls. This is for people who don’t want to spend three hours debugging why their `scanf("%d")` keeps skipping lines.

---

## 🛠️ How to Build

You need `cmake` and `make`:

```bash
mkdir build
cd build
cmake ..
make
```

Then run the executable:

```bash
./weedmate
```

---

## ⚙️ Features

- **Interactive menu** — easy-to-use keyboard input.
- **Strain price adjustment** — double or halve prices instantly.
- **Budtender mode** — update strain pricing individually.
- **Error handling** — cause i don't take no prisoners.

---

## License

This project is proudly licensed under the [Unlicense](LICENSE). Use it, sell it, put your dick in it. I don't give a fuck.

---

## 💡 Disclaimer

WeedMate is intended for use in whatever-the-fuck-you-want. Take my code, put your name on it, sell it to Jeff Bezos, make a fortune—whatever you feel like. This is free, no strings attached, and I’m not responsible if it gets you rich or gets you in jail. 