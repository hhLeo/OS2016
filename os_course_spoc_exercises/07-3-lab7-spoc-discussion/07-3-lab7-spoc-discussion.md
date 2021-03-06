# 同步互斥(lec 19 lab7) spoc 思考题


- 有"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的ucore_code和os_exercises的git repo上。

## 个人思考题

### 19.1 总体介绍

1. 同步机制、处理机调度、等待队列和定时器有些什么相互影响？

 > 底层支持：中断禁止、定时器、等待队列
 
 > 同步机制：信号量、管程（条件变量）

 > 同步管理：处理机调度

### 19.2 底层支撑

1. 操作系统内核如何利用定时器实现sleep系统？在定时队列中保存的时间格式是什么？

 > 定时器的数据结构、定时器的设置函数do_sleep()、定时器的触发流程

2. 中断屏蔽控制位在哪个寄存器中？如何修改中断屏蔽控制位？

 > 屏蔽中断指令：STI、CLI

3. 在ucore中有多少种等待队列？
4. 等待队列的两个基本操作down()和up()对线程状态和等待队列有什么影响？

 > 等待队列的数据结构、等待队列操作（等待、唤醒）

### 19.3 信号量设计实现

1. ucore中的信号量实现能实现是按FIFO获取信号量资源吗？给出你的理由。
2. ucore中的信号量实现用到自旋锁(spinlock)吗？为什么？
3. 为什么down()要加一个_down()函数来进行实质的处理？类似情况还出现在up()和_up()。

> 参考回答：有多个有小差异的类似函数要使用相同的核心功能实现。类似情况还出现在do_fork()函数。

### 19.4 管程和条件变量设计实现

1. 管程与信号量是等价的。如何理解？
 - 基于信号量的管程中在什么地方用到信号量？
 - 管程实现中的monitor.next的作用是什么？
 - 分析管程实现中PV操作的配对关系，并解释PV操作的目的。
 - 基于视频中对管程的17个状态或操作的分析，尝试分析管程在入口队列和条件变量等待队列间的等待和唤醒关系。注意分析各队列在什么情况下会有线程进入等待，在什么时候会被唤醒，以及这个等待和唤醒的依赖关系。

### 19.5 哲学家就餐问题

1. 哲学家就餐问题的管程实现中的外部操作成员函数有哪几个？
 - 哲学家就餐问题的管程实现中用了几个条件变量？每个条件变量的作用是什么？
 
## 小组思考题

1. (扩展练习) 每人用ucore中的信号量和条件变量两种手段分别实现40个同步问题中的一题。向勇老师的班级从前往后，陈渝老师的班级从后往前。请先理解与采用python threading 机制实现的异同点。

2. （扩展练习）请在lab7-answer中分析
  -  cvp->count含义是什么？cvp->count是否可能<0, 是否可能>1？请举例或说明原因。
```
		typedef struct condvar{
			semaphore_t sem;        // the sem semaphore  is used to down the waiting proc, and the signaling proc should up the waiting proc
			int count;              // the number of waiters on condvar
			monitor_t * owner;      // the owner(monitor) of this condvar
		} condvar_t;
cvp->count 是指等待条件变量条件不满足时进入睡眠的进程和数目，cvp->count一般不会<0, 因为从理论上看该值不应该<0实现上也看不出会<0, 因为对count的操作都是先加后减的。
```
  -  cvp->owner->next_count含义是什么？cvp->owner->next_count是否可能<0, 是否可能>1？请举例或说明原因。
```
        typedef struct monitor{
        semaphore_t mutex;      // the mutex lock for going into the routines in monitor, should be initialized to 1
        semaphore_t next;       // the next semaphore is used to down the signaling proc itself, and the other OR wakeuped waiting proc should wake up the sleeped signaling proc.
        int next_count;         // the number of of sleeped signaling proc
        condvar_t *cv;          // the condvars in monitor
        } monitor_t;
cvp->owner 是一个monitor, 从代码中可以看到next_count 是指执行cond_signal睡眠的进程，对其操作也是先加后减，不会小于零；因为执行cond_signal的进程会持有锁，不会有其他进程调用cond_signal使next_count再加1,所以不会>1.
```
  -  目前的lab7-answer中管程的实现是Hansen管程类型还是Hoare管程类型？请在lab7-answer中实现另外一种类型的管程。
```
Hoare管程类型
```

