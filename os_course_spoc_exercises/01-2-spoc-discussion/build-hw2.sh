#!/bin/sh
rm -f hw2
gcc -o xc -O3 -m32 -Ilinux -Iroot/lib root/bin/c.c
gcc -o xem -O3 -m32 -Ilinux -Iroot/lib root/bin/em.c -lm

./xc -o hw2 -Iroot/lib root/usr/hwprog/hw2.c
./xem hw2
