## 3.4 linux系统调用分析
 1. 通过分析[lab1_ex0](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex0.md)了解Linux应用的系统调用编写和含义。(w2l1)
 

 ```
  + 采分点：说明了objdump，nm，file的大致用途，说明了系统调用的具体含义
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 

objdump：反汇编，由二进制机器码反汇编出汇编指令

nm：获取符号信息

file：判断文件格式

 

系统调用含义：

执行man ...
 execve - execute program
 brk, sbrk - change data segment size
 access - check real user's permissions for a file
 mmap2 - map files or devices into memory
 mmap, munmap - map or unmap files or devices into memory
 mprotect - set protection on a region of memory

 

用户态程序发出系统调用请求：将$SYS_write存入eax，其余参数按次序存入寄存器ebx, ecx, edx中，再触发系统调用中断，以进入内核态。ret时，返回值存入eax中，返回用户态。

1. 通过调试[lab1_ex1](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex1.md)了解Linux应用的系统调用执行过程。(w2l1)
 

 ```
  + 采分点：说明了strace的大致用途，说明了系统调用的具体执行过程（包括应用，CPU硬件，操作系统的执行过程）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```


lab1-ex1.exe:

1.strace -c ./lab1-ex1.ex输出hello world,其系统调用的顺序是

 

% time     seconds  usecs/call     calls    errors syscall     系统调用函数注释
------ ----------- ----------- --------- --------- ------------------------------------------
 42.86    0.000042           5         8           mmap  
 15.31    0.000015           4         4           mprotect  --set protection on a region of memory
  9.18    0.000009           9         1           write      --map or unmap files or devices into memory
  9.18    0.000009           5         2           open       --map or unmap files or devices into memory
  8.16    0.000008           8         1           munmap    --map or unmap files or devices into memory
  7.14    0.000007           2         3         3 access    --check real user's permissions for a file
  6.12    0.000006           6         1           execve     --execute the file
  1.02    0.000001           1         1           read          --read from a file descriptor
  1.02    0.000001           0         3           fstat           --get file status
  0.00    0.000000           0         2           close         --close a file descriptor,
  0.00    0.000000           0         1           brk             --change data segment size
  0.00    0.000000           0         1           arch_prctl  -- set architecture-specific thread state
------ ----------- ----------- --------- --------- ----------------
100.00    0.000098                    28         3 total

而lab1-ex1.c的主程序里，只调用了库函数里的printf()函数,可以看出该函数调用里包含了一系列的系统调用。

strace -c:跟踪函数调用的过程和其中的系统调用。

系统调用的过程：应用程序提出请求，操作系统进入内核进行处理，并将处理结果返回给应用程序，返回用户态。
