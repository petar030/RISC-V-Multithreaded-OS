//
// Created by os on 5/8/24.
//

#include "../h/Scheduler.hpp"

List<TCB> Scheduler::readyCoroutineQueue;

TCB *Scheduler::get()
{
    return readyCoroutineQueue.removeFirst();
}

void Scheduler::put(TCB *TCB)
{
    readyCoroutineQueue.addLast(TCB);
}

void* Scheduler::operator new(size_t size) {
    return MemoryAllocator::malloc(size);
}

void Scheduler::operator delete(void* ptr) noexcept {
    MemoryAllocator::free(ptr);
}

void *Scheduler::operator new[](size_t size) {
    return MemoryAllocator::malloc(size);
}

void Scheduler::operator delete[](void *ptr) noexcept{
    MemoryAllocator::free(ptr);
}
