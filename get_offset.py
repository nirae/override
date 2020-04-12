#!/usr/bin/env python3

import sys


if len(sys.argv) < 2:
    sys.exit()

arg = bytes.fromhex(sys.argv[1])
result = arg.decode("ASCII")

nb = 65
if result.islower():
    nb = nb + 6

print("%s => offset = %d" % (result, (ord(result[0]) - nb) * 4))
