struct clientSession
{
    int socket_fd;
    char alias[64];
    int authenticated;
};