#!/bin/sh
rm -f hw1
gcc -o xc -O3 -m32 -Ilinux -Iroot/lib root/bin/c.c
gcc -o xem -O3 -m32 -Ilinux -Iroot/lib root/bin/em.c -lm
./xc -o hw1 -Iroot/lib root/usr/hwprog/hw1.c
./xem hw1
