# Level03

First step - testing the program

    $ ./level03 
    ***********************************
    *		level03		**
    ***********************************
    Password:coucou

    Invalid Password

Ask a password


Start in GDB

The program take an integer as a password with scanf and put it in a function **test** with a number `322424845`.

The function **test** do a substraction with the input and the number `322424845`and call a function **decrypt** with the result as a parameter if the result is less or equal then `21`.

The function **decrypt** have a buffer **"Q}|u`sfg~sf{}|a3"** and do a XOR on each char of this buffer with the parameter

```c
int i = 0;
while (i < strlen(buffer)) {
    buffer[i] = buffer[i] ^ param;
}
```

If the string result of this operation is "**Congratulations!"**, the function call `system("/bin/sh")`

We need to write a program for reverse the decrypt function and test the 21 solutions

    $ gcc reverse.c && ./a.out
    Q}|u`sfg~sf{}|a3
    P|}targfrgz|}`2
    S~wbqde|qdy~c1
    R~vcped}pex~b0
    Uyxqdwbczwbyxe7
    Txypevcb{vc~xyd6
    W{zsfu`axu`}{zg5
    Vz{rgta`yta|z{f4
    Yut}h{nov{nsuti;
    Xtu|izonwzortuh:
    [wvjylmtylqwvk9

    Zvw~kxmluxmpvwj8

    ]qpyljkrjwqpm?

    \pqxm~kjs~kvpql>
    _sr{n}hip}husro=
    ^rszo|ihq|itrsn<
    Amlepcvwncvkmlq#
    @lmdqbwvobwjlmp"
    Congratulations!
    Found! The password is 322424827

