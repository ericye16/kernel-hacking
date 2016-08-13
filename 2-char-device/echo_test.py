#!/usr/bin/env python
import fcntl
import os

ECHO_IOC_MAGIC = 'e'
ECHO_IOC_CLEAR = ord(ECHO_IOC_MAGIC) << 8 | 0 # _IO(ECHO_IOC_MAGIC, 0)
ECHO_IOC_LOWERCASE = ord(ECHO_IOC_MAGIC) << 8 | 1 # _IO(ECHO_IOC_MAGIC, 1)
ECHO_IOC_UPPERCASE = ord(ECHO_IOC_MAGIC) << 8 | 2 # _IO(ECHO_IOC_MAGIC, 2)
ECHO_IOC_REVERSE = ord(ECHO_IOC_MAGIC) << 8 | 3 # _IO(ECHO_IOC_MAGIC, 3)

if os.geteuid() != 0:
    exit("You need root!")

if not os.path.exists("/dev/echo"):
    exit("no /dev/echo")

if os.path.isfile("/dev/echo"):
    exit("/dev/echo should be character device")

def write_to_module(s):
    w = open("/dev/echo", "w")
    w.write(s);
    w.close()

def read_from_module():
    r = open("/dev/echo", "r")
    s = r.read()
    r.close()
    return s

write_to_module("hi this is a module")
print read_from_module()

f = open("/dev/echo")
fcntl.ioctl(f, ECHO_IOC_UPPERCASE)
print read_from_module()
fcntl.ioctl(f, ECHO_IOC_LOWERCASE)
print read_from_module()
fcntl.ioctl(f, ECHO_IOC_CLEAR)
print read_from_module()
f.close()
