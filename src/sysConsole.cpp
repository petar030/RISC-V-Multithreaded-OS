//
// Created by os on 5/16/24.
//

#include "../h/sysConsole.hpp"

void sysConsole::console_handler() {
    int code = plic_claim();
    char tmp;
    int i = 0;
    if(code == 10){
        while((CONSOLE_RX_STATUS_BIT & *((char*)CONSOLE_STATUS)) && i < 50){
            tmp = *((char*)CONSOLE_RX_DATA);
            Riscv::getcBuffer->put(tmp);
            i++;
        }
        plic_complete(code);
    }
    else{
        printString("Unrecognized supervisor external interrupt\nPLIC code: ");
        printInt(code);

        __asm__ volatile("li a0, 0x5555");
        __asm__ volatile("li a1, 0x100000");
        __asm__ volatile("sw a0, 0(a1)");
    }
}


void sysConsole::putcHandler(void* arg) {
    char tmp;
    int i;
    while(true){
        i = 0;
        while((CONSOLE_TX_STATUS_BIT & *((char*)CONSOLE_STATUS)) && i < 50){
            tmp = Riscv::putcBuffer->get();
            *((char*)CONSOLE_TX_DATA) = tmp;
            i++;
        }
    }
}

void sysConsole::putcThread(void *arg) {
    putc_handler();
}

void sysConsole::putcFlush(){
    char tmp;
    while(Riscv::putcBuffer->getCount() > 0){
        while((CONSOLE_TX_STATUS_BIT & *((char*)CONSOLE_STATUS))){
            tmp = Riscv::putcBuffer->buffer[Riscv::putcBuffer->head];
            Riscv::putcBuffer->head++; Riscv::putcBuffer->head %= Riscv::putcBuffer->length;
            Riscv::putcBuffer->count--;
            *((char*)CONSOLE_TX_DATA) = tmp;
            if(Riscv::putcBuffer->count <= 0) break;
        }
    }
}
