//
// Created by os on 5/18/22.
//

#include "../h/printing.hpp"

uint64 lockPrint = 0;

#define LOCK() while(copy_and_swap(lockPrint, 0, 1)) thread_dispatch()
#define UNLOCK() while(copy_and_swap(lockPrint, 1, 0))

void printString(char const *string)
{
    LOCK();
    while (*string != '\0')
    {
        putc(*string);
        string++;
    }
    UNLOCK();
}

char* getString(char *buf, int max) {
    LOCK();
    int i, cc;
    char c;

    for(i=0; i+1 < max; ){
        cc = getc();
        if(cc < 1)
            break;
        c = cc;
        buf[i++] = c;
        if(c == '\n' || c == '\r')
            break;
    }
    buf[i] = '\0';

    UNLOCK();
    return buf;
}

int stringToInt(const char *s) {
    int n;

    n = 0;
    while ('0' <= *s && *s <= '9')
        n = n * 10 + *s++ - '0';
    return n;
}

char digits[] = "0123456789ABCDEF";

void printInt(int xx, int base, int sgn)
{
    LOCK();
    char buf[16];
    int i, neg;
    uint x;

    neg = 0;
    if(sgn && xx < 0){
        neg = 1;
        x = -xx;
    } else {
        x = xx;
    }

    i = 0;
    do{
        buf[i++] = digits[x % base];
    }while((x /= base) != 0);
    if(neg)
        buf[i++] = '-';

    while(--i >= 0)
        putc(buf[i]);

    UNLOCK();
}

void print_char_pointer_value(const char* ptr) {
    LOCK();
    // Convert the memory address to hexadecimal
    unsigned long address = (unsigned long)ptr;
    char buffer[17]; // Buffer to store hexadecimal representation (16 characters + null terminator)
    int i;
    buffer[16] = '\0'; // Null-terminate the buffer

    int x = 0;
    // Print the memory address character by character
    for (i = 15; i >= 0; i--) {
        int digit = address & 0xF; // Get the least significant nibble
        buffer[i] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10); // Convert to hexadecimal character
        address >>= 4; // Shift right by 4 bits for the next nibble
        if(address == 0){
            x = i;
            break;
        }
    }

    // Print the hexadecimal representation using putc
    for (i = x; i < 16; i++) {
        putc(buffer[i]);
        }
    UNLOCK();
}