 //
// Created by os on 7/20/24.
//
#include "../h/memory_allocator.hpp"

MemoryAllocator::Node* MemoryAllocator::fmem_head = nullptr;

void MemoryAllocator::initMemoryAllocator() {
    fmem_head = (MemoryAllocator::Node*)HEAP_START_ADDR;
    fmem_head->size = (size_t)((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR) - sizeof(Node);
}

//2. zadatak, drugi kolokvijum, april 2014.
void* MemoryAllocator::mem_alloc(size_t size) {
    if (size <= 0) return nullptr;

    size_t sizeToTake = size / MEM_BLOCK_SIZE;
    size_t temp = size % MEM_BLOCK_SIZE;
    if (temp) sizeToTake++;
    sizeToTake *= MEM_BLOCK_SIZE;

    Node* cur = fmem_head, *prev = nullptr;
    for(; cur != nullptr; prev = cur, cur = cur->next){
        if(cur->size >= size)
            break;
    }

    if(cur == nullptr) return nullptr;

    size_t remainingSize = cur->size - sizeToTake;
    if (remainingSize < sizeof(Node) + MEM_BLOCK_SIZE) {
        if (prev) prev->next = cur->next;
        else fmem_head = cur->next;
    }
    else {
        cur->size = sizeToTake;
        Node* newFragment = (Node*)((char*)cur + sizeToTake + sizeof(Node));
        newFragment->size = remainingSize - sizeof(Node);
        if (prev) prev->next = newFragment;
        else fmem_head = newFragment;
        if (cur->next) newFragment->next = cur->next;
    }
    cur->next = nullptr;
    cur++;
    return cur;
}

//2. zadatak, drugi kolokvijum, septembar 2015.
int MemoryAllocator::tryToJoin(Node *cur) {
    if (!cur) return 0;
    if (cur->next && (char*)cur + sizeof(Node) + cur->size == (char*)cur->next) {
        cur->size += cur->next->size;
        cur->next = cur->next->next;
        return 1;
    }
    else return 0;
}

int MemoryAllocator::mem_free(void *addr) {
    if(!addr) return -1;

    Node* cur = nullptr;
    if (fmem_head == nullptr || (char*) addr < (char*) fmem_head) cur = nullptr;
    else {
        for (cur = fmem_head; cur->next != nullptr && (char*)addr > (char*)(cur->next); cur = cur->next);
    }

    Node* newFragment = (Node*) addr - 1;
    if (cur != nullptr) {
        newFragment->next = cur->next;
        cur->next = newFragment;
    }
    else {
        newFragment->next = fmem_head;
        fmem_head = newFragment;
    }
    tryToJoin(newFragment);
    tryToJoin(cur);

    return 0;
}