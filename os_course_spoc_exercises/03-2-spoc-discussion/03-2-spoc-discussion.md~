# lec6 SPOC思考题


NOTICE
- 有"w3l2"标记的题是助教要提交到学堂在线上的。
- 有"w3l2"和"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的git repo上。
- 有"hard"标记的题有一定难度，鼓励实现。
- 有"easy"标记的题很容易实现，鼓励实现。
- 有"midd"标记的题是一般水平，鼓励实现。


## 个人思考题
---

（1） (w3l2) 请简要分析64bit CPU体系结构下的分页机制是如何实现的
```
  + 采分点：说明64bit CPU架构的分页机制的大致特点和页表执行过程
  - 答案没有涉及如下3点；（0分）
  - 正确描述了64bit CPU支持的物理内存大小限制（1分）
  - 正确描述了64bit CPU下的多级页表的级数和多级页表的结构或反置页表的结构（2分）
  - 除上述两点外，进一步描述了在多级页表或反置页表下的虚拟地址-->物理地址的映射过程（3分）
 ```
- [x]  

>   x86-64 CPU的实际虚拟地址大小为48位（64位中的低48位，高16位不用于地址转换），可寻址256TB地址空间。x86-64 CPU的实际物理地址大小，不同的CPU不相同，但最大不超过52位。可以通过机器指令cpuid来查询该CPU的实际虚拟地址长度和实际物理地址长度。
    256TB的(虚拟)地址空间被分成了固定大小的页（有三种大小，4KB，2MB，1GB），每一页或者被映射到物理内存，或者没有映射任何东西。一个进程中的虚拟地址，最多需要四级转换，来得到对应的物理地址。
    "pagetable"（页表），一个页表的大小为4KB，放在一个物理页中。由512个8字节的PTE（页表项）组成。页表项的大小为8个字节（64 位），所以一个页表中有512个页表项。页表中的每一项的内容（每项8个字节，64位）低12位之后的M-12位用来放一个物理页的物理地址，低 12bit放着一些标志。
    CPU把虚拟地址转换成物理地址：
   一 个虚拟地址，大小8个字节（64位，实际只使用低48位），包含着找到物理地址的信息，分为5个部分：第39位到第47位这9位（最高9位） 是"pagemap level 4"表中的索引，第30位到第38位这9位是"page directorypointer"表中的索引，第21位到第29位这9位是页目录中的索引，第12位到第20位这9位是页表中的索引(四级页表)，第0位到第11位 这12位（低12位）是页内偏移。对于一个要转换成物理地址的虚拟地址，CPU首先根据CR3中的值，找到"pagemap level4"表所在的物理页，然后根据虚拟地址的第39位到第47位这9位（最高9位）的值作为索引，找到相应的PML4E项，PML4E项中有这个虚 拟地址所对应的"pagedirectory pointer"表的物理地址。有了"page directorypointer"表的物理地址（即表内搜索的基址），根据虚拟地址的第30位到第38位这9位的值作为索引（即offset），找到该"page directorypointer"表中相应的PDPTE项，PDPTE项中有这个虚拟地址所对应的页目录的物理地址。有了页目录的物理地址，根据虚拟地 址的第21位到第29位这9位的值作为索引，找到该页目录中相应的页目录项，页目录项中有这个虚拟地址所对应的页表的物理地址。有了页表的物理地址，根据 虚拟地址的第12位到第20位这9位的值作为索引，找到该页表中相应的页表项，页表项中有这个虚拟地址所对应的物理页的物理地址。最后用虚拟地址的最低 12位，也就是页内偏移，加上这个物理页的物理地址，就得到了该虚拟地址所对应的物理地址。即共有四级页表，第i级页表查出的地址作为第i+1级页表查询的基址，再加上offset（刚开始的虚拟地址里有），得到i+1级页表中的项，作为下一级页表的基址。

参考：http://blog.csdn.net/zdy0_2004/article/details/49530957


## 小组思考题
---

（1）(spoc) 某系统使用请求分页存储管理，若页在内存中，满足一个内存请求需要150ns (10^-9s)。若缺页率是10%，为使有效访问时间达到0.5us(10^-6s),求不在内存的页面的平均访问时间。请给出计算步骤。 

- [x]  

> 500=0.9\*150+0.1\*x

（2）(spoc) 有一台假想的计算机，页大小（page size）为32 Bytes，支持32KB的虚拟地址空间（virtual address space）,有4KB的物理内存空间（physical memory），采用二级页表，一个页目录项（page directory entry ，PDE）大小为1 Byte,一个页表项（page-table entries
PTEs）大小为1 Byte，1个页目录表大小为32 Bytes，1个页表大小为32 Bytes。页目录基址寄存器（page directory base register，PDBR）保存了页目录表的物理地址（按页对齐）。

PTE格式（8 bit） :
```
  VALID | PFN6 ... PFN0
```
PDE格式（8 bit） :
```
  VALID | PT6 ... PT0
```
其
```
VALID==1表示，表示映射存在；VALID==0表示，表示映射不存在。
PFN6..0:页帧号
PT6..0:页表的物理基址>>5
```
在[物理内存模拟数据文件](./03-2-spoc-testdata.md)中，给出了4KB物理内存空间的值，请回答下列虚地址是否有合法对应的物理内存，请给出对应的pde index, pde contents, pte index, pte contents。
```
Virtual Address 6c74
Virtual Address 6b22
Virtual Address 03df
Virtual Address 69dc
Virtual Address 317a
Virtual Address 4546
Virtual Address 2c03
Virtual Address 7fd7
Virtual Address 390e
Virtual Address 748b
```

比如答案可以如下表示：
```
Virtual Address 7570:
  --> pde index:0x1d  pde contents:(valid 1, pfn 0x33)
    --> pte index:0xb  pte contents:(valid 0, pfn 0x7f)
      --> Fault (page table entry not valid)
      
Virtual Address 21e1:
  --> pde index:0x8  pde contents:(valid 0, pfn 0x7f)
      --> Fault (page directory entry not valid)

Virtual Address 7268:
  --> pde index:0x1c  pde contents:(valid 1, pfn 0x5e)
    --> pte index:0x13  pte contents:(valid 1, pfn 0x65)
      --> Translates to Physical Address 0xca8 --> Value: 16
```



（3）请基于你对原理课二级页表的理解，并参考Lab2建页表的过程，设计一个应用程序（可基于python, ruby, C, C++，LISP等）可模拟实现(2)题中描述的抽象OS，可正确完成二级页表转换。


（4）假设你有一台支持[反置页表](http://en.wikipedia.org/wiki/Page_table#Inverted_page_table)的机器，请问你如何设计操作系统支持这种类型计算机？请给出设计方案。

 (5)[X86的页面结构](http://os.cs.tsinghua.edu.cn/oscourse/OS2015/lecture06#head-1f58ea81c046bd27b196ea2c366d0a2063b304ab)
--- 

## 扩展思考题

阅读64bit IBM Powerpc CPU架构是如何实现[反置页表](http://en.wikipedia.org/wiki/Page_table#Inverted_page_table)，给出分析报告。

--- 
