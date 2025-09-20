//
// Created by os on 8/15/24.
//

#include "../h/sem.hpp"
#include "../h/memory_allocator.hpp"
#include "../h/ccb.hpp"

void* Sem::operator new(size_t size) {
    void* tmp = MemoryAllocator::mem_alloc(size);
    return tmp;
}

void* Sem::operator new[](size_t size) {
    void* tmp = MemoryAllocator::mem_alloc(size);
    return tmp;
}

void Sem::operator delete(void* addr) {
    MemoryAllocator::mem_free(addr);
}

void Sem::operator delete[](void* addr) {
    MemoryAllocator::mem_free(addr);
}

int Sem::create(Sem** handle, int init) {
    Sem* myHandle = new Sem(init);
    *handle = myHandle;
    return 0;
}

int Sem::close(Sem* handle) {
    if(handle==nullptr) return -1;
    CCB* cur = handle->blockedHead;
    while (cur) {
        Scheduler::put(cur);
        cur = cur->next;
    }
    handle->blockedHead = nullptr;
    handle->blockedTail = nullptr;
    return 0;
}

int Sem::wait(Sem* id) {
    if (--(id->val) < 0) {
        CCB::running->state = CCB::BLOCKED;
        CCB::running->next = nullptr;
        if (id->blockedTail == nullptr) id->blockedHead = id->blockedTail = CCB::running;
        else {
            id->blockedTail->next = CCB::running;
            id->blockedTail = CCB::running;
        }
        CCB::dispatch();
    }
    return 0;
}

int Sem::signal(Sem* id) {
    if (++(id->val) <= 0) {
        CCB* tmp = id->blockedHead;
        if (tmp == nullptr) return 0;

        if (id->blockedHead == id->blockedTail) id->blockedHead = id->blockedTail = nullptr;
        else id->blockedHead = id->blockedHead->next;

        Scheduler::put(tmp);
    }
    return 0;
}

int Sem::trywait(Sem* id) {
    if (id == nullptr) return -1;
    if (id->val < 0) return 0;
    else return 1;
}