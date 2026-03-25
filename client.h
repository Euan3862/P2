enum states {
    unauthorised,
    create_new
    
};

void generate_key_pair();
int connectToServer();
void client_logic(int client_socket);

#define BUFFER_SIZE 1024