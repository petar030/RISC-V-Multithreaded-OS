//
// Created by os on 5/8/24.
//

#ifndef PROJEKAT_TCB_HPP
#define PROJEKAT_TCB_HPP
#include "../lib/hw.h"
#include "syscall_c.h"

class TCB {
public:
    ~TCB(){delete[] stack;}
    static TCB* running;
    using Body = void(*)(void*);
    static TCB* createThread(Body body, void* arg, void* stack);
    bool isFinished(){return finished;}
    void setFinished(bool set){this->finished = set;}
    static void yield();
    uint64 getTimeSlice(){return timeSlice;}
    void setBlock(bool val){this->blocked = val;}
    bool isBlocked(){return this->blocked;}
    void setSleep(bool val){this->sleeping = val;}
    bool isSleeping(){return this->sleeping;}
    void* operator new(size_t size);
    void operator delete(void* ptr) noexcept;
    void* operator new[](size_t size);
    void operator delete[](void* ptr)noexcept;








private:
    friend class Riscv;
    Body body;
    uint64* stack;
    struct Context{
        uint64 ra;
        uint64 sp;
    };
    Context context;
    bool finished;
    uint64 timeSlice;
    void* arg;
    bool blocked;
    bool sleeping;
    static void contextSwitch(Context* oldContext, Context* newContext);
    static void dispatch();
    static void threadWrapper();
    static uint64 timeSliceCounter;
    explicit TCB(Body body, void* arg, uint64* stack, uint64 timeSlice):
        body(body),
        stack(body == nullptr ? nullptr : (uint64*) stack),
        context({
            (uint64) &threadWrapper,
            stack == nullptr ? 0 : (uint64) &this->stack + (uint64)DEFAULT_STACK_SIZE - 1
        }),
        finished(false),
        timeSlice(timeSlice),
        arg(arg),
        blocked(false),
        sleeping(false){}

};


#endif //PROJEKAT_TCB_HPP
