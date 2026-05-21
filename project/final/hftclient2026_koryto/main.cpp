#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <sstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MATRIX_SIZE 128
#define MODULO 997

using namespace std;

void deserializePayload(
    const string& payload,
    string& cid,
    int& matrixSize,
    vector<vector<int>>& A,
    vector<vector<int>>& B
) {
    stringstream ss(payload);

    // Read cid
    getline(ss, cid);

    // Read matrix size
    ss >> matrixSize;

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
}

int main(int argc, char** argv) {
    if (argc < 4) {
        cout << "Usage: " << argv[0] << " <host> <port> <team_name>\n";
        return 1;
    }

    string host = argv[1];
    int port = stoi(argv[2]);
    string team = argv[3];

    cout << "HFT Client\n";

    // --- Connect to server ---
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &addr.sin_addr);

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        return 1;
    }

    // Send team name
    string intro = team + "\n";
    send(sock, intro.c_str(), intro.size(), 0);

    cout << "Connected to server at " << host << ":" << port << "\n";
    cout << "Waiting for challenges...\n";

    char buffer[65536];

    while (true) {
        int n = recv(sock, buffer, sizeof(buffer)-1, 0);
        if (n <= 0) {
            cout << "Disconnected from server.\n";
            break;
        }

        buffer[n] = '\0';
        string msg(buffer);

        string cid;
        int matrixSize;
        vector<vector<int>> A;
        vector<vector<int>> B;

        deserializePayload(msg, cid, matrixSize, A, B);

        cout << "cid: " << cid << endl;
        cout << "matrix size: " << matrixSize << endl;

        cout << "Matrix A:" << endl;
        for (const auto& row : A) {
            for (int val : row) {
                cout << val << " ";
            }
            cout << endl;
        }

        cout << "Matrix B:" << endl;
        for (const auto& row : B) {
            for (int val : row) {
                cout << val << " ";
            }
            cout << endl;
        }

        // Pretend to compute something
        this_thread::sleep_for(chrono::milliseconds(5));

        // Always send wrong answer
        string fake = "0 0\n";
        send(sock, fake.c_str(), fake.size(), 0);

        cout << "[DEBUG] Sent fake answer\n";
    }

    close(sock);
    return 0;
}
