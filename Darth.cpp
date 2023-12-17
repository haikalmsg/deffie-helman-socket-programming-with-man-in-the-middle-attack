#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int random1 =  5; //XD1
    int random2 = 1; //XD2
    int alpha =7;
    int q = 23;
    std::string public_key1 = std::to_string((int)pow(alpha, random1) % q); //YD1
    std::string public_key2 = std::to_string((int)pow(alpha, random2) % q); //YD2
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket\n";
        return -1;
    }

    // Bind the socket to a specific address and port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8082); // You can use any available port

    int bindResult = bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (bindResult == -1) {
        std::cerr << "Error binding socket\n";
        close(serverSocket); // Close the socket before returning
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 2) == -1) {
        std::cerr << "Error listening on socket\n";
        close(serverSocket); // Close the socket before returning
        return -1;
    }

    std::cout << "Server listening on port 8080...\n";

    // Accept the first client
    sockaddr_in clientAddress;
    socklen_t clientSize = sizeof(clientAddress);
    int clientSocket1 = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientSize);
    std::cout << "Client 1 connected\n";

    // Accept the second client
    int clientSocket2 = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientSize);
    std::cout << "Client 2 connected\n";

    // Key exchange
    char buffer[1024];

    // Receive public key from client 1
    memset(buffer, 0, sizeof(buffer));
    if (recv(clientSocket1, buffer, sizeof(buffer), 0) == -1) {
        std::cerr << "Error receiving public key from client 1\n";
        close(clientSocket1); // Close the client socket before returning
        close(clientSocket2); // Close the other client socket before returning
        close(serverSocket); // Close the server socket before returning
        return -1;
    }
    int other_public_key1 = atoi(buffer);
    std::cout << "pc 1 : "<< other_public_key1 << std::endl;
    std::string Ka = std::to_string((int)pow(other_public_key1, random2) % q); //YD1
    strcpy(buffer, public_key1.c_str());
    // Send client 1's public key to client 2
    std::cout << "key a : "<< Ka << std::endl;
    send(clientSocket2, buffer, strlen(buffer), 0);

    // Receive public key from client 2
    memset(buffer, 0, sizeof(buffer));
    if (recv(clientSocket2, buffer, sizeof(buffer), 0) == -1) {
        std::cerr << "Error receiving public key from client 2\n";
        close(clientSocket1); // Close the client socket before returning
        close(clientSocket2); // Close the other client socket before returning
        close(serverSocket); // Close the server socket before returning
        return -1;
    }
    int other_public_key2 = atoi(buffer);
    std::cout << "pc 2 : "<< other_public_key2 << std::endl;
    std::string Kb = std::to_string((int)pow(other_public_key2, random1) % q); //YD1
    std::cout << "key b : "<< Kb << std::endl;
    strcpy(buffer, public_key2.c_str());


    // Send client 2's public key to client 1
    send(clientSocket1, buffer, strlen(buffer), 0);

    // Key exchange complete

    // Close sockets
    close(clientSocket1);
    close(clientSocket2);
    close(serverSocket);

    return 0;
}
