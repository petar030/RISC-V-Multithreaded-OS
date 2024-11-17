//
// Created by os on 5/7/24.
//

#include "../h/syscall_c.h"


using thread_t = TCB*;


void system_call(uint64 code, ...){
    __asm__ volatile("ecall");
}



void* mem_alloc(size_t size){
    system_call(MEM_ALLOC, size);
    void* retVal;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));
    return retVal;
}

int mem_free(void* ptr){
    system_call(MEM_FREE, ptr);
    int retVal;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));
    return retVal;
}

void thread_dispatch(){
    system_call(DISPATCH);
}

int thread_create(thread_t* handle, Body body, void* arg){
    system_call(CREATE_THREAD, handle, body, arg);
    int retVal;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));
    return retVal;
}

int thread_exit(){
    system_call(EXIT_THREAD);
    int retVal;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));
    return retVal;
}

int sem_wait(sem_t id){
    system_call(SEM_WAIT, id);
    int retVal;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));
    return retVal;
}

int sem_signal(sem_t id){
    system_call(SEM_SIGNAL, id);
    int retVal;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));
    return retVal;
}

int sem_open(sem_t* handle, unsigned init){
    system_call(SEM_OPEN, handle, init);
    int retVal;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));
    return retVal;
}

int sem_close(sem_t handle){
    system_call(SEM_CLOSE, handle);
    int retVal;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));
    return retVal;
}

int sem_trywait(sem_t handle){
    system_call(SEM_TRYWAIT, handle);
    int retVal;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));
    return retVal;
}

char getc(){
    system_call(GETC);
    char retVal;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));
    if(retVal == '\r')
        retVal = '\n';
    return retVal;
}
void putc(char c){
    system_call(PUTC, c);
}

int time_sleep(time_t time){
    if((int)time <= 0) return -1;
    system_call(TIME_SLEEP, time);
    uint64 retVal;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));
    return retVal;
}

int sem_timedwait(sem_t id, time_t timeout){
    system_call(SEM_TIMEDWAIT, id, timeout);
    uint64 retVal;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));
    return retVal;

}

void putc_handler(){
    system_call(PUTC_HANDLER);
}

void user_mode_start(){
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    Riscv::user_mode = true;
}

void user_mode_end(){
    Riscv::user_mode = false;
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
}



