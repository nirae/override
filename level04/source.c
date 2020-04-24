#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/ptrace.h>

int main(void)
{
    /*
    0x080486c8 <+0>:	push   ebp
    0x080486c9 <+1>:	mov    ebp,esp
    0x080486cb <+3>:	push   edi
    0x080486cc <+4>:	push   ebx
    0x080486cd <+5>:	and    esp,0xfffffff0
    0x080486d0 <+8>:	sub    esp,0xb0
        total stack = 176
    */
    /*
    0x080486d6 <+14>:	call   0x8048550 <fork@plt>
    0x080486db <+19>:	mov    DWORD PTR [esp+0xac],eax
        esp+0xac (172) = 176 - 172 = 4 = int ?
    */
    pid_t child_pid = fork(); /* esp+0xac */

    /*
    0x080486e2 <+26>:	lea    ebx,[esp+0x20]
        esp+0x20 (32) = 160 - 32 = 128
    */
    char buffer[128]; /* esp+0x20 */

    /*
    0x080486e6 <+30>:	mov    eax,0x0
    0x080486eb <+35>:	mov    edx,0x20
    0x080486f0 <+40>:	mov    edi,ebx
    0x080486f2 <+42>:	mov    ecx,edx
    0x080486f4 <+44>:	rep stos DWORD PTR es:[edi],eax
    */
    bzero(buffer, 32);

    /*
    0x080486f6 <+46>:	mov    DWORD PTR [esp+0xa8],0x0
        esp+0xa8 (168) = 172 - 168 = 4 int?
    */
    int ptrace_ret = 0; /* esp+0xa8 */

    /*
    0x08048701 <+57>:	mov    DWORD PTR [esp+0x1c],0x0
        esp+0x1c (28) = 32 - 28 = 4 int?
    */
    int status = 0; /* esp+0x1c */

    /*
    0x08048709 <+65>:	cmp    DWORD PTR [esp+0xac],0x0
    0x08048711 <+73>:	jne    0x8048769 <main+161>
    */
    if (child_pid == 0)
    {
        /*
        0x08048713 <+75>:	mov    DWORD PTR [esp+0x4],0x1
        0x0804871b <+83>:	mov    DWORD PTR [esp],0x1
        0x08048722 <+90>:	call   0x8048540 <prctl@plt>
        */
        prctl(1, 1);
        /*
        0x08048727 <+95>:	mov    DWORD PTR [esp+0xc],0x0
        0x0804872f <+103>:	mov    DWORD PTR [esp+0x8],0x0
        0x08048737 <+111>:	mov    DWORD PTR [esp+0x4],0x0
        0x0804873f <+119>:	mov    DWORD PTR [esp],0x0
        0x08048746 <+126>:	call   0x8048570 <ptrace@plt>
        */
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        /*
        0x0804874b <+131>:	mov    DWORD PTR [esp],0x8048903
        0x08048752 <+138>:	call   0x8048500 <puts@plt>
        */
        puts("Give me some shellcode, k");
        /*
        0x08048757 <+143>:	lea    eax,[esp+0x20]
        0x0804875b <+147>:	mov    DWORD PTR [esp],eax
        0x0804875e <+150>:	call   0x80484b0 <gets@plt>
        */
        gets(&buffer);
        /*
        0x08048763 <+155>:	jmp    0x804881a <main+338>
        0x08048768 <+160>:	nop
        */
        return 0;
    }
    /*
    0x080487ba <+242>:	mov    DWORD PTR [esp+0xc],0x0
    0x080487c2 <+250>:	mov    DWORD PTR [esp+0x8],0x2c
    0x080487ca <+258>:	mov    eax,DWORD PTR [esp+0xac]
    0x080487d1 <+265>:	mov    DWORD PTR [esp+0x4],eax
    0x080487d5 <+269>:	mov    DWORD PTR [esp],0x3
    0x080487dc <+276>:	call   0x8048570 <ptrace@plt>
    
    0x080487e1 <+281>:	mov    DWORD PTR [esp+0xa8],eax
    0x080487e8 <+288>:	cmp    DWORD PTR [esp+0xa8],0xb
    0x080487f0 <+296>:	jne    0x8048768 <main+160>
    */
    while (ptrace_ret = ptrace(PTRACE_PEEKUSR, child_pid, 44, 0) != 11)
    {
        /*
        0x08048769 <+161>:	lea    eax,[esp+0x1c]
        0x0804876d <+165>:	mov    DWORD PTR [esp],eax
        0x08048770 <+168>:	call   0x80484f0 <wait@plt>
        */
        wait(&status);
        /*
        0x08048775 <+173>:	mov    eax,DWORD PTR [esp+0x1c]
        0x08048779 <+177>:	mov    DWORD PTR [esp+0xa0],eax
            esp+0xa0 (160) = 164 - 160 = 4 ? int tmp
        0x08048780 <+184>:	mov    eax,DWORD PTR [esp+0xa0]
        0x08048787 <+191>:	and    eax,0x7f 127
        0x0804878a <+194>:	test   eax,eax
        0x0804878c <+196>:	je     0x80487ac <main+228>
            if ((status & 127) == 0)
        */
        /*
        0x0804878e <+198>:	mov    eax,DWORD PTR [esp+0x1c]
        0x08048792 <+202>:	mov    DWORD PTR [esp+0xa4],eax
            esp+0xa4 (164) = 168 - 164 = 4 ? int tmp
        0x08048799 <+209>:	mov    eax,DWORD PTR [esp+0xa4]
        0x080487a0 <+216>:	and    eax,0x7f
        0x080487a3 <+219>:	add    eax,0x1
        0x080487a6 <+222>:	sar    al,1 ; division / 2
        0x080487a8 <+224>:	test   al,al
        0x080487aa <+226>:	jle    0x80487ba <main+242> ; jle -> <=
            if ((((status & 127) + 1) / 2) > 0)
        */
        if ((status & 127) == 0 || (((status & 127) + 1) / 2) > 0)
        {
            /*
            0x080487ac <+228>:	mov    DWORD PTR [esp],0x804891d
            0x080487b3 <+235>:	call   0x8048500 <puts@plt>
            */
            puts("child is exiting...");
            /*
            0x080487b8 <+240>:	jmp    0x804881a <main+338>
            */
            return 0;
        }
    }
    /*
    0x080487f6 <+302>:	mov    DWORD PTR [esp],0x8048931
    0x080487fd <+309>:	call   0x8048500 <puts@plt>
    */
    puts("no exec() for you");
    /*
    0x08048802 <+314>:	mov    DWORD PTR [esp+0x4],0x9
    0x0804880a <+322>:	mov    eax,DWORD PTR [esp+0xac]
    0x08048811 <+329>:	mov    DWORD PTR [esp],eax
    0x08048814 <+332>:	call   0x8048520 <kill@plt>
    */
    kill(child_pid, 9);
    /*
    0x08048819 <+337>:	nop
    0x0804881a <+338>:	mov    eax,0x0
    0x0804881f <+343>:	lea    esp,[ebp-0x8]
    0x08048822 <+346>:	pop    ebx
    0x08048823 <+347>:	pop    edi
    0x08048824 <+348>:	pop    ebp
    0x08048825 <+349>:	ret    
    End of assembler dump.
    */
    return 0;
}