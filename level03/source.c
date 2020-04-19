#include <stdio.h> 

void decrypt(int input) {
    /*
        0x08048660 <+0>:	push   ebp
        0x08048661 <+1>:	mov    ebp,esp
        0x08048663 <+3>:	push   edi
        0x08048664 <+4>:	push   esi
        0x08048665 <+5>:	sub    esp,0x40                         <--- stack total 64      
        0x08048668 <+8>:	mov    eax,gs:0x14
        0x0804866e <+14>:	mov    DWORD PTR [ebp-0xc],eax          <--- ebp-0xc = 12
        0x08048671 <+17>:	xor    eax,eax
        0x08048673 <+19>:	mov    DWORD PTR [ebp-0x1d],0x757c7d51  <--- ebp-0x1d = 29
        0x0804867a <+26>:	mov    DWORD PTR [ebp-0x19],0x67667360  <--- ebp-0x19 = 25
        0x08048681 <+33>:	mov    DWORD PTR [ebp-0x15],0x7b66737e  <--- ebp-0x15 = 21
        0x08048688 <+40>:	mov    DWORD PTR [ebp-0x11],0x33617c7d  <--- ebp-0x11 = 17
        0x0804868f <+47>:	mov    BYTE PTR [ebp-0xd],0x0           <--- ebp-0xd = 13
    */
    /*
    int str_part1 = 0x757c7d51;
    int str_part1 = 0x67667360;
    int str_part1 = 0x7b66737e;
    int str_part1 = 0x33617c7d;
    char str_end = 0;
    */
    char buf[17] = "Q}|u`sfg~sf{}|a3"; /* total = 29 - 13 = 16 + 1 (\0)  = 17 | ebp-0x1d */

    /*
        0x08048693 <+51>:	push   eax
        0x08048694 <+52>:	xor    eax,eax
        0x08048696 <+54>:	je     0x804869b <decrypt+59>
        0x08048698 <+56>:	add    esp,0x4
        0x0804869b <+59>:	pop    eax
        0x0804869c <+60>:	lea    eax,[ebp-0x1d]                       buf
        0x0804869f <+63>:	mov    DWORD PTR [ebp-0x2c],0xffffffff
        0x080486a6 <+70>:	mov    edx,eax                              buf
        0x080486a8 <+72>:	mov    eax,0x0
        0x080486ad <+77>:	mov    ecx,DWORD PTR [ebp-0x2c]             0xffffffff
        0x080486b0 <+80>:	mov    edi,edx                              buf
        0x080486b2 <+82>:	repnz scas al,BYTE PTR es:[edi]
        0x080486b4 <+84>:	mov    eax,ecx
        0x080486b6 <+86>:	not    eax
        0x080486b8 <+88>:	sub    eax,0x1
        0x080486bb <+91>:	mov    DWORD PTR [ebp-0x24],eax
    */
    size_t len = strlen(buf); /* ebp-0x24 */

    int i = 0; /* ebp-0x28 */

    /*
        0x080486c5 <+101>:	jmp    0x80486e5 <decrypt+133>
            ....
        0x080486e5 <+133>:	mov    eax,DWORD PTR [ebp-0x28]              i
        0x080486e8 <+136>:	cmp    eax,DWORD PTR [ebp-0x24]              len
        0x080486eb <+139>:	jb     0x80486c7 <decrypt+103>
    */
    while (i < len) {
        /*
            0x080486c7 <+103>:	lea    eax,[ebp-0x1d]                       buf
            0x080486ca <+106>:	add    eax,DWORD PTR [ebp-0x28]             i
            0x080486cd <+109>:	movzx  eax,BYTE PTR [eax]
            0x080486d0 <+112>:	mov    edx,eax
                Get  buf[i] and put in edx

            0x080486d2 <+114>:	mov    eax,DWORD PTR [ebp+0x8]              input
            0x080486d5 <+117>:	xor    eax,edx                              buf[i] XOR input
            0x080486d7 <+119>:	mov    edx,eax
            0x080486d9 <+121>:	lea    eax,[ebp-0x1d]                       buf
            0x080486dc <+124>:	add    eax,DWORD PTR [ebp-0x28]              i
            0x080486df <+127>:	mov    BYTE PTR [eax],dl                    put least significant byte of edx (result of xor) in eax (buf[i])
        */
        buf[i] = buf[i] ^ input;
        /*
            0x080486e1 <+129>:	add    DWORD PTR [ebp-0x28],0x1
        */
        i++;
    }
    /*
        0x080486ed <+141>:	lea    eax,[ebp-0x1d]       buf
        0x080486f0 <+144>:	mov    edx,eax
        0x080486f2 <+146>:	mov    eax,0x80489c3        "Congratulations!"
        0x080486f7 <+151>:	mov    ecx,0x11             17
        0x080486fc <+156>:	mov    esi,edx
        0x080486fe <+158>:	mov    edi,eax
        0x08048700 <+160>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]       strncmp
        0x08048702 <+162>:	seta   dl
        0x08048705 <+165>:	setb   al
        0x08048708 <+168>:	mov    ecx,edx
        0x0804870a <+170>:	sub    cl,al
        0x0804870c <+172>:	mov    eax,ecx
        0x0804870e <+174>:	movsx  eax,al
        0x08048711 <+177>:	test   eax,eax
        0x08048713 <+179>:	jne    0x8048723 <decrypt+195>
    */
    if (strncmp(buf, "Congratulations!", 17) == 0) {
        system("/bin/sh");
    } else {
        puts("\nInvalid Password");
    }
    return;
}

void test(int input, int nb) {
    nb = nb - input;
    if (nb <= 21) {
        decrypt(nb);
    }
    decrypt(rand());
    return;
}


int main(int argc, char **argv) {
    
    int input; /* esp+0x1c */

    srand(time(0));
    puts("***********************************");
    puts("*\t\tlevel03\t\t**");
    puts("***********************************");
    printf("Password:");
    scanf("%d", &input);
    test(input, 322424845);
    return 0;
}
