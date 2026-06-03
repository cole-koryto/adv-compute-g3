Unlimited Attempts Allowed
🏁 HFT Matrix Challenge: How to Win
Welcome to the High-Frequency Trading Matrix Challenge. Your mission is to build the fastest, most efficient client to compete against other teams in a real-time, latency-sensitive environment.

🎯 Objective
Every 10 seconds, the server will broadcast a matrix multiplication challenge to all connected clients. Your client must:

Receive the challenge, which includes:

A unique challenge ID

The matrix size Unexpected text node: 'N'

Two Unexpected text node: 'N \times N' matrices Unexpected text node: 'A' and Unexpected text node: 'B'

Compute the result:

Multiply the matrices

Compute the trace

Send back the result as a single integer

Do it faster than everyone else

🧠 What You’re Competing On
Latency: The server records how long it takes your client to respond after the challenge is sent.

Correctness: Only correct answers count.

Consistency: The more challenges you answer correctly and quickly, the higher your ranking.

🏆 How to Win
The leaderboard is based on:

🥇 Number of wins (fastest correct response per challenge)

⏱️ Average latency

📈 Overall ranking across all challenges

The dashboard will show:

Victory counts

Latency trends

Participation stats

Ranking over time

🛠️ What You Need to Build
You must write a TCP client that:

Connects to the server

Sends your group name upon connection

Listens for challenges

Parses the matrices

Computes the result efficiently

Sends back the answer as fast as possible

🚀 Performance Tips
To win, you’ll need to:

Use multithreading or multiprocessing to parallelize matrix multiplication

Optimize memory access and avoid unnecessary copies

Consider using libraries like:

C++: OpenMP, TBB, Eigen, BLAS

Python: NumPy, multiprocessing, Numba, JAX

Profile your code and minimize latency from parsing, computation, and I/O

🧪 Test Locally
You can simulate the server locally to test your client. Make sure your client:

Handles large matrices (e.g., 128×128 or more)

Can run continuously and handle multiple challenges

Recovers gracefully from disconnects or malformed input

📦 What You Submit
Your client source code

A README with:

Your group name

How to compile/run your client

Any optimizations or libraries used

💡 Final Advice
This is a real-time systems challenge. Think like an HFT engineer:

Every millisecond counts

Parallelism is your friend

Clean code is fast code

Good luck — may the fastest matrix win. 🧮⚡

README
HFT Project 2026 - Programming Challenge
Welcome to the 2026 High-Frequency Trading (HFT) Programming Challenge. Your mission is to build the fastest possible client capable of surviving extremely high message rates from a matrix-multiplication challenge server.

This project simulates the type of real-time, low-latency pipeline engineering used in high-frequency trading systems.

You will write:

A high-performance client (your code) You are given:
A reference server (simple)
A blast server (instructor stress-test)
A placeholder client (does nothing)
Repository Structure
Here’s the Markdown version that renders cleanly but still looks like plain text — perfect for documentation or GitHub display.
You can copy‑paste this directly into your README.md file.

HFTProject2026/
|-- start_all_clients.sh <-- start many clients at the same time
|-- CMakeLists.txt
|-- build.sh
|-- README.md
|
|-- hftserver2026/
|     |-- CMakeLists.txt
|     |-- main.cpp
|
|-- hftclient2026/
|     |-- CMakeLists.txt
|     |-- main.cpp   <-- placeholder client (you replace this)
|
|-- tools/
|     |-- CMakeLists.txt
|     |-- blast_server.cpp   <-- stress-test server
|     |-- client_concurrent.cpp  <-- placeholder client (you replace this)
|
|-- logs/
The Challenge Protocol
The server repeatedly sends matrix multiplication challenges.

Each challenge has the following format:

challenge_id
N
A (N*N integers)
B (N*N integers)
Your client must:

Parse the challenge

Compute C = A * B (mod 997)

Compute checksum = sum of all entries of C (mod 997)

Send back:

challenge_id answer

Example:

42 123
Where 42 is the challenge ID and 123 is your computed checksum.

Provided Components
(1) Reference Server (hftserver2026/)
A simple server for basic testing. Sends challenges at a moderate rate.

(2) Blast Server (tools/blast_server.cpp)
This is the stress-test server. It can send hundreds of challenges per second and uses configurable blast modes.

You will be tested against both servers.

(3) Placeholder Client (hftclient2026/)
This client:

Compiles
Connects
Prints debug messages
Sends fake answers
Does NOT solve challenges
You must replace it with your own optimized client.

Building the Project
Option A: Using build.sh

./build.sh
Option B: Manual build

cmake -S . -B build
cmake --build build -j8
Executables will appear under:

build/bin/hftserver2026
build/bin/hftclient2026/
build/bin/hftclient_concurrent
build/bin/blast_server
Running the Servers
Reference server:

./build/bin/hftserver2026
Blast server (instructor only):

./build/bin/blast_server --rate 200 --window 10 --size 128 --mode 1
Blast server options:

--rate N       challenges per second
--window MS    answer window in milliseconds
--size N       matrix dimension
--mode M       0=normal, 1=heavy, 2=ultra
Running Your Client
Template client (does nothing):

./build/bin/hftclient2026 127.0.0.1 12345 TeamA
Your real client must:

Connect to the server
Read challenges
Parse efficiently
Multiply matrices quickly
Pipeline work across threads
Send answers with minimal latency
Your Task
You must replace the placeholder client with a real, high-performance implementation.

Your client will be evaluated on:

Correctness
Latency
Throughput
Stability under blast conditions
Ability to handle overlapping challenges
Efficient use of threads and CPU cores
Using the Python dashboard
HFT Dashboard (Python + Flask + Dash)
A small web dashboard is provided to visualize the results written by the server to /tmp/results.json.

The dashboard:

Reads /tmp/results.json
Serves the raw JSON at /results
Exposes an interactive dashboard at /dashboard/
Shows:
Aggregated stats per client
Overall average latency per challenge
Per-client latency over time
Victories per client
Latency histogram
Overall average ranking per client
Raw JSON data
0. Requirements
You need Python 3 and the following packages:

pip install flask dash plotly
1. File location
Save the script as, for example:

HFTProject2026/HFTDashboard/dashboard.py
Make sure the server writes its results to:

/tmp/results.json
(the script expects RESULTS_FILE = "/tmp/results.json").

2. Running the dashboard
From the project root:

cd HFTDashboard
python dashboard.py
By default it runs on:

Host: 0.0.0.0
Port: 5001
So the URLs are:

Raw JSON: http://localhost:5001/results
Dashboard: http://localhost:5001/dashboard/
3. Typical workflow
Start your HFT server (which writes to /tmp/results.json).

Run several clients so that results accumulate.

Start the dashboard:

cd HFTDashboard
python dashboard.py
4. Open a browser and go to:
http://localhost:5001/dashboard/
5. Watch latencies, wins, and rankings update over time.
Rules
You may use:

C++17 or C++20
STL containers
Threads
std::chrono
Any algorithm you write yourself
External matrix libraries
GPU acceleration
Python or other languages
Precomputed answers
Your client must not crash under:

High message rates
Burst traffic
Overlapping challenges
Tips for Success
Use a thread pool
Use a low-lock or lock-free pipeline
Avoid string parsing overhead
Avoid unnecessary memory allocations
Use cache-friendly matrix multiplication
Consider tiling or blocking
Send answers as soon as they are ready
Measure latency continuously
Good Luck
This project is designed to simulate real-world HFT engineering constraints. The fastest, most stable clients will rise to the top.

Good luck, and may your latency be low.