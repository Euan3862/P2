#include <pthread.h>

enum states {
    unauthorised,
    authorised,
    login_attempt,
    create_new_1,
    create_new_2,
    create_new_3,
    disconnect,
    hub
};

struct clientSession
{
    int socket_fd;
    char alias[64];
    enum states current_state;
    int in_use;
    pthread_t thread_id;

};


#define MAX_CLIENTS 64
#define BUFFER_SIZE 1024
//pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
struct clientSession clients[MAX_CLIENTS];