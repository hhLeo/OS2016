# lab6 spoc 思考题

## 小组练习与思考题

### (1)(spoc) 跟踪和展现ucore的处理机调度过程

在ucore执行处理机调度时，跟踪并显示上一个让出CPU线程的暂停代码位置和下一个进入执行状态线程的开始执行位置。

> 详见代码

### (2)(spoc) 理解调度算法支撑框架的执行过程

即在ucore运行过程中通过`cprintf`函数来完整地展现出来多个进程在调度算法和框架的支撑下，在相关调度点如何动态调度和执行的细节。(越全面细致越好)

> 详见代码

### 练习用的[lab6 spoc exercise project source code](https://github.com/chyyuu/ucore_lab/tree/master/labcodes_answer/lab6_result)

## 课堂问答题

### 为什么只用32位整数存stride的数值就够了？溢出时如何判断大小？

> 由于 `STRIDE_MAX – STRIDE_MIN <= PASS_MAX <= BIG_STRIDE` 且 `stride`, `pass` 是无符号整数（0 ~ 2^32-1），又用有符号整数（-2^31 ~ 2^31-1）表示 `delta =（Proc.A.stride – Proc.B.stride）`，则 `|delta| <= 2^31` 。

> 取 `PASS_MAX ` 为小于 `2^31` 的无符号整数，假设 `Proc.B.stride` 加上了程序B的 `pass`，比较 `(int)delta` 与 0 的大小。

> 若 `(int)delta > 0` ，则 `Proc.B.stride` 溢出了，考察 `Proc.A.stride – Proc.B.stride - 2^32` ；

> 若 `(int)delta < 0` ，则 `Proc.B.stride` 未溢出。
