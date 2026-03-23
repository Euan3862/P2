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
    server_address.sin_addr.s_addr = INADDR_ANY;

    int status = connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    if (status == -1)
    {
        perror("Error...\n");
        return;
    }

    else
    {
        while (1){
            char message[255]; // Reserve space for 1024 bytes, contiguously stored in memory.        
              memset(message, 0, sizeof(message));
            scanf("%s", message);
            send(client_socket, message, sizeof(message), 0);

            int bytes = recv(client_socket, message, sizeof(message) - 1, 0);
            if (bytes <= 0){
                break;
            }

            message[bytes] = '\0';
            printf("%s\n", message);
          
        }
    }
}

int main(int argc, char const* argv[])
{
    connectToServer();
    return 0;
}