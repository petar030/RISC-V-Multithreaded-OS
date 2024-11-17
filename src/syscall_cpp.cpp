//
// Created by os on 5/15/24.
//
#include "../h/syscall_cpp.hpp"
#include "../lib/mem.h"



using size_t = decltype(sizeof(0));

void *operator new(size_t n)
{
    return mem_alloc(n);
}

void *operator new[](size_t n)
{
    return mem_alloc(n);
}


void operator delete(void *p) noexcept
{
    mem_free(p);
}

void operator delete[](void *p) noexcept
{
    mem_free(p);
}

//Threads
Thread::~Thread(){
    delete myHandle;
}
int Thread::start() {
    if(this->body){
        return thread_create(&this->myHandle, this->body, this->arg);
    }
    else{
        return thread_create(&this->myHandle, runWrapper, this);
    }
}
void Thread::dispatch() {
    thread_dispatch();
}
int Thread::sleep(time_t time) {
    if((int)time <= 0) return -1;
    return time_sleep(time);
}
void Thread::run() {

}

//Semaphore
Semaphore::Semaphore(unsigned int init) {
    this->myHandle = nullptr;
    sem_open(&this->myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(this->myHandle);
}

int Semaphore::wait() {
    return sem_wait(this->myHandle);
}

int Semaphore::signal() {
    return sem_signal(this->myHandle);
}

int Semaphore::timedWait(time_t timeout) {
    return sem_timedwait(this->myHandle, timeout);
}

int Semaphore::tryWait() {
    return sem_trywait(this->myHandle);
}

void PeriodicThread::run() {
    while(!TCB::running->isFinished()){
        this->periodicActivation();
        time_sleep(this->period);
    }
}

void PeriodicThread::terminate() {
    thread_exit();
}

void PeriodicThread::periodicActivation() {

}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c){
    ::putc(c);
}
