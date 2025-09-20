//
// Created by marko on 20.4.22..
//
#include "../h/riscv.hpp"
#include "../h/ccb.hpp"
#include "../h/print.hpp"
#include "../h/memory_allocator.hpp"
#include "../h/syscall_c.hpp"
extern void userMain();

int main()
{
    MemoryAllocator::initMemoryAllocator();
    Riscv::w_stvec((uint64)&Riscv::supervisor);
    CCB* main = new CCB();
    CCB::running=main;
    //CCB* thread;

    modeUser();
    userMain();

//    thread_create(&thread, (void(*)(void*))&userMain, nullptr);
//
//
//    while (thread->state != CCB::FINISHED && thread->state != CCB::BLOCKED) {
//        thread_dispatch();
//    }

    modeSupervisor();

    mem_free(main);
    //mem_free(thread);

    return 0;
}