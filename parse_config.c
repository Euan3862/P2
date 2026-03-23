#include <stdio.h>

int count;
// https://www.geeksforgeeks.org/c/c-program-to-read-contents-of-whole-file/
int get_client_count() {
    FILE *fptr = fopen("server_config.txt", "r");
    
    // Variables for storing data
    char client_count[12];
    
    // Read data of file in specific format
    while (fscanf(fptr, "%s %d", client_count, &count) == 2) {
        //printf("Count: %d\n",count);
    }
    fclose(fptr);
    return count;
}

