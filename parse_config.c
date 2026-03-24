#include <stdio.h>


// https://www.geeksforgeeks.org/c/c-program-to-read-contents-of-whole-file/
int get_client_count() {
    FILE *fptr = fopen("server_config.txt", "r");
    
    // Variables for storing data
    int client_id;
    int count;
    
    // Read data of file in specific format
    while (fscanf(fptr, "%d %d", &client_id, &count) == 2) {
        //printf("Count: %d\n",count);
    }
    fclose(fptr);
    return client_id, count;
}

