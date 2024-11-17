//
// Created by os on 5/16/24.
//

#ifndef PROJEKAT_SYSCONSOLE_HPP
#define PROJEKAT_SYSCONSOLE_HPP
#include "riscv.hpp"


class sysConsole {
    friend class Riscv;
protected:
    static void console_handler();

public:
    static void putcHandler(void *arg);

    static void putcThread(void *arg);

    static void putcFlush();
};


#endif //PROJEKAT_SYSCONSOLE_HPP
