#include <stdio.h>
#include <string.h>

int main (void) {

    unsigned char* test = "á";

    for (int i = 0; i < strlen(test); i++){
        printf("%X", test[i]);
    }
    printf("\n");

    return 0;
}