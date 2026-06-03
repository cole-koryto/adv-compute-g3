/**
 * hftclient2026_kim — Jonathan Kim's HFT client
 * Two selectable algorithms for computing the challenge checksum:
 * "naive"  : O(N^3) — actually multiplies C = A * B (mod 997) and sums C.
 * This is the honest baseline. It is what the server itself does.
 * "fast"   : O(N^2) — never multiplies the matrices. Uses the identity
 * sum(C) = sum_{i,j} sum_k A[i][k]*B[k][j]
 * = sum_k ( sum_i A[i][k] ) * ( sum_j B[k][j] )
 * = sum_k colsum_A[k] * rowsum_B[k]   (mod 997)
 * so we only need column sums of A and row sums of B.
 * Usage:
 * hftclient2026_kim <host> <port> <team_name> [naive|fast]
 * (mode defaults to "fast")
 * Robustness note: a 128x128 challenge serializes to ~127 KB, which is larger
 * than any single recv() buffer, so we read the full message across multiple
 * recv() calls before parsing. This is the main correctness fix versus a
 * "parse one recv buffer" client.
 */

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;
using clock_type = std::chrono::steady_clock;

static constexpr int MODULO = 997;
static constexpr int RECV_CHUNK = 65536;
static constexpr int ANSWER_BUFFER_SIZE = 32;

// Streaming int reader
// Reads unsigned integers from a TCP stream, refilling from recv() as needed so
// that a single number (or the whole challenge) may span many TCP segments.
struct StreamReader {
    int sock;
    char buf[RECV_CHUNK];
    int len = 0;   // number of valid bytes in buf
    int pos = 0;   // current read offset into buf
    bool eof = false;

    explicit StreamReader(int s) : sock(s) {}

    // Returns next byte, or -1 on disconnect.
    inline int next_byte() {
        if (pos >= len) {
            len = static_cast<int>(recv(sock, buf, sizeof(buf), 0));
            pos = 0;
            if (len <= 0) { eof = true; return -1; }
        }
        return static_cast<unsigned char>(buf[pos++]);
    }

    // Reads the next unsigned integer. Returns false on disconnect.
    bool read_uint(int& out) {
        int c = next_byte();
        // Skip any leading non-digit bytes (spaces, newlines).
        while (c != -1 && !(c >= '0' && c <= '9')) c = next_byte();
        if (c == -1) return false;

        int v = 0;
        while (c >= '0' && c <= '9') {
            v = v * 10 + (c - '0');
            c = next_byte();
            if (c == -1) break;  // last token of the stream
        }
        out = v;
        return true;
    }
};

//Reply formatting
inline char* append_uint(char* p, unsigned int x) {
    char tmp[10];
    int n = 0;
    do { tmp[n++] = static_cast<char>('0' + (x % 10)); x /= 10; } while (x != 0);
    while (n > 0) *p++ = tmp[--n];
    return p;
}

// Checksum kernels
// O(N^3) baseline: build C = A*B (mod 997) row by row and sum it, mirroring
// exactly what the server does.
int checksum_naive(const vector<int>& A, const vector<int>& B, int N) {
    int checksum = 0;
    vector<int> rowC(N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) rowC[j] = 0;
        for (int k = 0; k < N; ++k) {
            int a = A[i * N + k];
            const int* brow = &B[k * N];
            for (int j = 0; j < N; ++j) {
                rowC[j] = (rowC[j] + static_cast<int>((1LL * a * brow[j]) % MODULO)) % MODULO;
            }
        }
        for (int j = 0; j < N; ++j) checksum = (checksum + rowC[j]) % MODULO;
    }
    return checksum;
}

// O(N^2) fast path: sum_k colsum_A[k] * rowsum_B[k] (mod 997).
int checksum_fast(const vector<int>& A, const vector<int>& B, int N) {
    vector<long long> colsumA(N, 0);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            colsumA[j] += A[i * N + j];
    for (int j = 0; j < N; ++j) colsumA[j] %= MODULO;

    int checksum = 0;
    for (int k = 0; k < N; ++k) {
        long long rowsumB = 0;
        const int* brow = &B[k * N];
        for (int j = 0; j < N; ++j) rowsumB += brow[j];
        rowsumB %= MODULO;
        checksum = static_cast<int>((checksum + (colsumA[k] * rowsumB) % MODULO) % MODULO);
    }
    return checksum;
}

int main(int argc, char** argv) {
    if (argc < 4) {
        cout << "Usage: " << argv[0] << " <host> <port> <team_name> [naive|fast]\n";
        return 1;
    }

    string host = argv[1];
    int port = stoi(argv[2]);
    string team = argv[3];
    bool naive = (argc >= 5 && string(argv[4]) == "naive");

    cout << "HFT Client (kim) — mode: " << (naive ? "naive O(N^3)" : "fast O(N^2)") << "\n";

    // connect to server here
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("socket"); return 1; }

    int flag = 1;
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));  // disable Nagle

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &addr.sin_addr);

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) { perror("connect"); return 1; }

    string intro = team + "\n";
    send(sock, intro.c_str(), intro.size(), 0);

    cout << "Connected to " << host << ":" << port << " — waiting for challenges...\n";

    StreamReader reader(sock);
    char answer_buffer[ANSWER_BUFFER_SIZE];
    vector<int> A, B;

    while (true) {
        int cid, N;
        if (!reader.read_uint(cid)) { cout << "Disconnected from server.\n"; break; }
        if (!reader.read_uint(N))   { cout << "Disconnected from server.\n"; break; }

        // Read the full challenge into arrays
        auto t_read0 = clock_type::now();
        int total = N * N;
        A.assign(total, 0);
        B.assign(total, 0);
        bool ok = true;
        for (int t = 0; t < total && ok; ++t) ok = reader.read_uint(A[t]);
        for (int t = 0; t < total && ok; ++t) ok = reader.read_uint(B[t]);
        if (!ok) { cout << "Disconnected mid-challenge.\n"; break; }
        auto t_read1 = clock_type::now();

        // Checksum computation
        auto t_c0 = clock_type::now();
        int checksum = naive ? checksum_naive(A, B, N) : checksum_fast(A, B, N);
        auto t_c1 = clock_type::now();

        // pack/send-
        char* p = answer_buffer;
        p = append_uint(p, static_cast<unsigned int>(cid));
        *p++ = ' ';
        p = append_uint(p, static_cast<unsigned int>(checksum));
        *p++ = '\n';
        send(sock, answer_buffer, static_cast<size_t>(p - answer_buffer), 0);

        auto read_us    = std::chrono::duration_cast<std::chrono::microseconds>(t_read1 - t_read0).count();
        auto compute_us = std::chrono::duration_cast<std::chrono::microseconds>(t_c1 - t_c0).count();
        cerr << "cid=" << cid << " N=" << N
        << " checksum=" << checksum
        << " | read=" << read_us << "us"
        << " compute=" << compute_us << "us" << endl;
    }

    close(sock);
    return 0;
}
