#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include <string.h>


//https://www.geeksforgeeks.org/computer-networks/simple-client-server-application-in-c/
int main(int argc, char const* argv[])
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    char server_msg[255] = "Hello Client\n";

    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET; // AF_INET = TCP
    server_address.sin_port = htons(9001);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    listen(server_socket, 1);
    int client_socket = accept(server_socket, NULL, NULL);

   
    char msg[128];
    while (1)
    {   
        recv(client_socket, msg, sizeof(msg), 0);
        printf("%s\n", msg);
        break;
    }
    //send(client_socket, server_msg, sizeof(server_msg), 0);
       
 
    return 0;
    
}