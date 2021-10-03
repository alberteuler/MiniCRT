#include "minicrt.h"

typedef struct _heap_header {
    enum {
        HEAP_BLOCK_FREE = 0xABABABAB,
        HEAP_BLOCK_USED = 0xCDCDCDCD,
    } type;

    unsigned size;
    struct _heap_header* next;
    struct _heap_header* prev;
} heap_header;

#define ADDR_ADD(a,o) (((char*)(a)) + o)
#define HEADER_SIZE  sizeof(heap_header)

static heap_header* list_head = NULL;

void free(void *ptr) {
    heap_header* header = (heap_header*)ADDR_ADD(ptr, -HEADER_SIZE);
    if(header->type != HEAP_BLOCK_USED) return; //header是空的,说明整个内存都是空的
    header->type = HEAP_BLOCK_FREE; 
    if(header->prev != NULL && header->prev->type == HEAP_BLOCK_FREE) {
        header->prev->next = header->next;
        if(header->next != NULL) {
            header->next->prev = header->prev;
        }
        header->prev->size += header->size;
        header = header->prev;
    }
    if(header->next != NULL && header->next->type == HEAP_BLOCK_FREE){
        //merge
        header->size += header->next->size;
        header->next = header->next->next;
    }
}

void* malloc(unsigned size) {
    heap_header *header;
    if(size == 0) return NULL;

    header = list_head;
    while(header != NULL) {
        //跳过被使用的内存块
        if(header->type == HEAP_BLOCK_USED){
            header = header->next;
            continue;
        //内存块的大小刚好满足需求
        }else if(header->size > size + HEADER_SIZE && 
                 header->size <= size + HEADER_SIZE * 2) {
            header->type = HEAP_BLOCK_USED;
        }else{


        //内存块大小过大，将内存分割
            heap_header* next = (heap_header*) ADDR_ADD(header, size + HEADER_SIZE);
            next->prev = header;
            next->next = header->next;
            next->type = HEAP_BLOCK_FREE;
            next->size = header->size - (size - HEADER_SIZE);
            header->next = next;
            header->size = size + HEADER_SIZE;
            header->type = HEAP_BLOCK_USED;
            return ADDR_ADD(header, HEADER_SIZE);
        }
        header = header->next;
    }
    return NULL;
}

#ifndef WIN32

static int brk(void* end_data_segment) {
    int ret = 0;
    //为什么是45？因为brk的系统调用就是45.
    asm("movl $45, %%eax     \n\t"
        "movl %1, %%ebx      \n\t"
            "int $0x80       \n\t"
            : "=r"(ret):"m"(end_data_segment));
}
#endif

#ifdef WIN32
#include <Windows.h>
#endif

int mini_crt_heap_init() {
    void *base = NULL;
    heap_header *header = NULL;
    unsigned heap_size = 1024 * 1024 * 32;  //内存大小被设定为32MB

//为自由链表申请空间
#ifdef WIN32
    base = VirtualAlloc(0, heap_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if(base == NULL) return 0;
#else
    base = (void*) brk(0);
    void* end = ADDR_ADD(base, heap_size);
    end = (void*)brk(end);
    if(end == NULL) return 0;
#endif
    //初始化自由链表的头部
    header = (heap_header*)base;
    header->size = heap_size;
    header->type = HEAP_BLOCK_FREE;
    header->next = NULL;
    header->prev = NULL;
    list_head = header;
    return 1;
}