#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>

int main(int argc, char const* argv[])
{
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9001);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int server_socket = connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    if (server_socket == -1)
    {
        printf("Error...\n");
    }
    else
    {
  
        char message[255];
        scanf("%s", message);

        send(server_socket, message, sizeof(message), 0);

    }

   return 0;
}