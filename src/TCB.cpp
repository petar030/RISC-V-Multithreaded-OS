//
// Created by os on 5/8/24.
//

#include "../h/TCB.hpp"


TCB* TCB::running = nullptr;
uint64 TCB::timeSliceCounter = 0;


void TCB::dispatch(){
    TCB* old = running;
   if(!old->isFinished() && !old->isBlocked() && !old->isSleeping()){
        Scheduler::put(old);
    }
    running = Scheduler::get();
    if(old != running)TCB::contextSwitch(&old->context, &running->context);

}

void TCB::yield() {
    thread_dispatch();
}

TCB* TCB::createThread(Body body, void* arg, void* stack){
    if(stack == nullptr && body != nullptr) stack = new uint64[DEFAULT_STACK_SIZE];
    TCB* newTCB = new TCB(body, arg, (uint64*) stack,DEFAULT_TIME_SLICE);
    if(body) Scheduler::put(newTCB);

    return newTCB;
}

void TCB::threadWrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    TCB::yield();
}

void* TCB::operator new(size_t size) {
    return MemoryAllocator::malloc(size);
}

void TCB::operator delete(void* ptr) noexcept {
    MemoryAllocator::free(ptr);
}

void *TCB::operator new[](size_t size) {
    return MemoryAllocator::malloc(size);
}

void TCB::operator delete[](void *ptr) noexcept{
    MemoryAllocator::free(ptr);
}


