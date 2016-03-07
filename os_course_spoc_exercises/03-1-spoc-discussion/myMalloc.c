/*
 * 参考http://github.com/mit-pdos/xv6-public/blob/master/umalloc.c
 * 最先匹配算法
 */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

typedef long Align;/*for alignment to long boundary*/
union header {
    struct {
        union header *ptr; /*next block if on free list*/
        unsigned int size; /*size of this block*/
    } s;
    Align x;
};

typedef union header Header;

static Header base;
static Header *freep = NULL;//空闲列表

void free(void *sp);

#define NALLOC 1024    /* minimum #units to request */
static Header *morecore(unsigned int nu)//每次向os额外申请的大小最少是NALLOC
{
    char *cp;
    Header *up;
    if(nu < NALLOC)
        nu = NALLOC;
    cp = sbrk(nu * sizeof(Header));//sbrk申请空间
    if(cp == (char *)-1)    /* no space at all*/
        return NULL;
    up = (Header *)cp;
    up->s.size = nu;//申请之后的空闲地址空间
    free((void *)(up+1));
    return freep;//返回修改之后的空闲列表
}

void *malloc(unsigned int nbytes)
{
    Header *p, *prevp;
    unsigned int nunits;
    nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) + 1;//取整
    if((prevp = freep) == NULL) { /* no free list */
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }
    for(p = prevp->s.ptr; ;prevp = p, p= p->s.ptr) {
        if(p->s.size >= nunits) { /* big enough */
            if (p->s.size == nunits)  /* exactly */
                prevp->s.ptr = p->s.ptr;//把整个都分配出去，
            else {//被选中的区域部分分配出去，但是还有区域留在空闲列表中
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
        freep = prevp;
            return (void*)(p+1);//最先分配
        }
        if (p== freep) /* wrapped around free list */
            if ((p = morecore(nunits)) == NULL)
                return NULL; /* none left */
    }
}


void free(void *ap)//释放空间：增加freep列表的长度，合并
{
    Header *bp,*p;
    bp = (Header *)ap -1; /* point to block header */
    for(p=freep;!(bp>p && bp< p->s.ptr);p=p->s.ptr)
        if(p>=p->s.ptr && (bp>p || bp<p->s.ptr))
            break;    /* freed block at start or end of arena*/
    if (bp+bp->s.size==p->s.ptr) {    /* join to upper nbr */
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else
        bp->s.ptr = p->s.ptr;
    if (p+p->s.size == bp) {     /* join to lower nbr */
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else
        p->s.ptr = bp;
    freep = p;
}

int main(){
    printf("test begin...\n");
    void *p = malloc(256);
    void *q = malloc(64);
    if(p && q) printf("malloc right!!\n");
    else printf("malloc wrong\n");
    free(q);
    free(p);
    printf("test right...\n");
    return 0;
}
