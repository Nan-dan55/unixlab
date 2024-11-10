#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

const int PORT = 8080;

void handle_request(int client_socket) {
    // Simplified HTTP response
    const char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>Hello, World!</h1>";
    send(client_socket, response, strlen(response), 0);
    close(client_socket);
}

int main() {
    // Create server socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Error creating server socket" << std::endl;
        return -1;
    }

    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Bind socket to port
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Error binding to port " << PORT << std::endl;
        close(server_socket);
        return -1;
    }

    // Listen for incoming connections
    if (listen(server_socket, 10) == -1) {
        std::cerr << "Error listening on port " << PORT << std::endl;
        close(server_socket);
        return -1;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        sockaddr_in client_address{};
        socklen_t client_address_len = sizeof(client_address);
        
        // Accept incoming client connection
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
        if (client_socket == -1) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        // Handle the client request
        handle_request(client_socket);
    }

    // Close server socket
    close(server_socket);
    return 0;
}
