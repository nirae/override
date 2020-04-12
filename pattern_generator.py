#!/usr/bin/env python3

import string

for c in string.ascii_uppercase:
    print("%c%c%c%c" % (c, c, c, c), end = '')
for c in string.ascii_lowercase:
    print("%c%c%c%c" % (c, c, c, c), end = '')
print()
