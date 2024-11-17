//
// Created by os on 5/8/24.
//

#ifndef PROJEKAT_SCHEDULER_HPP
#define PROJEKAT_SCHEDULER_HPP
#include "syscall_c.h"


class TCB;

class Scheduler
{
public:
    static List<TCB> readyCoroutineQueue;

    static TCB *get();

    static void put(TCB *TCB);

    void* operator new(size_t size);
    void operator delete(void* ptr) noexcept;
    void* operator new[](size_t size);
    void operator delete[](void* ptr)noexcept;






};

#endif //PROJEKAT_SCHEDULER_HPP
