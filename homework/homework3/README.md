# Homework 3 – Manual Memory Management & RAII

## Build

```bash
mkdir -p build && cd build
cmake ..
cmake --build .
```

Binaries are output to `build/`: `part1`, `part2`, `part3`.

> ASan is enabled by default in `CMakeLists.txt`. It will report memory errors (double-delete, use-after-free, buffer overflows) at runtime.

## Run

```bash
./build/part1
./build/part2
./build/part3
```

## Check for Memory Leaks (macOS)

ASan does not report leaks on macOS by default. Use the native `leaks` tool:

```bash
MallocStackLogging=1 ./build/part1
leaks --atExit -- ./build/part1
```
