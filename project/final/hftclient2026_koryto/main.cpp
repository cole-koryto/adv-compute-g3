#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>   // socket, connect, send, recv
#include <netinet/in.h>   // sockaddr_in, htons
#include <netinet/tcp.h>  // TCP_NODELAY
#include <arpa/inet.h>    // inet_pton
#include <unistd.h>       // close

#define MODULO 997
#define BUFFER_SIZE 65536
#define ANSWER_BUFFER_SIZE 32

using namespace std;

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

struct FastChecksumParser
{
    enum Stage
    {
        READ_CID,
        READ_N,
        READ_A,
        READ_B
    };

    Stage stage = READ_CID;

    int cid = 0;
    int N = 0;
    int total = 0;

    int a_count = 0;
    int b_count = 0;

    int a_col = 0;
    int b_row = 0;
    int b_col = 0;

    int checksum = 0;
    int row_sum_b = 0;

    std::vector<int> a_col_sums;

    int current_num = 0;
    bool in_number = false;

    void reset_for_next_challenge()
    {
        stage = READ_CID;

        cid = 0;
        N = 0;
        total = 0;

        a_count = 0;
        b_count = 0;

        a_col = 0;
        b_row = 0;
        b_col = 0;

        checksum = 0;
        row_sum_b = 0;

        current_num = 0;
        in_number = false;
    }

    int pack_answer(char* answer_buffer)
    {
        char* p = answer_buffer;

        p = append_uint(p, static_cast<unsigned int>(cid));
        *p++ = ' ';
        p = append_uint(p, static_cast<unsigned int>(checksum));
        *p++ = '\n';

        // Returns length of answer
        return static_cast<int>(p - answer_buffer);
    }

    bool consume_number(int x, char* answer_buffer, int& answer_len)
    {
        if (stage == READ_CID)
        {
            cid = x;
            stage = READ_N;
            return false;
        }

        if (stage == READ_N)
        {
            N = x;
            total = N * N;
            a_col_sums.assign(N, 0);

            a_count = 0;
            b_count = 0;

            a_col = 0;
            b_row = 0;
            b_col = 0;

            checksum = 0;
            row_sum_b = 0;

            stage = READ_A;
            return false;
        }

        if (stage == READ_A)
        {
            a_col_sums[a_col] += x;

            a_col++;
            a_count++;

            if (a_col == N)
            {
                a_col = 0;
            }

            if (a_count == total)
            {
                stage = READ_B;
            }

            return false;
        }

        if (stage == READ_B)
        {
            row_sum_b += x;

            b_col++;
            b_count++;

            // End of this row of B
            if (b_col == N)
            {
                checksum = (checksum + static_cast<int>((1LL * a_col_sums[b_row] * row_sum_b) % MODULO)) % MODULO;

                row_sum_b = 0;
                b_col = 0;
                b_row++;
            }

            // End of full challenge
            if (b_count == total)
            {
                answer_len = pack_answer(answer_buffer);

                reset_for_next_challenge();

                return true;
            }

            return false;
        }

        return false;
    }

    void feed(const char* buffer, int bytesReceived, int sock, char* answer_buffer)
    {
        char c;
        int answer_len;
        bool complete;
        for (int i = 0; i < bytesReceived; i++)
        {
            c = buffer[i];

            if (c >= '0' && c <= '9')
            {
                current_num = current_num * 10 + (c - '0');
                in_number = true;
            }
            else
            {
                if (in_number)
                {
                    answer_len = 0;

                    complete = consume_number(current_num, answer_buffer, answer_len);

                    current_num = 0;
                    in_number = false;

                    if (complete)
                    {
                        send(sock, answer_buffer, answer_len, 0);
                    }
                }
            }
        }
    }
};

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
    if (sock < 0)
    {
        perror("socket");
        return 1;
    }

    int flag = 1;
    if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag)) < 0)
    {
        perror("setsockopt TCP_NODELAY");
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) != 1)
    {
        cerr << "Invalid IPv4 address: " << host << '\n';
        close(sock);
        return 1;
    }

    if (connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
    {
        perror("connect");
        close(sock);
        return 1;
    }

    // Send team name
    string intro = team + "\n";
    send(sock, intro.c_str(), intro.size(), 0);

    cout << "Connected to server at " << host << ":" << port << "\n";
    cout << "Waiting for challenges...\n";

    char buffer[BUFFER_SIZE];
    char answer_buffer[ANSWER_BUFFER_SIZE];
    int n;
    FastChecksumParser parser;
    while (true)
    {
        n = recv(sock, buffer, sizeof(buffer), 0);

        if (n <= 0)
        {
            cout << "Disconnected from server.\n";
            break;
        }

        parser.feed(buffer, n, sock, answer_buffer);
    }

    close(sock);
    return 0;
}
