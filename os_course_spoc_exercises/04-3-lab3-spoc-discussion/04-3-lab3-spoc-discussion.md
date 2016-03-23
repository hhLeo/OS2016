# lab3 SPOC思考题

=======

## 小组思考题
---
(1)(spoc) 请参考lab3_result的代码，思考如何在lab3_results中实现clock算法，给出你的概要设计方案。可4人一个小组。要求说明你的方案中clock算法与LRU算法上相比，潜在的性能差异性。进而说明在lab3的LRU算法实现的可能性评价（给出理由）。
```
解：从ucore-lab3_result代码中可以看出，fifo所实现的页置换功能都被封装在swap_manager_fifo中，
所以，想要实现另一种置换算法，最好另外实现一个文件“swap_clock.c”,
并实现_clock_init_mm(),_clock_map_swappable(), _clock_swap_out_victim(), _clock_check_swap()等函数，
其中有些函数的实现没有什么变化，主要不同之处在于_clock_swap_out_victim()和_clock_init_mm()中，
这些函数也会封装在swap_manager_clock中，成为相应的接口（init_mm， map_swappable， swap_out_victim等）操作。
    在_clock_init_mm()中，初始化环形链表，指针指向最先调入的页面， 设置访问位为零。
    在_clock_swap_out_victim()中，按照clock算法的内容，从指针当前位置顺序检查环形链表；若访问位是0，则置换，若访问位是1，则把访问位设0；指针移动到下一项；重复操作，找到置换页。

    时钟算法是LRU和FIFO的折中，性能比FIFO要好，实现比LRU简单，因为对过去信息的考虑比LRU要少，开销较小。
'''

