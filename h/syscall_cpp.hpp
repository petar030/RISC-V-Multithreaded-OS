//
// Created by os on 5/15/24.
//

#ifndef PROJEKAT_SYSCALL_CPP_HPP
#define PROJEKAT_SYSCALL_CPP_HPP
#include "syscall_c.h"

void* operator new (size_t);
void operator delete (void*) noexcept;

class Thread {
public:
    Thread(void (*body)(void*), void* arg)
            :  myHandle(nullptr), body(body), arg(arg){}
    virtual ~Thread ();
    int start ();
    static void dispatch ();
    static int sleep (time_t);


protected:
    Thread () : Thread(nullptr, nullptr){}
    virtual void run ();
private:
    thread_t myHandle;
    void (*body)(void*); void* arg;

    static void runWrapper(void* arg) {
        Thread* self = (Thread*)(arg);
        self->run();
    }
};

class Semaphore {
public:
    explicit Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
    int timedWait (time_t);
    int tryWait();
private:
    sem_t myHandle;
};

class PeriodicThread : public Thread {
public:
    void terminate();

protected:
    explicit PeriodicThread(time_t period): Thread(nullptr, nullptr), period(period){}
    void run() override;
    virtual void periodicActivation();

private:
    time_t period;

};

class Console {
public:
    static char getc ();
    static void putc (char);
};
#endif //PROJEKAT_SYSCALL_CPP_HPP
