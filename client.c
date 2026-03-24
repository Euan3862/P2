#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include "client.h"

enum states current_state = unauthorised;

void generate_key_pair()
{
    
}

int connectToServer()
{
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9000);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    int status = connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    if (status == -1)
    {
        perror("Error...\n");
        return 1;
    }
    return client_socket;
}

void client_logic(int client_socket)
{
    while (1)
    {     
        switch (current_state)
        {
            case unauthorised:
                break;
            case create_new:
                //generate_key_pair();
                break;
        }

        char incoming[1024];
        char outgoing[1024];

        ssize_t bytes_received = recv(client_socket, incoming, sizeof(incoming) - 1, 0);

        incoming[bytes_received] = '\0';
        printf("Server Message: %s", incoming);
        // printf(incoming);

        printf("Enter message: ");
         fflush(stdout);

        if (fgets(outgoing, sizeof(outgoing), stdin) == NULL) {
            break;
        }
        ssize_t len = strlen(outgoing);
        if (len == 0){
            continue;
        }

        ssize_t total_sent = 0;
        while (total_sent < len) {
            ssize_t bytes_sent = send(client_socket, outgoing + total_sent, len - total_sent, 0);
            total_sent += bytes_sent;
        }

        if (strcmp(outgoing, "exit\n") == 0) { // Exit only after sending exit message to server.
            close(client_socket);
            break;
        }
        if (bytes_received == 0) {
            printf("Server closed the connection\n");
            break;
        }
    }
}


int main(int argc, char const* argv[])
{
    int client_socket = connectToServer();
    if (client_socket == 1) {
        exit(1);
    }
    client_logic(client_socket);
    return 0;
}