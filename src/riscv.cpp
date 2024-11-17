
#include "../h/riscv.hpp"
using thread_t = TCB*;

ringBuffer* Riscv::putcBuffer = nullptr; //Initialized on heap in main (Started thread that handles output)
ringBuffer* Riscv::getcBuffer = nullptr; //Initialized on heap in main
bool Riscv::user_mode = false;



void Riscv::popSppSpie()
{
    if (user_mode) {
        mc_sstatus(Riscv::SSTATUS_SPP);
    }
    else {
        ms_sstatus(Riscv::SSTATUS_SPP);
    }
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::trapHandler(){
    uint64 volatile scause = Riscv::r_scause();
    uint64 volatile sepc = Riscv::r_sepc();
    uint64 volatile sstatus = Riscv::r_sstatus();
    uint64 volatile code;
    __asm__ volatile ("mv %0, a0" : "=r" (code));
    uint64 volatile a1;
    __asm__ volatile ("mv %0, a1" : "=r" (a1));
    uint64 volatile a2;
    __asm__ volatile ("mv %0, a2" : "=r" (a2));
    uint64 volatile a3;
    __asm__ volatile ("mv %0, a3" : "=r" (a3));

    //Not interrupt, environment call from ecall
    if(scause == 0x0000000000000009UL || scause == 0x0000000000000008UL){
        //Fetch sepc, status and code
        sepc += 4;



        switch(code){
            case MEM_ALLOC:{
                size_t volatile size = a1;
                void* volatile retVal = MemoryAllocator::malloc(size);
                __asm__ volatile ("mv a0, %0" : : "r" (retVal));
                break;

            }
            case MEM_FREE:{
                void* volatile ptr = (void*) a1;
                int volatile retVal = MemoryAllocator::free(ptr);
                __asm__ volatile ("mv a0, %0" : : "r" (retVal));
                break;
            }
            case DISPATCH:{
                TCB::timeSliceCounter = 0;
                TCB::dispatch();
                break;
            }
            case CREATE_THREAD:{
                thread_t*  handle;
                Body volatile body = (Body) a2;
                void* volatile arg = (void*) a3;
                int volatile retVal = 0;
                handle = (thread_t*) a1;
                if(handle == nullptr){
                    retVal = -1;
                    __asm__ volatile ("mv a0, %0" : : "r" (retVal));
                    break;
                }
                *handle = TCB::createThread(body, arg, nullptr);
                __asm__ volatile ("mv a0, %0" : : "r" (retVal));
                break;
            }
            case EXIT_THREAD:{
                TCB::running->setFinished(true);
                thread_dispatch();
                __asm__ volatile ("mv a0, %0" : : "r" (-1));
                break;

            }
            case SEM_WAIT:{
                sem_t volatile handle = (sem_t) a1;
                int volatile retVal = 0;
                if(handle == nullptr){
                    retVal = -1;
                }
                if(retVal == 0) retVal = handle->wait();
                __asm__ volatile ("mv a0, %0" : : "r" (retVal));
                break;
            }
            case SEM_SIGNAL:{
                sem_t volatile handle = (sem_t) a1;
                int volatile retVal = 0;
                if(handle == nullptr){
                    retVal = -1;
                }
                if(retVal == 0) retVal = handle->signal();
                __asm__ volatile ("mv a0, %0" : : "r" (retVal));
                break;
            }
            case SEM_OPEN:{
                sem_t* volatile handle = (sem_t*) a1;
                uint64 volatile init;
                int volatile retVal = 0;
                if(handle == nullptr){
                    retVal = -1;
                }
                if(retVal == 0){
                    init = (uint64) a2;
                    *handle = new sysSemaphore(init);
                }
                __asm__ volatile ("mv a0, %0" : : "r" (retVal));
                break;
            }
            case SEM_CLOSE:{
                sem_t volatile handle = (sem_t) a1;
                int volatile retVal = 0;
                if(handle == nullptr){
                    retVal = -1;
                }
                if(retVal == 0) retVal = handle->close();
                __asm__ volatile ("mv a0, %0" : : "r" (retVal));
                break;
            }
            case SEM_TRYWAIT:{
                sem_t volatile handle = (sem_t) a1;
                int volatile retVal = 0;
                if(handle == nullptr){
                    retVal = -1;
                }
                if(retVal == 0) retVal = handle->tryWait();
                __asm__ volatile ("mv a0, %0" : : "r" (retVal));
                break;
            }
            case SEM_TIMEDWAIT:{
                sem_t volatile handle = (sem_t) a1;
                int volatile retVal = 0;
                if(handle == nullptr){
                    retVal = -1;
                }
                if(retVal == 0){
                    time_t volatile time;
                    __asm__ volatile ("mv %0, a2" : "=r" (time));
                    retVal = handle->timedWait(time);
                }
                __asm__ volatile ("mv a0, %0" : : "r" (retVal));

                break;
            }

            case TIME_SLEEP:{
                uint64 volatile time = a1;
                int volatile retVal = WaitList::sleep(time);
                __asm__ volatile ("mv a0, %0" : : "r" (retVal));
                break;

            }
            case PUTC:{
                char volatile c = (char) a1;
                putcBuffer->put(c);
                break;
            }
            case GETC:{
                char volatile c;
                c = getcBuffer->get();
                __asm__ volatile ("mv a0, %0" : : "r" (c));
                break;
            }
            case PUTC_HANDLER:{
                sysConsole::putcHandler(nullptr);
                break;
            }

//            case USER_MODE_START:{
//                user_mode = true;
//                break;
//            }
//
//            case USER_MODE_END:{
//                user_mode = false;
//                break;
//            }



        }


        Riscv::w_sstatus(sstatus);
        Riscv::w_sepc(sepc);
    }
     //Interrupt caused by supervisor software interrupt (timer)
    else if(scause == 0x8000000000000001UL){
        mc_sip(SIP_SSIP);
        WaitList::update();
        TCB::timeSliceCounter++;
        if(TCB::timeSliceCounter >= TCB::running->timeSlice){
            TCB::timeSliceCounter = 0;
            TCB::dispatch();

        }
        Riscv::w_sstatus(sstatus);
        Riscv::w_sepc(sepc);

    }
        //Interrupt caused by supervisor external interrupt (console)
    else if(scause == 0x8000000000000009UL){
            scause = Riscv::r_scause();
            sepc = Riscv::r_sepc();
            sysConsole::console_handler();
            __asm__ volatile ("mv a0, %0" : : "r" (code));
            Riscv::w_sstatus(sstatus);
            Riscv::w_sepc(sepc);
    }
    else{
        printString("Scause = ");
        printInt(scause);
        printString("\nSepc = ");
        printInt(r_sepc());
        printString("\n");
        Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
        sysConsole::putcFlush();
        __asm__ volatile("li a0, 0x5555");
        __asm__ volatile("li a1, 0x100000");
        __asm__ volatile("sw a0, 0(a1)");
    }

}