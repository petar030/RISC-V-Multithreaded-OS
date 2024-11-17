//
// Created by os on 5/6/24.
//
#include "../h/MemoryAllocator.hpp"
bool MemoryAllocator::exists = false;
MemoryAllocator::FreeNode* MemoryAllocator::freeHead = nullptr;
MemoryAllocator::FullNode* MemoryAllocator::fullHead = nullptr;

void MemoryAllocator::printLists(){
    FreeNode* free = freeHead;
    FullNode* full = fullHead;
    while(free){
        print_char_pointer_value((char*)free);
        __putc(' ');
        print_char_pointer_value((char*)free + (free->size*MEM_BLOCK_SIZE)-1);
        __putc(' ');
        __putc('\n');
        free = free->next;
    }

    __putc('\n');
    __putc('\n');
    while(full){
        print_char_pointer_value((char*) full);
        __putc(' ');
        print_char_pointer_value((char*)full + (full->size * MEM_BLOCK_SIZE) - 1);
        __putc('\n');
        full = full->next;
    }
}





void MemoryAllocator::init(){
    exists = true;
    freeHead = (FreeNode*) HEAP_START_ADDR;
    freeHead->next = nullptr;
    freeHead->prev = nullptr;
    freeHead->size = ((size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR) / MEM_BLOCK_SIZE;
}

void* MemoryAllocator::malloc(size_t size) {
    if(size <= 0) return nullptr;
    if(!MemoryAllocator::exists) MemoryAllocator::init();
    size_t blockSize, maxSize;
    maxSize = size + sizeof(FullNode);
    //blockSize represents size in blocks
    blockSize = maxSize / MEM_BLOCK_SIZE;
    if(maxSize % MEM_BLOCK_SIZE != 0) blockSize += 1;

    //Finding first fit free block
    FreeNode* tmp = freeHead;
    while(tmp){
        if(tmp->size >= blockSize) break;
        tmp = tmp->next;
    }
    //No enough free space
    if(tmp == nullptr) return nullptr;

    //Making new FreeNode if needed
    if(tmp->size > blockSize){
        FreeNode* newFree = (FreeNode*)((char*)tmp + (blockSize*MEM_BLOCK_SIZE));
        newFree->size = tmp->size - blockSize;
        newFree->prev = tmp->prev;
        newFree->next = tmp->next;
        if(!newFree->prev) freeHead = newFree;
        else newFree->prev->next = newFree;
        if(newFree->next) newFree->next->prev = newFree;
    }
    //Else just relinking existing free nodes
    else{
        if(!tmp->prev) freeHead = tmp->next;
        else tmp->prev->next = tmp->next;
        if(tmp->next) tmp->next->prev = tmp->prev;
    }


    //Allocating new FullNode
    FullNode* newNode = (FullNode*)tmp;
    newNode->size = blockSize;
    newNode->next = newNode->prev = nullptr;
    FullNode* nextFull = fullHead;
    FullNode* prev = nullptr;
    //Finding position of newNode
    while(nextFull){
        if((char*)nextFull > (char*) newNode) break;
        prev = nextFull;
        nextFull = nextFull->next;
    }
    //If next doesn't exist
    if(!nextFull){
        if(!prev) fullHead = newNode;
        else{
            prev->next = newNode;
            newNode->prev = prev;
        }
    }
    //If next does exist
    else{
        if(!prev){
            fullHead = newNode;
            newNode->next = nextFull;
            nextFull->prev = newNode;
        }
        else{
            prev->next = newNode;
            newNode->prev = prev;
            newNode->next = nextFull;
            nextFull->prev = newNode;
        }
    }




    //Returning free space (after the FullNode heading)
    return (void*)((char*)newNode + sizeof(FullNode));

}

int MemoryAllocator::free(void* ptr){
    if(!MemoryAllocator::exists) return -1;
    FullNode* tmpFull = (FullNode*)((char*)ptr - sizeof(FullNode));
    FullNode* iterator = fullHead;
    while(iterator){
        if(iterator == tmpFull) break;
        iterator = iterator->next;
    }
    if(!iterator) return -1;

    //Unlink tmpFull from the list
    if(tmpFull->prev) tmpFull->prev->next = tmpFull->next;
    else fullHead = tmpFull->next;
    if(tmpFull->next) tmpFull->next->prev = tmpFull->prev;

    //Find previous and next FreeNode
    FreeNode* nextFree = freeHead;
    FreeNode* prevFree = nullptr;
    while(nextFree) {
        if ((char *) nextFree > (char *) tmpFull) break;
        nextFree = nextFree->next;
    }
    if(nextFree) prevFree = nextFree->prev;

    //Merge or link with previous if exists
    FreeNode* newFree = (FreeNode*) tmpFull;
    size_t tmpSize = tmpFull->size;
    if(prevFree){
        //Check if mergable
        if((FreeNode*)((char*)prevFree + (prevFree->size * MEM_BLOCK_SIZE)) == newFree){
            newFree = prevFree;
            newFree->size += tmpSize;
        }
        //Link if not
        else{
            prevFree->next = newFree;
            newFree->prev = prevFree;
            newFree->size = tmpSize;
        }

    }
    //If previous doesn't exist make a
    else{
        freeHead = newFree;
        newFree->prev = nullptr;
        newFree->size = tmpSize;
    }

    //Merge or link with next if exists
    if(nextFree){
        if((FreeNode*)((char*)newFree + (newFree->size * MEM_BLOCK_SIZE)) == nextFree){
            newFree->size += nextFree->size;
            newFree->next = nextFree->next;
            if(newFree->next) newFree->next->prev = newFree;
        }
        else{
            newFree->next = nextFree;
            nextFree->prev = newFree;
        }

    }
    else newFree->next = nullptr;


    return 0;

}

