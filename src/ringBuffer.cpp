//
// Created by os on 5/16/24.
//

#include "../h/ringBuffer.hpp"

ringBuffer::ringBuffer(char type): length(256), head(0), tail(0), count(0), type(type){
    sem_open(&this->itemAvailable, 0);
    if(type == 'P') sem_open(&this->spaceAvailable, length);
}
ringBuffer::~ringBuffer() {
    sem_close(this->itemAvailable);
    if(type == 'P') sem_close(this->spaceAvailable);
}



void ringBuffer::put(char c) {
    if(type == 'P') this->spaceAvailable->wait();
    if(count < length){
        buffer[tail] = c;
        tail++; tail %= length;
        count++;
        this->itemAvailable->signal();
    }
}

char ringBuffer::get() {
    this->itemAvailable->wait();
    char tmp = buffer[head];
    head++; head %= length;
    count--;
    if(type == 'P') this->spaceAvailable->signal();
    return tmp;
}





