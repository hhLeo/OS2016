#!/bin/sh
rm -f hw3
gcc -o xc -O3 -m32 -Ilinux -Iroot/lib root/bin/c.c
gcc -o xem -O3 -m32 -Ilinux -Iroot/lib root/bin/em.c -lm

./xc -o hw3 -Iroot/lib root/usr/hwprog/hw3.c
./xem hw3
