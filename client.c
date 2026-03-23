#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>

void connectToServer()
{
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9001);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    int status = connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    char buffer[1024];

    if (status == -1)
    {
        perror("Error...\n");
        return;
    }

    else
    {
        while (1){     
            printf("Enter message: ");
            fflush(stdout);

            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                break;
            }
            ssize_t len = strlen(buffer);
            if (len == 0){
                continue;
            }

            ssize_t total_sent = 0;
            while (total_sent < len) {
                ssize_t bytes_sent = send(client_socket, buffer + total_sent, len - total_sent, 0);
                total_sent += bytes_sent;
            }

            ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

            if (bytes_received == 0) {
                printf("Server closed the connection\n");
                break;
            }

            buffer[bytes_received] = '\0';
            printf("Echo from server: %s", buffer);
            printf(buffer);
            if (strcmp(buffer, "exit\n") == 0){
                break;
            }

        }
    }
}

int main(int argc, char const* argv[])
{
    connectToServer();
    return 0;
}