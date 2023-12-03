#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    double p = 13;
    double g = 6;
    double private_key = 4;
    std::string public_key = std::to_string(fmod(pow(g, private_key), p));
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);

   
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8888);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) == -1) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server waiting for incoming connections...\n";

    
    client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
    if (client_socket == -1) {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }

   
    char buffer[1024] = {0};
    recv(client_socket, buffer, sizeof(buffer), 0);
    int public_key_from_client = atoi(buffer);
    int shared_secret = fmod(pow(public_key_from_client, private_key), p);
    std::cout << "shared secret: " << shared_secret << "\n";

    
    const char* response = public_key.c_str();
    send(client_socket, response, strlen(response), 0);

    
    close(server_socket);
    close(client_socket);

    return 0;
}
