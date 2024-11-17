//
// Created by os on 5/13/24.
//

#ifndef PROJEKAT_SYSSEMAPHORE_HPP
#define PROJEKAT_SYSSEMAPHORE_HPP
#include "syscall_c.h"



class TCB;
class sysSemaphore {
public:
    explicit sysSemaphore(uint64 initVal = 1): val(initVal), closed(false){}
    int wait();
    int signal();
    int close();
    int tryWait();
    int timedWait(time_t time);
    int getVal() const;
    static List<sysSemaphore> listOfClosed;
    static void removeClosed();

    void* operator new(size_t size);
    void operator delete(void* ptr) noexcept;
    void* operator new[](size_t size);
    void operator delete[](void* ptr) noexcept;




private:
    uint64 val;
    bool closed;
    List<TCB> blocked;


};


#endif //PROJEKAT_SYSSEMAPHORE_HPP
