#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <sstream>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MODULO 997
#define BUFFER_SIZE 65536
#define ANSWER_BUFFER_SIZE 32

using namespace std;
using clock_type = std::chrono::steady_clock;

void deserializePrintPayload(const char* buffer, int bytesReceived)
{
    string msg(buffer, bytesReceived);
    string cid;
    int matrixSize;
    vector<vector<int>> A;
    vector<vector<int>> B;

    stringstream ss(msg);

    getline(ss, cid);

    if (!(ss >> matrixSize)) {
        throw runtime_error("Failed to read matrix size");
    }

    A.assign(matrixSize, vector<int>(matrixSize));
    B.assign(matrixSize, vector<int>(matrixSize));


    // Read A values
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            if (!(ss >> A[i][j])) {
                throw runtime_error("Failed to read matrix A");
            }
        }
    }

    // Read B values
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            if (!(ss >> B[i][j])) {
                throw runtime_error("Failed to read matrix B");
            }
        }
    }

    cout << "cid: " << cid << endl;
    cout << "matrix size: " << matrixSize << endl;

    cout << "Matrix A:" << endl;
    for (const auto& row : A)
    {
        for (int val : row)
        {
            cout << val << " ";
        }
        cout << endl;
    }

    cout << "Matrix B:" << endl;
    for (const auto& row : B)
    {
        for (int val : row)
        {
            cout << val << " ";
        }
        cout << endl;
    }
}

inline char* append_uint(char* p, unsigned int x)
{
    char tmp[10];
    int n = 0;

    do
    {
        tmp[n++] = static_cast<char>('0' + (x % 10));
        x /= 10;
    }
    while (x != 0);

    while (n > 0)
    {
        *p++ = tmp[--n];
    }

    return p;
}

int computeChecksumFast(const char* buffer, int bytesReceived, char* answer_buffer){
    // Reads through buffer from server and computes checksum inplace
    int checksum = 0;
    int cid = 0;
    int N = 0;
    int buffer_it = 0;
    int temp_val;

    // Parse cid
    while (buffer[buffer_it] >= '0' && buffer[buffer_it] <= '9')
    {
        cid = cid * 10 + (buffer[buffer_it] - '0');
        buffer_it++;
    }
    buffer_it++;    // Skip newline after cid

    // Parse N
    while (buffer[buffer_it] >= '0' && buffer[buffer_it] <= '9')
    {
        N = N * 10 + (buffer[buffer_it] - '0');
        buffer_it++;
    }
    buffer_it++;    // Skip newline after N

    int total = N * N;
    std::vector<int> a_col_sums(N, 0);

    // Parse A and compute col sums
    int col;
    int count;
    for (count = 0; count < total; count++)
    {
        temp_val = 0;
        while (buffer[buffer_it] >= '0' && buffer[buffer_it] <= '9')
        {
            temp_val = temp_val * 10 + (buffer[buffer_it] - '0');
            buffer_it++;
        }
        buffer_it++;    // Skip space after A_{i, j}

        col = count % N;
        a_col_sums[col] = (a_col_sums[col] + temp_val) % MODULO;
    }
    buffer_it++;    // Skip newline after A

    // Parse B and compute checksum
    int row;
    int row_sum_b;
    for (row = 0; row < N; row++)
    {
        row_sum_b = 0;
        for (col = 0; col < N; col++)
        {
            temp_val = 0;
            while (buffer[buffer_it] >= '0' && buffer[buffer_it] <= '9')
            {
                temp_val = temp_val * 10 + (buffer[buffer_it] - '0');
                buffer_it++;
            }
            buffer_it++;    // Skip space after B_{i, j}

            row_sum_b = (row_sum_b + temp_val) % MODULO;
        }

        // Updates checksum with new row data from B
        checksum = (checksum + static_cast<int>((1LL * a_col_sums[row] * row_sum_b) % MODULO)) % MODULO;
    }

    // Pack answer into answer buffer
    char* p = answer_buffer;
    p = append_uint(p, static_cast<unsigned int>(cid));
    *p++ = ' ';
    p = append_uint(p, static_cast<unsigned int>(checksum));
    *p++ = '\n';

    return static_cast<int>(p - answer_buffer);
}

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        cout << "Usage: " << argv[0] << " <host> <port> <team_name>\n";
        return 1;
    }

    string host = argv[1];
    int port = stoi(argv[2]);
    string team = argv[3];

    cout << "HFT Client\n";

    // --- Connect to server ---
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    int flag = 1;
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
    if (sock < 0)
    {
        perror("socket");
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &addr.sin_addr);

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        return 1;
    }

    // Send team name
    string intro = team + "\n";
    send(sock, intro.c_str(), intro.size(), 0);

    cout << "Connected to server at " << host << ":" << port << "\n";
    cout << "Waiting for challenges...\n";

    char buffer[BUFFER_SIZE];
    char answer_buffer[ANSWER_BUFFER_SIZE];
    int answer_len;
    int n;
    while (true)
    {
        n = recv(sock, buffer, sizeof(buffer)-1, 0);
        if (n <= 0)
        {
            cout << "Disconnected from server.\n";
            break;
        }
        buffer[n] = '\0';

        deserializePrintPayload(buffer);

        // Send Checksum answer
        auto t0 = clock_type::now();
        answer_len = computeChecksumFast(buffer, answer_buffer);
        auto t1 = clock_type::now();
        send(sock, answer_buffer, answer_len, 0);
        auto t2 = clock_type::now();

        auto compute_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
        auto send_ns    = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
        auto total_ns   = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t0).count();

        cerr << "compute/pack ns: " << compute_ns
                  << " send ns: " << send_ns
                  << " total ns: " << total_ns
                  << std::endl;

        cout << "Sent answer " << answer_buffer;
    }

    close(sock);
    return 0;
}
