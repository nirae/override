# Level06

First, there is a lot of protection on the binary

Testing the program

    $ ./level06 
    ***********************************
    *		level06		  *
    ***********************************
    -> Enter Login: coucou
    ***********************************
    ***** NEW ACCOUNT DETECTED ********
    ***********************************
    -> Enter Serial: coucou

Start in GDB

    0x08048879 <+0>:	push   ebp
    0x0804887a <+1>:	mov    ebp,esp
    0x0804887c <+3>:	and    esp,0xfffffff0
    0x0804887f <+6>:	sub    esp,0x50
    0x08048882 <+9>:	mov    eax,DWORD PTR [ebp+0xc]
    0x08048885 <+12>:	mov    DWORD PTR [esp+0x1c],eax
    0x08048889 <+16>:	mov    eax,gs:0x14
    0x0804888f <+22>:	mov    DWORD PTR [esp+0x4c],eax
    0x08048893 <+26>:	xor    eax,eax
    0x08048895 <+28>:	push   eax
    0x08048896 <+29>:	xor    eax,eax
    0x08048898 <+31>:	je     0x804889d <main+36>
    0x0804889a <+33>:	add    esp,0x4
    0x0804889d <+36>:	pop    eax
    0x0804889e <+37>:	mov    DWORD PTR [esp],0x8048ad4
    0x080488a5 <+44>:	call   0x8048590 <puts@plt>
    0x080488aa <+49>:	mov    DWORD PTR [esp],0x8048af8
    0x080488b1 <+56>:	call   0x8048590 <puts@plt>
    0x080488b6 <+61>:	mov    DWORD PTR [esp],0x8048ad4
    0x080488bd <+68>:	call   0x8048590 <puts@plt>
    0x080488c2 <+73>:	mov    eax,0x8048b08
    0x080488c7 <+78>:	mov    DWORD PTR [esp],eax
    0x080488ca <+81>:	call   0x8048510 <printf@plt>
    0x080488cf <+86>:	mov    eax,ds:0x804a060
    0x080488d4 <+91>:	mov    DWORD PTR [esp+0x8],eax
    0x080488d8 <+95>:	mov    DWORD PTR [esp+0x4],0x20
    0x080488e0 <+103>:	lea    eax,[esp+0x2c]
    0x080488e4 <+107>:	mov    DWORD PTR [esp],eax
    0x080488e7 <+110>:	call   0x8048550 <fgets@plt>
    0x080488ec <+115>:	mov    DWORD PTR [esp],0x8048ad4
    0x080488f3 <+122>:	call   0x8048590 <puts@plt>
    0x080488f8 <+127>:	mov    DWORD PTR [esp],0x8048b1c
    0x080488ff <+134>:	call   0x8048590 <puts@plt>
    0x08048904 <+139>:	mov    DWORD PTR [esp],0x8048ad4
    0x0804890b <+146>:	call   0x8048590 <puts@plt>
    0x08048910 <+151>:	mov    eax,0x8048b40
    0x08048915 <+156>:	mov    DWORD PTR [esp],eax
    0x08048918 <+159>:	call   0x8048510 <printf@plt>
    0x0804891d <+164>:	mov    eax,0x8048a60
    0x08048922 <+169>:	lea    edx,[esp+0x28]
    0x08048926 <+173>:	mov    DWORD PTR [esp+0x4],edx
    0x0804892a <+177>:	mov    DWORD PTR [esp],eax
    0x0804892d <+180>:	call   0x80485e0 <__isoc99_scanf@plt>
    0x08048932 <+185>:	mov    eax,DWORD PTR [esp+0x28]
    0x08048936 <+189>:	mov    DWORD PTR [esp+0x4],eax
    0x0804893a <+193>:	lea    eax,[esp+0x2c]
    0x0804893e <+197>:	mov    DWORD PTR [esp],eax
    0x08048941 <+200>:	call   0x8048748 <auth>
    0x08048946 <+205>:	test   eax,eax
    0x08048948 <+207>:	jne    0x8048969 <main+240>
    0x0804894a <+209>:	mov    DWORD PTR [esp],0x8048b52
    0x08048951 <+216>:	call   0x8048590 <puts@plt>
    0x08048956 <+221>:	mov    DWORD PTR [esp],0x8048b61
    0x0804895d <+228>:	call   0x80485a0 <system@plt>
    0x08048962 <+233>:	mov    eax,0x0
    0x08048967 <+238>:	jmp    0x804896e <main+245>
    0x08048969 <+240>:	mov    eax,0x1
    0x0804896e <+245>:	mov    edx,DWORD PTR [esp+0x4c]
    0x08048972 <+249>:	xor    edx,DWORD PTR gs:0x14
    0x08048979 <+256>:	je     0x8048980 <main+263>
    0x0804897b <+258>:	call   0x8048580 <__stack_chk_fail@plt>
    0x08048980 <+263>:	leave  
    0x08048981 <+264>:	ret

The main of the program first call `fgets()` to get the login and put it in a buffer of `char[32]`. Next there is a call to scanf to get the serial and store it in an `unsigned int`.

Then, the main call a function `auth` with the login and the serial and check the return. If the return is 0 the main call `system("/bin/sh")` (what we wan't), else return. We need to pass the validation of `auth()`

In the function auth, there is a complex hashing algorithm for calculate the good serial with the login (calculating with each characters of the login). We could reverse it to do a hashing program for generate the serials, or ... read the memory when the function `auth` compare the serial generated with the input!

At line auth+286 :

        /*
        0x08048863 <+283>:	mov    eax,DWORD PTR [ebp+0xc]  ; input serial
        0x08048866 <+286>:	cmp    eax,DWORD PTR [ebp-0x10] ; serial generated
        0x08048869 <+289>:	je     0x8048872 <auth+298>
        */
        if (serial != nb) {
           return 1;
        }
        else {
            return 0;
        }

We need also to bypass the ptrace verification

    (gdb) catch syscall ptrace
    Catchpoint 1 (syscall 'ptrace' [26])
    (gdb) commands 1
    Type commands for breakpoint(s) 1, one per line.
    End with a line saying just "end".
    >set $eax=0
    >continue
    >end

With that, ptrace will always return 0

Breakpoint to the comp line and we put a bullshit serial

    (gdb) b *auth+286
    Breakpoint 2 at 0x8048866
    (gdb) r
    Starting program: /home/users/level06/level06 
    ***********************************
    *		level06		  *
    ***********************************
    -> Enter Login: coucou
    ***********************************
    ***** NEW ACCOUNT DETECTED ********
    ***********************************
    -> Enter Serial: 1234

    Catchpoint 1 (call to syscall ptrace), 0xf7fdb440 in __kernel_vsyscall ()

    Catchpoint 1 (returned from syscall ptrace), 0xf7fdb440 in __kernel_vsyscall ()

    Breakpoint 2, 0x08048866 in auth ()

    (gdb) p $eax
    $1 = 1234

We found our serial in EAX, so the real serial generated is in EBP-0x10

    (gdb) x/x $ebp-0x10
    0xffffd5b8:	0x005f1ae1

The serial is `0x005f1ae1`, in dec `6232801`

    $ ./level06 
    ***********************************
    *	        	level06	          *
    ***********************************
    -> Enter Login: coucou
    ***********************************
    ***** NEW ACCOUNT DETECTED ********
    ***********************************
    -> Enter Serial: 6232801
    Authenticated!
    $ whoami
    level07