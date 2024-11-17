//
// Created by os on 5/7/24.
//

#ifndef PROJEKAT_SYSCALL_C_H
#define PROJEKAT_SYSCALL_C_H
#include "../lib/hw.h"
#include "../lib/console.h"
#include "list.hpp"
#include "riscv.hpp"
#include "printing.hpp"
#include "MemoryAllocator.hpp"
#include "TCB.hpp"
#include "sysSemaphore.hpp"
#include "WaitList.hpp"
#include "Scheduler.hpp"
#include "ringBuffer.hpp"



class TCB;
class sysSemaphore;
using thread_t = TCB*;
using Body = void(*)(void*);
using sem_t = sysSemaphore* ;
using time_t = uint64;


void system_call(uint64 code, ...);
void* mem_alloc(size_t size);
int mem_free(void* ptr);
void thread_dispatch();
int thread_create(thread_t* handle, Body body, void* arg);
int thread_exit();
int sem_wait(sem_t id);
int sem_signal(sem_t id);
int sem_open(sem_t* handle, unsigned init);
int sem_close(sem_t handle);
int sem_trywait(sem_t handle);
int time_sleep(time_t time);
char getc();
void putc(char c);
int sem_timedwait(sem_t id, time_t timeout);
void putc_handler();
void user_mode_start();
void user_mode_end();



enum codes{
    MEM_ALLOC = 0x01,
    MEM_FREE = 0x02,
    CREATE_THREAD = 0x11,
    EXIT_THREAD = 0x12,
    DISPATCH = 0x13,
    SEM_OPEN = 0x21,
    SEM_CLOSE = 0x22,
    SEM_WAIT = 0x23,
    SEM_SIGNAL = 0x24,
    SEM_TIMEDWAIT = 0x25,
    SEM_TRYWAIT = 0x26,
    TIME_SLEEP = 0x31,
    GETC = 0x41,
    PUTC = 0x42,
    PUTC_HANDLER = 0x43,
    USER_MODE_START = 0x44,
    USER_MODE_END = 0x45
};


#endif //PROJEKAT_SYSCALL_C_H
