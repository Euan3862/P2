#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include "server.h"
#include "parse_config.h"

int client_id, client_count;
 
//https://www.geeksforgeeks.org/computer-networks/simple-client-server-application-in-c/
//https://www.geeksforgeeks.org/computer-networks/tcp-and-udp-server-using-select/

/**
 * State machine to manage server responses to client based on clients messages.
 * 
 * Return Type: char * - The message string (buffer)
 * 
 * Params: char *outgoing - The buffer to be updated
 *       : enum states current_state- Current state of client
 */
char * server_message(char *outgoing, enum states current_state)
{
    switch (current_state) {
        case unauthorised:
            strcpy(outgoing,"Enter - \"login\" or \"create\" or \"back\" \n");
            break;
        case login_attempt:
            strcpy(outgoing, "Enter Public Key\n");
            break;
        case create_new_1:
            strcpy(outgoing, "Enter Alias\n");
            break;
        case create_new_2: {
            char *alias_msg = "Your Alias is: ";
            strcpy(outgoing, alias_msg);
            strcpy(outgoing + strlen(alias_msg), clients[0].alias);
            break;
            }
        case hub: {
            char *hub_msg = "You are already at the start!\n";
            strcpy(outgoing, hub_msg); // Copy hub_msg into outgoing buffer
            //current_state = unauthorised;
            }
        };
    return outgoing;
}


enum states server_state_logic(char *incoming, enum states current_state)
{    
    if (current_state == create_new_1) {
        strcpy(clients[0].alias, incoming);
        current_state = create_new_2;
    }
    if ((strcmp(incoming, "create\n") == 0) && ((current_state == unauthorised) || (current_state == hub))) {
        current_state = create_new_1;
    } 
    if ((strcmp(incoming, "login\n") == 0) && ((current_state == unauthorised) || (current_state == hub))) {                                                  
        current_state = login_attempt;
    }
    if (strcmp(incoming, "exit\n") == 0) {
        printf("Client has terminated connection\n");
        current_state = disconnect;
    }
    if (strcmp(incoming, "back\n") == 0) {
        current_state = unauthorised;
    }
    if ((strcmp(incoming, " back\n") && current_state == unauthorised)) {
        current_state = hub;
    }
    if (current_state == hub) {
        current_state = unauthorised;
    }
    return current_state;
}
// PRINT OUT CURRENT STATE AND FIND WHY BACK MESSAGE NOT DISPLAYING

void* server_engine(int client_socket)
{
    char outgoing[BUFFER_SIZE];
    char incoming[BUFFER_SIZE];
    enum states current_state = unauthorised;

    while (1)
    {
        memset(outgoing, 0, BUFFER_SIZE);
        strcpy(outgoing, server_message(outgoing, current_state)); // Checking server state machine to provide the correct server message to client.
        // SEND
        send(client_socket, outgoing, strlen(outgoing), 0);
        // RECEIVE
        ssize_t bytes_received = recv(client_socket, incoming, sizeof(incoming) - 1, 0);

        if (bytes_received <= 0){
            printf("Client Disconnected\n");
            break;
        }

        incoming[bytes_received] = '\0';
        printf("Received: %s", incoming);

        current_state = server_state_logic(incoming, current_state);
        if (current_state == disconnect) {
            break;
        }

    }

    close(client_socket);
}


void client_connection()
{
    int listenfd, connfd;
    socklen_t len;
    struct sockaddr_in cliaddr, server_address;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9000);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(listenfd, (struct sockaddr*)&server_address, sizeof(server_address));
    listen(listenfd, 10);

    while (1)
    {
        len = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);

        if (fork() == 0) {
            // child process
            // Connfd = connection file descriptor
            close(listenfd); // Close listening socket as child only needs its connfd client socket
            server_engine(connfd);
            exit(0);
        }

        // parent process
        close(connfd);
    }
}


int main(int argc, char const* argv[])
{   
    printf("CC: %d\n", client_count);
    client_connection();
    return 0;
}