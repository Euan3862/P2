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
    int sockfd; 
    struct sockaddr_in server_address; 
    int n, len; 
    // Creating socket file descriptor 
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        printf("socket creation failed"); 
        exit(0); 
    } 

    memset(&server_address, 0, sizeof(server_address)); 

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9000);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) { 
        printf("\n Error : Connect Failed \n"); 
    } 

    client_logic(sockfd);
}

void client_logic(int server_socket)
{
    char incoming[BUFFER_SIZE];
    char outgoing[BUFFER_SIZE];

    printf("\nHello Client.\nThe usage of this encrypted \"client - server - client\" messaging application is as follows.\nCreate an account and provide a public key, OR, Login to an already created account.\nYou can go to a previous step by entering \"back\".\n\n");
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
        memset(outgoing, 0, sizeof(outgoing)); 
        memset(incoming, 0, sizeof(incoming)); 

        ssize_t bytes_received = read(server_socket, incoming, sizeof(incoming) - 1);

        incoming[bytes_received] = '\0';
        printf("Server Message: %s", incoming);
        // printf(incoming);

        printf("> ");
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
            ssize_t bytes_sent = send(server_socket, outgoing + total_sent, len - total_sent, 0);
            total_sent += bytes_sent;
        }

        if (strcmp(outgoing, "exit\n") == 0) { // Exit only after sending exit message to server.
            close(server_socket);
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
    // if (client_socket == 1) {
    //     exit(1);
    // }
    // client_logic(client_socket);
    return 0;
}