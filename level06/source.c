void auth(char *login, unsigned int serial) {
    /*
    0x08048748 <+0>:	push   ebp
    0x08048749 <+1>:	mov    ebp,esp
    0x0804874b <+3>:	sub    esp,0x28
        stack total = 40
    0x0804874e <+6>:	mov    DWORD PTR [esp+0x4],0x8048a63
    0x08048756 <+14>:	mov    eax,DWORD PTR [ebp+0x8]
    0x08048759 <+17>:	mov    DWORD PTR [esp],eax
    0x0804875c <+20>:	call   0x8048520 <strcspn@plt>
    0x08048761 <+25>:	add    eax,DWORD PTR [ebp+0x8]
    0x08048764 <+28>:	mov    BYTE PTR [eax],0x0
    */
    login[strcspn(login, "\n")] = 0; /* ebp+0x8 */
    /*
    0x08048767 <+31>:	mov    DWORD PTR [esp+0x4],0x20
    0x0804876f <+39>:	mov    eax,DWORD PTR [ebp+0x8]
    0x08048772 <+42>:	mov    DWORD PTR [esp],eax
    0x08048775 <+45>:	call   0x80485d0 <strnlen@plt>
    0x0804877a <+50>:	mov    DWORD PTR [ebp-0xc],eax
        ebp-0xc = 12
    */
    size_t len = strnlen(login, 32); /* ebp-0xc */
    /*
    0x0804877d <+53>:	push   eax
    0x0804877e <+54>:	xor    eax,eax
    0x08048780 <+56>:	je     0x8048785 <auth+61>
    0x08048782 <+58>:	add    esp,0x4
    0x08048785 <+61>:	pop    eax
    0x08048786 <+62>:	cmp    DWORD PTR [ebp-0xc],0x5
    0x0804878a <+66>:	jg     0x8048796 <auth+78>
    */
    if (len > 5) {
        /*
        0x08048796 <+78>:	mov    DWORD PTR [esp+0xc],0x0
        0x0804879e <+86>:	mov    DWORD PTR [esp+0x8],0x1
        0x080487a6 <+94>:	mov    DWORD PTR [esp+0x4],0x0
        0x080487ae <+102>:	mov    DWORD PTR [esp],0x0
        0x080487b5 <+109>:	call   0x80485f0 <ptrace@plt>
        0x080487ba <+114>:	cmp    eax,0xffffffff
        0x080487bd <+117>:	jne    0x80487ed <auth+165>
        */
        if (ptrace(0, 0, 1, 0) == -1) {
            /*
            0x080487bf <+119>:	mov    DWORD PTR [esp],0x8048a68
            0x080487c6 <+126>:	call   0x8048590 <puts@plt>
            */
            puts("\033[32m.----------------------------.");
            /*
            0x080487cb <+131>:	mov    DWORD PTR [esp],0x8048a8c
            0x080487d2 <+138>:	call   0x8048590 <puts@plt>
            */
            puts("\033[31m| !! TAMPERING DETECTED !!  |");
            /*
            0x080487d7 <+143>:	mov    DWORD PTR [esp],0x8048ab0
            0x080487de <+150>:	call   0x8048590 <puts@plt>
            */
            puts("\033[32m'----------------------------'");
            /*
            0x080487e3 <+155>:	mov    eax,0x1
            0x080487e8 <+160>:	jmp    0x8048877 <auth+303>
            */
            return 1;
        }
        /*
        0x080487ed <+165>:	mov    eax,DWORD PTR [ebp+0x8]
        0x080487f0 <+168>:	add    eax,0x3                  login[3]
        0x080487f3 <+171>:	movzx  eax,BYTE PTR [eax]
        0x080487f6 <+174>:	movsx  eax,al
        0x080487f9 <+177>:	xor    eax,0x1337               login[3] ^ 0x1337
        0x080487fe <+182>:	add    eax,0x5eeded             (login[3] ^ 0x1337) + 0x5eeded
        0x08048803 <+187>:	mov    DWORD PTR [ebp-0x10],eax 
            ebp-0x10 = 16 - 12 = 4 ; int
        */
        int nb = (login[3] ^ 4919) + 6221293 /* ebp-0x10 */
        /*
        0x08048806 <+190>:	mov    DWORD PTR [ebp-0x14],0x0
        ebp-0x14 = 20 - 16 = 4 ; int
        */
        int i = 0; /* ebp-0x14 */
        /*
        0x0804880d <+197>:	jmp    0x804885b <auth+275>
        */
        /*
        0x0804885b <+275>:	mov    eax,DWORD PTR [ebp-0x14]
        0x0804885e <+278>:	cmp    eax,DWORD PTR [ebp-0xc]
        0x08048861 <+281>:	jl     0x804880f <auth+199>
        */
        while (i < len) {
            /*
            0x0804880f <+199>:	mov    eax,DWORD PTR [ebp-0x14]
            0x08048812 <+202>:	add    eax,DWORD PTR [ebp+0x8]  login[i]
            0x08048815 <+205>:	movzx  eax,BYTE PTR [eax]
            0x08048818 <+208>:	cmp    al,0x1f
            0x0804881a <+210>:	jg     0x8048823 <auth+219>
            */
            if (login[i] <= 31) {
                /*
                0x0804881c <+212>:	mov    eax,0x1
                0x08048821 <+217>:	jmp    0x8048877 <auth+303>
                */
                return 1;
            }
            /*
                hashing algo
            0x08048823 <+219>:	mov    eax,DWORD PTR [ebp-0x14]
            0x08048826 <+222>:	add    eax,DWORD PTR [ebp+0x8]      eax = login[i]
            0x08048829 <+225>:	movzx  eax,BYTE PTR [eax]
            0x0804882c <+228>:	movsx  eax,al
            0x0804882f <+231>:	mov    ecx,eax
            0x08048831 <+233>:	xor    ecx,DWORD PTR [ebp-0x10]     ecx = login[i] ^ nb
            0x08048834 <+236>:	mov    edx,0x88233b2b               edx = 0x88233b2b
            0x08048839 <+241>:	mov    eax,ecx                      eax = ecx
            0x0804883b <+243>:	mul    edx                          edx:eax = (login[i] ^ nb) * 0x88233b2b
            0x0804883d <+245>:	mov    eax,ecx                      eax = ecx
            0x0804883f <+247>:	sub    eax,edx                      eax = eax - edx
            0x08048841 <+249>:	shr    eax,1                        eax = eax / 2
            0x08048843 <+251>:	add    eax,edx                      eax = eax + edx
            0x08048845 <+253>:	shr    eax,0xa                      eax = eax / 1024
            0x08048848 <+256>:	imul   eax,eax,0x539                edx:eax = eax * 0x539
            0x0804884e <+262>:	mov    edx,ecx                      edx = ecx
            0x08048850 <+264>:	sub    edx,eax                      edx = edx - eax
            0x08048852 <+266>:	mov    eax,edx                      eax = edx
            0x08048854 <+268>:	add    DWORD PTR [ebp-0x10],eax     nb += eax
            */
            nb += hashing_algo(login[i]);
            /*
            0x08048857 <+271>:	add    DWORD PTR [ebp-0x14],0x1 *login++
            */
            i++;
        }
        /*
        0x08048863 <+283>:	mov    eax,DWORD PTR [ebp+0xc]
            ebp+0xc = 12 = serial
        0x08048866 <+286>:	cmp    eax,DWORD PTR [ebp-0x10]     BREAKPOINT HERE TO GET ebp-0x10 == serial
        0x08048869 <+289>:	je     0x8048872 <auth+298>
        */
        if (serial != nb) {
            /*
            0x0804886b <+291>:	mov    eax,0x1
            0x08048870 <+296>:	jmp    0x8048877 <auth+303>
            */
           return 1;
        }
        /*
        0x08048872 <+298>:	mov    eax,0x0
        */
        return 0;
    }
    /*
    0x0804878c <+68>:	mov    eax,0x1
    0x08048791 <+73>:	jmp    0x8048877 <auth+303>
    */
    return 1;
    /*
    0x08048877 <+303>:	leave  
    0x08048878 <+304>:	ret
    */
}

int main(int ac, char **av) {
    /*
    0x08048879 <+0>:	push   ebp
    0x0804887a <+1>:	mov    ebp,esp
    0x0804887c <+3>:	and    esp,0xfffffff0
    0x0804887f <+6>:	sub    esp,0x50
        total stack 80
    0x08048882 <+9>:	mov    eax,DWORD PTR [ebp+0xc]
        ebp+0xc = 12 = argv[0]
    0x08048885 <+12>:	mov    DWORD PTR [esp+0x1c],eax
        esp+0x1c = 28
    0x08048889 <+16>:	mov    eax,gs:0x14
    0x0804888f <+22>:	mov    DWORD PTR [esp+0x4c],eax
        esp+0x4c = 76 = 80 - 76 = 4 ; int
    */
    int var1 = 20; /* check ? */
    /*
    0x08048893 <+26>:	xor    eax,eax
    0x08048895 <+28>:	push   eax
    0x08048896 <+29>:	xor    eax,eax
    0x08048898 <+31>:	je     0x804889d <main+36>
    0x0804889a <+33>:	add    esp,0x4
    0x0804889d <+36>:	pop    eax
    0x0804889e <+37>:	mov    DWORD PTR [esp],0x8048ad4
    0x080488a5 <+44>:	call   0x8048590 <puts@plt>
    */
    puts("***********************************");
    /*
    0x080488aa <+49>:	mov    DWORD PTR [esp],0x8048af8
    0x080488b1 <+56>:	call   0x8048590 <puts@plt>
    */
    puts("*\t\tlevel06\t\t  *");
    /*
    0x080488b6 <+61>:	mov    DWORD PTR [esp],0x8048ad4
    0x080488bd <+68>:	call   0x8048590 <puts@plt>
    */
    puts("***********************************");
    /*
    0x080488c2 <+73>:	mov    eax,0x8048b08
    0x080488c7 <+78>:	mov    DWORD PTR [esp],eax
    0x080488ca <+81>:	call   0x8048510 <printf@plt>
    */
    printf("-> Enter Login: ");
    /*
    0x080488cf <+86>:	mov    eax,ds:0x804a060
    0x080488d4 <+91>:	mov    DWORD PTR [esp+0x8],eax
    0x080488d8 <+95>:	mov    DWORD PTR [esp+0x4],0x20
    0x080488e0 <+103>:	lea    eax,[esp+0x2c]
        esp+0x2c = 44 = 76 - 44 = 32 ; char[32]
    0x080488e4 <+107>:	mov    DWORD PTR [esp],eax
    0x080488e7 <+110>:	call   0x8048550 <fgets@plt>
    */
    char buffer[32]; /* esp+0x2c */
    fgets(buffer, 32, stdin);
    /*
    0x080488ec <+115>:	mov    DWORD PTR [esp],0x8048ad4
    0x080488f3 <+122>:	call   0x8048590 <puts@plt>
    */
    puts("***********************************");
    /*
    0x080488f8 <+127>:	mov    DWORD PTR [esp],0x8048b1c
    0x080488ff <+134>:	call   0x8048590 <puts@plt>
    */
    puts("***** NEW ACCOUNT DETECTED ********");
    /*
    0x08048904 <+139>:	mov    DWORD PTR [esp],0x8048ad4
    0x0804890b <+146>:	call   0x8048590 <puts@plt>
    */
    puts("***********************************");
    /*
    0x08048910 <+151>:	mov    eax,0x8048b40
    0x08048915 <+156>:	mov    DWORD PTR [esp],eax
    0x08048918 <+159>:	call   0x8048510 <printf@plt>
    */
    printf("-> Enter Serial: ");
    /*
    0x0804891d <+164>:	mov    eax,0x8048a60
    0x08048922 <+169>:	lea    edx,[esp+0x28]
        esp+0x28 = 40 = 44 - 40 = 4 ; int
    0x08048926 <+173>:	mov    DWORD PTR [esp+0x4],edx
    0x0804892a <+177>:	mov    DWORD PTR [esp],eax
    0x0804892d <+180>:	call   0x80485e0 <__isoc99_scanf@plt>
    */
    unsigned int serial;
    scanf("%u", &serial);
    /*
    0x08048932 <+185>:	mov    eax,DWORD PTR [esp+0x28]
    0x08048936 <+189>:	mov    DWORD PTR [esp+0x4],eax
    0x0804893a <+193>:	lea    eax,[esp+0x2c]
    0x0804893e <+197>:	mov    DWORD PTR [esp],eax
    0x08048941 <+200>:	call   0x8048748 <auth>
    */
    /*
    0x08048946 <+205>:	test   eax,eax
    0x08048948 <+207>:	jne    0x8048969 <main+240>
    */
    if (auth(buffer, serial) == 0) {
        /*
        0x0804894a <+209>:	mov    DWORD PTR [esp],0x8048b52
        0x08048951 <+216>:	call   0x8048590 <puts@plt>
        */
        puts("Authenticated!");
        /*
        0x08048956 <+221>:	mov    DWORD PTR [esp],0x8048b61
        0x0804895d <+228>:	call   0x80485a0 <system@plt>
        */
        system("/bin/sh");
        /*
        0x08048962 <+233>:	mov    eax,0x0
        0x08048967 <+238>:	jmp    0x804896e <main+245>
        */
       return 0;
    }
    /*
    0x08048969 <+240>:	mov    eax,0x1
    0x0804896e <+245>:	mov    edx,DWORD PTR [esp+0x4c]
    0x08048972 <+249>:	xor    edx,DWORD PTR gs:0x14
    0x08048979 <+256>:	je     0x8048980 <main+263>
    0x0804897b <+258>:	call   0x8048580 <__stack_chk_fail@plt>
    0x08048980 <+263>:	leave  
    0x08048981 <+264>:	ret 
    */
    return 1;
}