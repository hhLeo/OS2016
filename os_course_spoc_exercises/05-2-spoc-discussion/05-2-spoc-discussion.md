#lec12 进程／线程控制spoc练习

## SPOC个人练习
### 12.1 进程切换

1. 进程切换的可能时机有哪些？
2. 分析ucore的进程切换代码，说明ucore的进程切换触发时机和进程切换的判断时机都有哪些。
3. ucore的进程控制块数据结构是如何组织的？主要字段分别表示什么？有哪些函数对它进行了修改？有哪些函数用到它？
```
arch_proc_struct
mm_struct
need_resched
wait_state
run_link、list_link、hash_link
```

### 12.2 进程创建

1. fork()的返回值是唯一的吗？父进程和子进程的返回值是不同的。请找到相应的赋值代码。
2. 新进程创建时的进程标识是如何设置的？请指明相关代码。
3. fork()的例子中进程标识的赋值顺序说明进程的执行顺序。
4. 请在ucore启动时显示空闲进程（idleproc）和初始进程（initproc）的进程标识。

---
答：在`proc.c`文件中，可以看到`proc_init()`函数中创建了空闲进程`idleproc`和初始进程`initproc`;
```
		if ((idleproc = alloc_proc()) == NULL) {
			panic("cannot alloc idleproc.\n");
		}

		idleproc->pid = 0;//设置空闲进程的进程标识pid = 0;
		cprintf("pid of idleproc : %d\n", idleproc->pid);//this is spoc-exercise code
		idleproc->state = PROC_RUNNABLE;
		idleproc->kstack = (uintptr_t)bootstack;
		idleproc->need_resched = 1;
		set_proc_name(idleproc, "idle");
		nr_process ++;
```
`initproc`的创建也是在`proc.c`里：
```
		int pid = kernel_thread(init_main, "Hello world!!", 0);
		if (pid <= 0) {
			panic("create init_main failed.\n");
		}

		initproc = find_proc(pid);//initproc的进程标识即为pid
		cprintf("pid of initproc : %d\n", pid);//this is spoc-exercise code
		set_proc_name(initproc, "init");
```
`proc_init()`函数中先创建了第一个进程`idleproc`，并设置该进程的相关状态信息，然后创建了第二个进程，该进程加载的代码是`init_main()`，
然后让`initproc`指向第二个进程，也就得到了`initproc`的进程标识`pid`。输出即可。

---
5.请在ucore启动时显示空闲线程（idleproc）和初始进程(initproc)的进程控制块中的“pde_t *pgdir”的内容。它们是否一致？为什么？

### 12.3 进程加载

1. 加载进程后，新进程进入就绪状态，它开始执行时的第一条指令的位置，在elf中保存在什么地方？在加载后，保存在什么地方？
2. 第一个用户进程执行的代码在哪里？它是什么时候加载到内存中的？

---
答：第一个用户进程执行的代码在user文件夹中，lab8中默认为sh.c文件；加载用户代码的进程代码在/kern/process/proc.c:user_main中，具体过程如下：

1.在`init.c`中调用了`proc_init()`

2.在`proc.c`中，`proc_init`创建了两个内核进程：`idleproc`和`initproc`

```
	idleproc = alloc_proc()
	...
	int pid = kernel_thread(init_main, NULL, 0);
	...
	initproc = find_proc(pid);
```

3.在`proc.c`中，`init_main`创建了`user_main`内核进程

```
	int pid = kernel_thread(user_main, NULL, 0);
```

4.`user_main`是用于执行一个用户程序的内核进程

```
	KERNEL_EXECVE(sh);
```

5.`kernel_execve`：执行`SYS_exec`系统调用，加载用户程序到内存并开始执行进程

---

### 12.4 进程等待与退出

1. 试分析wait()和exit()的结果放在什么地方？exit()是在什么时候放进去的？wait()在什么地方取到出的？
2. 试分析ucore操作系统内核是如何把子进程exit()的返回值传递给父进程wait()的？
2. 试分析sleep()系统调用的实现。在什么地方设置的定时器？它对应的等待队列是哪个？它的唤醒操作在什么地方？
3. 通常的函数调用和函数返回都是一一对应的。有不是一一对应的例外情况？如果有，请举例说明。

## SPOC小组思考题

(1) (spoc)设计一个简化的进程管理子系统，可以管理并调度如下简化进程.给出了[参考代码](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab5/process-cpuio-homework.py)，请理解代码，并完成＂YOUR CODE"部分的内容．　可２个人一组

### 进程的状态 
```
 - RUNNING - 进程正在使用CPU
 - READY   - 进程可使用CPU
 - WAIT    - 进程等待I/O完成
 - DONE    - 进程结束
```

### 进程的行为
```
 - 使用CPU, 
 - 发出YIELD请求,放弃使用CPU
 - 发出I/O操作请求,放弃使用CPU
```

### 进程调度
 - 使用FIFO/FCFS：先来先服务, 只有进程done, yield, io时才会执行切换
   - 先查找位于proc_info队列的curr_proc元素(当前进程)之后的进程(curr_proc+1..end)是否处于READY态，
   - 再查找位于proc_info队列的curr_proc元素(当前进程)之前的进程(begin..curr_proc-1)是否处于READY态
   - 如都没有，继续执行curr_proc直到结束

### 关键模拟变量
 - io_length : IO操作的执行时间
 - 进程控制块
```
PROC_CODE = 'code_'
PROC_PC = 'pc_'
PROC_ID = 'pid_'
PROC_STATE = 'proc_state_'
```
 - 当前进程 curr_proc 
 - 进程列表：proc_info是就绪进程的队列（list），
 - 在命令行（如下所示）需要说明每进程的行为特征：（１）使用CPU ;(2)等待I/O
```
   -l PROCESS_LIST, --processlist= X1:Y1,X2:Y2,...
   X 是进程的执行指令数; 
   Ｙ是执行yield指令（进程放弃CPU,进入READY状态）的比例(0..100) 
   Ｚ是执行I/O请求指令（进程放弃CPU,进入WAIT状态）的比例(0..100)
```
 - 进程切换行为：系统决定何时(when)切换进程:进程结束或进程发出yield请求

### 进程执行
```
instruction_to_execute = self.proc_info[self.curr_proc][PROC_CODE].pop(0)
```

### 关键函数
 - 系统执行过程：run
 - 执行状态切换函数:　move_to_ready/running/done　
 - 调度函数：next_proc

### 执行实例
   
#### 例1
```
$./process-simulation.py  -l 5:30:30,5:40:30 -c
Produce a trace of what would happen when you run these processes:
Process 0
  io
  io
  yld
  cpu
  yld

Process 1
  yld
  io
  yld
  yld
  yld

Important behaviors:
  System will switch when the current process is FINISHED or ISSUES AN YIELD or IO
Time     PID: 0     PID: 1        CPU        IOs 
  1      RUN:io      READY          1            
  2     WAITING    RUN:yld          1          1 
  3     WAITING     RUN:io          1          1 
  4     WAITING    WAITING                     2 
  5     WAITING    WAITING                     2 
  6*     RUN:io    WAITING          1          1 
  7     WAITING    WAITING                     2 
  8*    WAITING    RUN:yld          1          1 
  9     WAITING    RUN:yld          1          1 
 10     WAITING    RUN:yld          1          1 
 11*    RUN:yld       DONE          1            
 12     RUN:cpu       DONE          1            
 13     RUN:yld       DONE          1            
```
