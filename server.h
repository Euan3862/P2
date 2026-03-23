struct clientSession
{
    int socket_fd;
    char alias[64];
    int authenticated;
};

enum states {
    unauthorised,
    authorised,
    login_attempt,
    create_new_1,
    create_new_2,
    create_new_3
};

#define MAX_CLIENTS 64
struct clientSession clients[MAX_CLIENTS];