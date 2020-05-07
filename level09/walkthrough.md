# Level09

    $ ./level09 
    --------------------------------------------
    |   ~Welcome to l33t-m$n ~    v1337        |
    --------------------------------------------
    >: Enter your username
    >>: coucou
    >: Welcome, coucou
    >: Msg @Unix-Dude
    >>: hello
    >: Msg sent!

The program send a message?

Start in GDB

The program fill a structure `message` with the username and message input. Using 2 functions `set_message` and `set_username`

    struct s_message {
        char message[140];  /* message */
        char username[40];  /* message+140 */
        int len_message;    /* message+180 */
    };

The function `set_message()` use `strncpy` to fill the structure, with the good size, no overflow possible

The function `set_username()` use a manual loop to fill the structure

    int i = 0; /* rbp-0x4 */
    while (i <= 40 && buffer[i] != 0) {
        message->username[i] = buffer[i];
        i++;
    }

The structure string is a buffer of `char[40]`. The loop start with `i = 0` and loop while `i <= 40`, so 41 times and not 40. This is an overflow

Also, there is a non used function in the program, called `secret_backdoor`

    (gdb) info functions
    All defined functions:

    Non-debugging symbols:
    ....
    0x000000000000088c  secret_backdoor
    0x00000000000008c0  handle_msg
    0x0000000000000932  set_msg
    0x00000000000009cd  set_username
    0x0000000000000aa8  main
    ....

This function ask an input and exec it with `system()`

    void secret_backdoor() {
        char buffer[128]; /* rbp-0x80 */

        fgets(buffer, 128, stdin);
        system(buffer);
        return;
    }

We need to use the overflow in the `set_username` function for execute the function `secret_backdoor`


