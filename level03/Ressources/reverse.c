#include <stdio.h>
#include <string.h>

int main(void) {
    char buffer[17] = "Q}|u`sfg~sf{}|a3";
    char tmp[17];

    // 21 is the range in the function "test"
    for (int i = 0; i <= 21; i++) {

        bzero(&tmp, 17);

        // Do the same thing of the "decrypt" function
        for (int y = 0; y < 17; y++) {
            tmp[y] = buffer[y] ^ i; 
        }
        printf("%s\n", tmp);

        if (strncmp("Congratulations!", tmp, 16) == 0) {
            printf("Found! The password is %d\n", 322424845 - i);
            break;
        }
    }

    return 0;
}