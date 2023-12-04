#include <iostream>
#include <cstring>
#include <cmath>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int private_key = 4;//a
    int alpha = 7; //g
    int q = 23; //p
    std::string public_key = std::to_string((int)pow(alpha, private_key) % q);

    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket\n";
        return -1;
    }

    // Specify the server address and port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Use the server's IP address
    serverAddress.sin_port = htons(8082); // Use the same port as the server

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error connecting to server\n";
        return -1;
    }

    std::cout << "Connected to server\n";

    // Exchange public keys with the server
    std::cout << "bob : " << public_key << std::endl;
    char buffer[1024];
    strcpy(buffer, public_key.c_str());
    if (send(clientSocket, buffer, strlen(buffer), 0) == -1) {
        std::cerr << "Error sending public key to server\n";
        return -1;
    }

    // Receive the other client's public key from the server
    memset(buffer, 0, sizeof(buffer));
    if (recv(clientSocket, buffer, sizeof(buffer), 0) == -1) {
        std::cerr << "Error receiving public key from server\n";
        return -1;
    }

    // Calculate the shared secret
    int other_public_key = atoi(buffer);
    std::cout << other_public_key << std::endl;
    int shared_secret = (int)pow(other_public_key, private_key) % q;
    std::cout << "Shared secret = " << shared_secret << std::endl;

    

    // Close the socket
    close(clientSocket);

    return 0;
}
