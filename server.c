#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include <string.h>
#include "server.h"
#include "parse_config.h"

int client_id, client_count;
enum states current_state = unauthorised;



/**
 * Dealing with the core server logic, and possible client branches ie, logging in or creating a new account.
 */
void server_logic(int client_socket)
{
    int BUFFER_SIZE = 1024;
    char outgoing[BUFFER_SIZE];
    char incoming[BUFFER_SIZE];

    while (1)
    {
        switch (current_state) {
            case unauthorised:
                strcpy(outgoing,"Enter exactly - \"login\" or \"create\" [New Account]\n");
                break;
            case login_attempt:
                strcpy(outgoing, "Enter Public Key\n");
                break;
            case create_new_1:
                strcpy(outgoing, "Enter Alias\n");
                break;
            case create_new_2:
                char *alias_msg = "Your Alias is: ";
                strcpy(outgoing, alias_msg);
                strcpy(outgoing + strlen(alias_msg),  clients[0].alias); //Concatenating server message with alias value.
                break;
        };

        // SENDING TO CLIENT
        ssize_t len = strlen(outgoing);
        if (len == 0){
            continue;
        }

        ssize_t total_sent = 0;
        while (total_sent < len) {
            ssize_t bytes_sent = send(client_socket, outgoing + total_sent, len - total_sent, 0);
            total_sent += bytes_sent;
        }

        //RECEIVING FROM CLIENT
        ssize_t bytes_received = recv(client_socket, incoming, sizeof(incoming) - 1, 0);

        if (bytes_received == 0){
            printf("Client Disconnected\n");
            break;
        }
        incoming[bytes_received] = '\0';
        printf("Received: %s", incoming);


        //printf("Current State: %s" , currentState);
        if (current_state == create_new_1) { // Must be before check for "create" incoming.
            const char *alias = incoming;
            strcpy(clients[0].alias, alias);
           
            //printf("Client: %s", clients[0].alias);
            current_state = create_new_2;
        }
        if ((strcmp(incoming, "create\n") == 0) && (current_state == unauthorised)) {
            current_state = create_new_1;
        } 
        if ((strcmp(incoming, "login\n") == 0) && (current_state == unauthorised)) {      // Newline is included as part of the string                                                   
            current_state = login_attempt;           // before null terminator so it must be included for comparison.
        }
        if ((strcmp(incoming, "exit\n") == 0)) {
            printf("Client has terminated connection\n");
            close(client_socket);
            break;
        }

        // Need to act functionality to go back a state
    }
}


/**
 * Initial Client connection to the server
 */
int client_connection()
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET; // AF_INET = TCP
    server_address.sin_port = htons(9000);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    listen(server_socket, 1);

    while (1)
    {
        int client_socket = accept(server_socket, NULL, NULL); // Client socket for this specific client
        if (client_socket < 0) {
            perror("accept");
            continue;
        }

        pthread_mutex_lock(&clients_mutex);

        int slot = -1;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (!clients[i].in_use) {
                slot = i;
                break;
            }
        }

        if (slot == -1) {
            pthread_mutex_unlock(&clients_mutex);
            close(client_socket);
            continue;
        }

        clients[slot].socket_fd = client_socket;
        clients[slot].current_state = unauthorised;
        clients[slot].in_use = 1;
        clients[slot].alias[0] = '\0';

        pthread_mutex_unlock(&clients_mutex);

        if (pthread_create(&clients[slot].thread_id, NULL, server_logic, &clients[slot]) != 0) {
            perror("pthread_create");

            pthread_mutex_lock(&clients_mutex);
            clients[slot].in_use = 0;
            pthread_mutex_unlock(&clients_mutex);
            close(client_socket);
            continue;
        }

        pthread_detach(clients[slot].thread_id);

        
    }

}


//https://www.geeksforgeeks.org/computer-networks/simple-client-server-application-in-c/
int main(int argc, char const* argv[])
{   
    client_id, client_count = get_client_count();
    printf("CC: %d\n", client_count);
    int client_socket = client_connection();
    server_logic(client_socket);
    return 0;
    
}