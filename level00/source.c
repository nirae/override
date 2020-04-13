int main(void) {

    int pass;

    puts("***********************************");
    puts("* \t     -Level00 -\t\t  *");
    puts("***********************************");
    printf("Password:");

    scanf("%d", &pass);

    if (pass == 0x149c) { /* 5276 */
        puts("\nAuthenticated!");
        system("/bin/sh");
    } else {
        puts("\nInvalid Password!");
    }

    return 0;
}