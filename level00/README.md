# Level00

First step - testing the program

    $ ./level00 
    ***********************************
    * 	     -Level00 -		  *
    ***********************************
    Password:coucou

    Invalid Password!

The program ask a password

Start in GDB

The program ask the password with scanf

    0x080484ce <+58>:	mov    eax,0x8048636
    0x080484d3 <+63>:	lea    edx,[esp+0x1c]
    0x080484d7 <+67>:	mov    DWORD PTR [esp+0x4],edx
    0x080484db <+71>:	mov    DWORD PTR [esp],eax
    0x080484de <+74>:	call   0x80483d0 <__isoc99_scanf@plt>

Compare the result stored in `esp+0x1c` with `0x149c`


    0x080484e3 <+79>:	mov    eax,DWORD PTR [esp+0x1c]
    0x080484e7 <+83>:	cmp    eax,0x149c

If it's good, execute `system("/bin/sh")`

    0x080484fa <+102>:	mov    DWORD PTR [esp],0x8048649
    0x08048501 <+109>:	call   0x80483a0 <system@plt>

Just enter the good password : **5276**

    $ ./level00 
    ***********************************
    * 	     -Level00 -		  *
    ***********************************
    Password:5276

    Authenticated!
    $ whoami
    level01