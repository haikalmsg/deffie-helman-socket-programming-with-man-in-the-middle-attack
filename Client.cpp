#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <math.h>

using namespace std;
int main() {
    #include <cmath>

    double p = 13;
    double g = 6;
    double private_key = 5;
    string public_key = to_string(fmod(pow(g, private_key), p));
    int client_socket;
    struct sockaddr_in server_address;


    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8888);

    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        exit(EXIT_FAILURE);
    }


    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    
    const char* message = public_key.c_str();
    send(client_socket, message, strlen(message), 0);

    // Receive response from server
    char buffer[1024] = {0};
    recv(client_socket, buffer, sizeof(buffer), 0);
    int public_key_from_server = atoi(buffer);
    int shared_secret = fmod(pow(public_key_from_server, private_key), p);

    std::cout << "shared secret: " << shared_secret << "\n";

    close(client_socket);

    return 0;
}
