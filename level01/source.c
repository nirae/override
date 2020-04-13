char *a_user_name;

int verify_user_name(void) {
    puts("verifying username....\n");
    return (strncmp(a_user_name, "dat_wil", 7));
}

int verify_user_pass(char *pass) {
    return (strncmp(pass, "admin", 5));
}

int main(void) {
    char pass[16]; /* esp+0x1c */
    int ret; /* esp+0x5c */

    bzero(pass, 16);
    ret = 0;

    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");

    fgets(&a_user_name, 256, stdin);
    ret = verify_user_name();
    if (ret != 0) {
        puts("nope, incorrect username...\n");
        return 1;
    }
    puts("Enter Password: ");
    /* Overflow */
    fgets(&pass, 100, stdin);
    ret = verify_user_pass(pass);
    if (ret == 0) {
        puts("nope, incorrect password...\n");
        return 1;
    }
    if (ret == 0) {
        return 0;
    }
    return 0;
}