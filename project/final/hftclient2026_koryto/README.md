# Final phase - High-Frequency Trading (HFT) Challenge
## Group: Q22026 Group 3

## Build

From the project root:

```bash
./build.sh
```

Or build manually with CMake:

```bash
cmake -S . -B build
cmake --build build -j8
```

The executable is created at:

```text
build/bin/hftclient2026_koryto
```

`build.sh` also copies the executable into the `hftclient2026_koryto/` directory.

## Run

Start the server first, then run:

```bash
./build/bin/hftclient2026_koryto 127.0.0.1 12345 Group3
```

Arguments:

```text
<host> <port> <team_name>
```

Example with the copied executable:

```bash
./hftclient2026_koryto/hftclient2026_koryto 127.0.0.1 12345 Group3
```

## Protocol Handling

The client connects over TCP, enables `TCP_NODELAY`, sends the team name followed by a newline, then continuously reads challenge data from the socket.

Each challenge is parsed as:

```text
challenge_id
N
A values, N*N integers
B values, N*N integers
```

The response format is:

```text
challenge_id answer
```

## Optimizations Used

- Streaming parser: the client parses numbers directly from the socket buffer without building strings or JSON objects.
- No full matrix storage: it stores only the column sums of matrix `A`.
- Algebraic checksum shortcut: instead of computing every entry of `C = A * B`, it computes the checksum directly.
- Low allocation path: the only per-challenge dynamic allocation is resizing the `A` column-sum vector for the current matrix size.
- Manual response formatting: the answer is written into a fixed-size character buffer instead of using stream formatting.
- `TCP_NODELAY`: disables Nagle's algorithm to reduce response latency for small answer messages.
- Native compiler tuning: the CMake target uses `-O2`, `-march=native`, and `NDEBUG` for GNU/Clang builds.

The checksum shortcut is based on:

```text
sum(A * B) = sum over k of column_sum(A, k) * row_sum(B, k)
```

The client therefore scans `A` once to build column sums, then scans `B` once to accumulate row sums and update the final checksum modulo `997`.

## Libraries

This client uses standard C++ and POSIX socket APIs. The target links against `nlohmann_json` through the project CMake setup, but this implementation does not use JSON parsing.

## Source Files

- `main.cpp`: TCP client, streaming parser, checksum computation, and response writer.
- `CMakeLists.txt`: build target and optimization flags.
