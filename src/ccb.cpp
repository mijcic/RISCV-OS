//
// Created by marko on 20.4.22..
//

#include "../h/ccb.hpp"
#include "../h/memory_allocator.hpp"

CCB *CCB::running = nullptr;
bool CCB::supUserFlag = true;

void* CCB::operator new(size_t size) {
    void* tmp = MemoryAllocator::mem_alloc(size);
    return tmp;
}

void* CCB::operator new[](size_t size) {
    void* tmp = MemoryAllocator::mem_alloc(size);
    return tmp;
}

void CCB::operator delete(void* addr) {
    MemoryAllocator::mem_free(addr);
}

void CCB::operator delete[](void* addr) {
    MemoryAllocator::mem_free(addr);
}

int CCB::createCCB(CCB** handle, Body body, void* arg, void* stack_space){
    if (stack_space == nullptr) return -1;
    *handle = new CCB(body, arg, (uint64*)stack_space);
    if (*handle == nullptr) return -1;

    return 0;
}

void CCB::threadWrapper() {
    if (supUserFlag) Riscv::popSppSpieSupervisor();
    else Riscv::popSppSpieUser();

    running->body(running->arg);
    running->state = FINISHED;

    asm volatile("mv a0, %0" : : "r" (0x13));
    asm volatile("ecall");
}

void CCB::yield()
{
    Riscv::pushRegisters();

    CCB::dispatch();

    Riscv::popRegisters();
}

int CCB::exit() {
    running->state=FINISHED;
    CCB::dispatch();
    return 0;
}

void CCB::dispatch() {
    CCB *old = running;
    if (old->state != FINISHED && old->state!=BLOCKED) { Scheduler::put(old); }
    running = Scheduler::get();
    running->state=RUNNING;
    CCB::contextSwitch(&old->context, &running->context);
}
