//
// Created by os on 5/13/24.
//

#include "../h/sysSemaphore.hpp"

List<sysSemaphore> sysSemaphore::listOfClosed;

int sysSemaphore::wait(){
    if(this->closed) return -1;
    if((int)--this->val < 0){
        //Block running TCB
        TCB::running->setBlock(true);
        this->blocked.addLast(TCB::running);
        //Dispatch
        thread_dispatch();
        //Check if sem was closed while waiting
        if(this->closed) return -1;
    }
    return 0;

}

int sysSemaphore::signal(){
    if(this->closed) return -1;
    if((int)++val <= 0){
        thread_t unblocked;
        do{
            unblocked = this->blocked.removeFirst();
        }while(unblocked && !unblocked->isBlocked);

        if(unblocked && unblocked->isBlocked()){
            unblocked->setBlock(false);
            Scheduler::put(unblocked);
        }
    }
    return 0;
}

int sysSemaphore::close(){
    if(this->closed) return -1;
    closed = true;
    while(this->blocked.peekFirst()){
        thread_t unblocked = this->blocked.removeFirst();
        unblocked->setBlock(false);
        Scheduler::put(unblocked);
    }
    listOfClosed.addLast(this);
    return 0;
}

int sysSemaphore::tryWait(){
    if(this->closed) return -1;
    int tmp = (int)this->val;
    tmp--;
    if(tmp < 0) return 1;
    --val;
    return 0;
}

int sysSemaphore::timedWait(time_t time) {
    if(this->closed) return -1;
    if((int)--this->val < 0){
        //Block running TCB
        TCB::running->setBlock(true);
        this->blocked.addLast(TCB::running);
        time_sleep(time);
        bool stillBlocked= TCB::running->isBlocked();
         TCB::running->setBlock(false);
        TCB::running->setSleep(false);
        if(this->closed) return -1; //SEMDEAD
        if(stillBlocked) return -2; //TIMEOUT
       
    }
    return 0;
}



void* sysSemaphore::operator new(size_t size) {
    return MemoryAllocator::malloc(size);
}

void sysSemaphore::operator delete(void* ptr) noexcept {
    MemoryAllocator::free(ptr);
}

void *sysSemaphore::operator new[](size_t size) {
    return MemoryAllocator::malloc(size);
}

void sysSemaphore::operator delete[](void *ptr) noexcept{
    MemoryAllocator::free(ptr);
}

int sysSemaphore::getVal() const {
    return (int)this->val;
}

void sysSemaphore::removeClosed() {
    sysSemaphore* curr = nullptr;
    while((curr = listOfClosed.removeFirst()) != nullptr){
        delete curr;
    }
}




