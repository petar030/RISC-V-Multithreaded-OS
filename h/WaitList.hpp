#ifndef PROJEKAT_WAIT_LIST_HPP
#define PROJEKAT_WAIT_LIST_HPP

#include "syscall_c.h"
#include "MemoryAllocator.hpp"

class TCB;
class Node {
    TCB* thread;
    time_t relativeWakeTime;
    Node* next;

    Node(TCB *thread, time_t time) : thread(thread), relativeWakeTime(time), next(nullptr) {}


    friend class WaitList;
};

class WaitList {
public:
    static Node *head;
    static void insert(TCB* thread, time_t time);
    static TCB* remove();
    static int sleep(time_t time);
    static void update();




};


#endif //PROJEKAT_WAIT_LIST_HPP