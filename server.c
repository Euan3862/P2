#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include <string.h>

void clientConnection()
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET; // AF_INET = TCP
    server_address.sin_port = htons(9001);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    listen(server_socket, 1);

    int client_socket = accept(server_socket, NULL, NULL); // Client socket for this specific client
    int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    while (1)
    {
     ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
     if (bytes_received < 0) {
    
     }
     if (bytes_received == 0){
        printf("Client Disconnected");
        break;
     }

     buffer[bytes_received] = '\0';
     printf("Received :%s", buffer);

     ssize_t total_sent = 0;
     while (total_sent < bytes_received) {
        ssize_t bytes_sent = send(client_socket, buffer + total_sent, bytes_received - total_sent, 0);
        
        if (bytes_sent < 0) {
     
        }
        total_sent += bytes_sent;
     }
    }
    
}

//https://www.geeksforgeeks.org/computer-networks/simple-client-server-application-in-c/
int main(int argc, char const* argv[])
{   
    clientConnection();
    return 0;
    
}