//
// Created by os on 5/6/24.
//

#ifndef PROJEKAT_MEMORYALLOCATOR_HPP
#define PROJEKAT_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"
#include "../lib/console.h"
#include "syscall_c.h"



class MemoryAllocator {

public:
    struct FreeNode{
        FreeNode* next;
        FreeNode* prev;
        size_t size;
    };

    struct FullNode{
        FullNode* next;
        FullNode* prev;
        size_t size;
    };
private:

    static bool exists;
    //Private constructor to prevent making an instance
    MemoryAllocator();
    ~MemoryAllocator();

public:
    //Constructors:
    static FullNode* fullHead;
    static FreeNode* freeHead;
    MemoryAllocator(const MemoryAllocator& mem) = delete;
    MemoryAllocator& operator=(const MemoryAllocator& mem) = delete;
    static void init();
    static void* malloc(size_t size);
    static int free(void* ptr);
    static void printLists();





};


#endif //PROJEKAT_MEMORYALLOCATOR_HPP
