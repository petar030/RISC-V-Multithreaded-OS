//
// Created by os on 5/16/24.
//

#ifndef PROJEKAT_RINGBUFFER_HPP
#define PROJEKAT_RINGBUFFER_HPP
#include "syscall_c.h"
#include "printing.hpp"

class sysSemaphore;
class ringBuffer {
friend class Riscv;
friend class sysConsole;
public:
    using sem_t = sysSemaphore*;
    explicit ringBuffer(char type);
    ~ringBuffer();
    char get();
    void put(char c);
    uint64 getCount() const{return this->count;}
protected:
    uint64 length;
    uint64 head, tail, count;
    sem_t itemAvailable;
    sem_t spaceAvailable;
    char buffer[256];
    char type; //'P' for putc, 'G' for getc




};


#endif //PROJEKAT_RINGBUFFER_HPP
