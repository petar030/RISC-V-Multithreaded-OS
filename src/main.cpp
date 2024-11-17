//
// Created by os on 5/8/24.
//
#include "../h/syscall_c.h"
#include "../h/syscall_cpp.hpp"








// OFFICIAL TESTING MAIN
extern void userMain();
extern void testingMain();

void test(void*){
    userMain();
    //testingMain();
}




int main(){
 //INIT START
    Riscv::w_stvec((uint64) Riscv::supervisor_trap | 0b00);
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
    // Main Thread
    thread_t main;
    thread_create(&main, nullptr, nullptr);
    TCB::running = main;
    //Putc Thread
    Riscv::getcBuffer = new ringBuffer('G');
    Riscv::putcBuffer = new ringBuffer('P');
    Thread putcThread(sysConsole::putcThread, nullptr);
    putcThread.start();

    //Test Thread
    thread_t testThread;
    thread_create(&testThread, &test, nullptr);
    printString("\n");
    printString("Main: ");
    print_char_pointer_value((char*)TCB::running);
    printString("\n");
    user_mode_start();
//INIT END




    while(!testThread->isFinished()) { thread_dispatch();};





//FINISH
    printString("\nEnd\n");
    user_mode_end();
    sysConsole::putcFlush();
    delete testThread;
    delete main;
    delete Riscv::putcBuffer;
    delete Riscv::getcBuffer;
    sysSemaphore::removeClosed();
    __asm__ volatile("li a0, 0x5555");
    __asm__ volatile("li a1, 0x100000");
    __asm__ volatile("sw a0, 0(a1)");
//FINISH END


    return 0;
}



