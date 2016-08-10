#!/usr/bin/env python
import fcntl
import os.path

if not os.path.exists("/dev/echo"):
    print("no /dev/echo")
    quit()

if os.path.isfile("/dev/echo"):
    print("/dev/echo should be character device")
    quit()

w = open("/dev/echo", "w")
w.write("blah BLAH blah elfds\n");
w.close()

r = open("/dev/echo", "r")
s = r.read()
print(s)
r.close()
