#小组思考题
2. （spoc)了解race condition. 进入[race-condition代码目录](https://github.com/chyyuu/ucore_lab/tree/master/related_info/lab7/race-condition)。

 - 执行 `./x86.py -p loop.s -t 1 -i 100 -R dx`， 请问`dx`的值是什么？
```
答：输出结果如下
		dx          Thread 0         
		?   
		?   	1000 sub  $1,%dx
		?   	1001 test $0,%dx
		?   	1002 jgte .top
		?   	1003 halt
由此可看出最初dx的值是1,然后减1变成0,判断之后终止。

```
 - 执行 `./x86.py -p loop.s -t 2 -i 100 -a dx=3,dx=3 -R dx` ， 请问`dx`的值是什么？
```
答：根据输出结果得出dx值如下
		dx          Thread 0                Thread 1         
		3   
		2   1000 sub  $1,%dx
		2   1001 test $0,%dx
		2   1002 jgte .top
		1   1000 sub  $1,%dx
		1   1001 test $0,%dx
		1   1002 jgte .top
		0   1000 sub  $1,%dx
		0   1001 test $0,%dx
		0   1002 jgte .top
		-1   1000 sub  $1,%dx
		-1   1001 test $0,%dx
		-1   1002 jgte .top
		-1   1003 halt
		3   ----- Halt;Switch -----  ----- Halt;Switch -----  
		2                            1000 sub  $1,%dx
		2                            1001 test $0,%dx
		2                            1002 jgte .top
		1                            1000 sub  $1,%dx
		1                            1001 test $0,%dx
		1                            1002 jgte .top
		0                            1000 sub  $1,%dx
		0                            1001 test $0,%dx
		0                            1002 jgte .top
		-1                            1000 sub  $1,%dx
		-1                            1001 test $0,%dx
		-1                            1002 jgte .top
		-1                            1003 halt

```
 - 执行 `./x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx`， 请问`dx`的值是什么？
```
答：
    dx          Thread 0                Thread 1         
    3   
    2   1000 sub  $1,%dx
    2   1001 test $0,%dx
    3   ------ Interrupt ------  ------ Interrupt ------  
    2                            1000 sub  $1,%dx
    2   ------ Interrupt ------  ------ Interrupt ------  
    2   1002 jgte .top
    1   1000 sub  $1,%dx
    1   1001 test $0,%dx
    2   ------ Interrupt ------  ------ Interrupt ------  
    2                            1001 test $0,%dx
    2                            1002 jgte .top
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1002 jgte .top
    0   1000 sub  $1,%dx
    2   ------ Interrupt ------  ------ Interrupt ------  
    1                            1000 sub  $1,%dx
    1                            1001 test $0,%dx
    0   ------ Interrupt ------  ------ Interrupt ------  
    0   1001 test $0,%dx
    0   1002 jgte .top
    -1   1000 sub  $1,%dx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1002 jgte .top
    0                            1000 sub  $1,%dx
    0                            1001 test $0,%dx
    -1   ------ Interrupt ------  ------ Interrupt ------  
    -1   1001 test $0,%dx
    -1   1002 jgte .top
    0   ------ Interrupt ------  ------ Interrupt ------  
    0                            1002 jgte .top
    -1                            1000 sub  $1,%dx
    -1   ------ Interrupt ------  ------ Interrupt ------  
    -1   1003 halt
    -1   ----- Halt;Switch -----  ----- Halt;Switch -----  
    -1                            1001 test $0,%dx
    -1   ------ Interrupt ------  ------ Interrupt ------  
    -1                            1002 jgte .top
    -1                            1003 halt

```
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 1 -M 2000`, 请问变量x的值是什么？
```
答：
   2000          Thread 0         
    0   
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   1004 test $0, %bx
    1   1005 jgt .top
    1   1006 halt

```
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -a bx=3 -M 2000`, 请问变量x的值是什么？为何每个线程要循环3次？
```
答：
   2000 bx       Thread 0                Thread 1         
    0   3
    0   3      1000 mov 2000, %ax
    0   3      1001 add $1, %ax
    1   3      1002 mov %ax, 2000
    1   2      1003 sub  $1, %bx
    1   2      1004 test $0, %bx
    1   2      1005 jgt .top
    1   2      1000 mov 2000, %ax
    1   2      1001 add $1, %ax
    2   2      1002 mov %ax, 2000
    2   1      1003 sub  $1, %bx
    2   1      1004 test $0, %bx
    2   1      1005 jgt .top
    2   1      1000 mov 2000, %ax
    2   1      1001 add $1, %ax
    3   1      1002 mov %ax, 2000
    3   0      1003 sub  $1, %bx
    3   0      1004 test $0, %bx
    3   0      1005 jgt .top
    3   0      1006 halt
    3   3    ----- Halt;Switch -----  ----- Halt;Switch -----  
    3   3                         1000 mov 2000, %ax
    3   3                         1001 add $1, %ax
    4   3                         1002 mov %ax, 2000
    4   2                         1003 sub  $1, %bx
    4   2                         1004 test $0, %bx
    4   2                         1005 jgt .top
    4   2                         1000 mov 2000, %ax
    4   2                         1001 add $1, %ax
    5   2                         1002 mov %ax, 2000
    5   1                         1003 sub  $1, %bx
    5   1                         1004 test $0, %bx
    5   1                         1005 jgt .top
    5   1                         1000 mov 2000, %ax
    5   1                         1001 add $1, %ax
    6   1                         1002 mov %ax, 2000
    6   0                         1003 sub  $1, %bx
    6   0                         1004 test $0, %bx
    6   0                         1005 jgt .top
    6   0                         1006 halt

```
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0`， 请问变量x的值是什么？
```
答：
 2000          Thread 0                Thread 1         
    0   
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1000 mov 2000, %ax
    1                            1001 add $1, %ax
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1003 sub  $1, %bx
    1   1004 test $0, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    2                            1002 mov %ax, 2000
    2   ------ Interrupt ------  ------ Interrupt ------  
    2   1005 jgt .top
    2   1006 halt
    2   ----- Halt;Switch -----  ----- Halt;Switch -----  
    2                            1003 sub  $1, %bx
    2   ------ Interrupt ------  ------ Interrupt ------  
    2                            1004 test $0, %bx
    2                            1005 jgt .top
    2                            1006 halt

```
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 1`， 请问变量x的值是什么？
```
答：
2000          Thread 0                Thread 1         
    0   
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1000 mov 2000, %ax
    1                            1001 add $1, %ax
    2                            1002 mov %ax, 2000
    2   ------ Interrupt ------  ------ Interrupt ------  
    2   1004 test $0, %bx
    2   1005 jgt .top
    2   ------ Interrupt ------  ------ Interrupt ------  
    2                            1003 sub  $1, %bx
    2                            1004 test $0, %bx
    2                            1005 jgt .top
    2   ------ Interrupt ------  ------ Interrupt ------  
    2   1006 halt
    2   ----- Halt;Switch -----  ----- Halt;Switch -----  
    2                            1006 halt

```
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 2`， 请问变量x的值是什么？
```
答：
 2000          Thread 0                Thread 1         
    0   
    0   1000 mov 2000, %ax
    0   ------ Interrupt ------  ------ Interrupt ------  
    0                            1000 mov 2000, %ax
    0                            1001 add $1, %ax
    1                            1002 mov %ax, 2000
    1                            1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1004 test $0, %bx
    1                            1005 jgt .top
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1006 halt
    1   ----- Halt;Switch -----  ----- Halt;Switch -----  
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1004 test $0, %bx
    1   1005 jgt .top
    1   1006 halt

``` 
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1`， 请问变量x的值是什么？
```
答：
2000          Thread 0                Thread 1         
    0   
    0   1000 mov 2000, %ax
    0   ------ Interrupt ------  ------ Interrupt ------  
    0                            1000 mov 2000, %ax
    0   ------ Interrupt ------  ------ Interrupt ------  
    0   1001 add $1, %ax
    0   ------ Interrupt ------  ------ Interrupt ------  
    0                            1001 add $1, %ax
    0   ------ Interrupt ------  ------ Interrupt ------  
    1   1002 mov %ax, 2000
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1002 mov %ax, 2000
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1004 test $0, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1004 test $0, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1005 jgt .top
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1005 jgt .top
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1006 halt
    1   ----- Halt;Switch -----  ----- Halt;Switch -----  
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1006 halt

``` 
