#include "../h/WaitList.hpp"

Node *WaitList::head = nullptr;





void WaitList::insert(TCB *thread, time_t time) {
    Node* newNode = new Node(TCB::running, time);
    Node *curr = head;
    Node* prev = nullptr;
    while(curr){
        if(curr->relativeWakeTime >= newNode->relativeWakeTime) break;
        newNode->relativeWakeTime -= curr->relativeWakeTime;
        prev = curr;
        curr = curr->next;
    }
    if(head == curr){
        head = newNode;
    }
    if(prev){
        prev->next = newNode;
    }
    newNode->next = curr;
    if(newNode->next) newNode->next->relativeWakeTime -= newNode->relativeWakeTime;
}
TCB* WaitList::remove() {
    if (!head) return nullptr;
    Node* node = head;
    TCB* retVal = head->thread;
    head = head->next;
    delete node;
    return retVal;
}




int WaitList::sleep(time_t time) {
    if (!TCB::running || (int) time <= 0) return -1;
    WaitList::insert(TCB::running, time);
    TCB::running->setSleep(true);
    thread_dispatch();
    return 0;
}


void WaitList::update() {
    if(WaitList::head) WaitList::head->relativeWakeTime--;
    while(head && head->relativeWakeTime <= 0){
        TCB* removeTCB = WaitList::remove();
        if(removeTCB && removeTCB->isSleeping()){
            removeTCB->setSleep(false);
            Scheduler::put(removeTCB);
        }
    }
}
