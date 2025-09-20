//
// Created by marko on 20.4.22..
//
#include "../test/printing.hpp"
#include "../lib/console.h"
#include "../h/memory_allocator.hpp"
#include "../h/ccb.hpp"
#include "../h/sem.hpp"

void Riscv::popSppSpieSupervisor()
{
    __asm__ volatile ("csrw sepc, ra");
    mc_sip(SIP_SSIE);
    ms_sstatus(SSTATUS_SPP);
    __asm__ volatile ("sret");
}

void Riscv::popSppSpieUser()
{
    __asm__ volatile ("csrw sepc, ra");
    mc_sip(SIP_SSIE);
    mc_sstatus(SSTATUS_SPP);
    __asm__ volatile ("sret");
}

//void Riscv::w_sie(uint64 val) {
//    __asm__ volatile("csrw sie, %0" : : "r"(val));
//}

void Riscv::handler() {
    uint64 scause = r_scause();
    uint64 return_code;
    uint64 arg1, arg2, arg3, arg4, arg5, arg6, arg7;
    uint64 volatile pc;
    uint64 volatile status;
    __asm__ volatile("mv %0, a0" : "=r" (return_code));
    __asm__ volatile("mv %0, a1" : "=r" (arg1));
    __asm__ volatile("mv %0, a2" : "=r" (arg2));
    __asm__ volatile("mv %0, a3" : "=r" (arg3));
    __asm__ volatile("mv %0, a4" : "=r" (arg4));
    __asm__ volatile("mv %0, a5" : "=r" (arg5));
    __asm__ volatile("mv %0, a6" : "=r" (arg6));
    __asm__ volatile("mv %0, a7" : "=r" (arg7));


    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        pc = r_sepc() + 4;
        status = r_sstatus();

        if (return_code == 0x01) {
            void* ret = MemoryAllocator::mem_alloc((size_t)arg1);
            asm volatile("mv a0, %0" : : "r" (ret));
        }
        else if (return_code == 0x02) {
            int ret = MemoryAllocator::mem_free((void*)arg1);
            asm volatile("mv a0, %0" : : "r" (ret));
        }
        else if (return_code == 0x11) {
            CCB** handle = (CCB**)arg1;
            void (*body)(void*) = (void(*)(void*))(arg2);
            void* arg = (void*)arg3;
            void* stack_space = MemoryAllocator::mem_alloc(DEFAULT_STACK_SIZE);
            int ret = CCB::createCCB(handle, body, arg, stack_space);
            asm volatile("mv a0, %0" : : "r" (ret));
        }
        else if (return_code == 0x12) {
            int ret = CCB::exit();
//            CCB::dispatch();
            asm volatile("mv a0, %0" : : "r" (ret));
        }
        else if (return_code == 0x13) {
            CCB::dispatch();
        }
        else if (return_code ==0x14) {
            CCB* handle = (CCB*)arg1;
            if (handle!=nullptr) {
                if (handle->getBody() != nullptr) {
                    Scheduler::put(handle);
                }
            }
        }
        else if (return_code == 0x21) {
            Sem** handle = (Sem**)arg1;
            int init = (int)arg2;
            int ret = Sem::create(handle, init);
            asm volatile("mv a0, %0" : : "r" (ret));
        }
        else if (return_code == 0x22) {
            Sem* handle = (Sem*)arg1;
            int ret = Sem::close(handle);
            asm volatile("mv a0, %0" : : "r" (ret));
        }
        else if (return_code == 0x23) {
            Sem* id = (Sem*)arg1;
            int ret = Sem::wait(id);
            asm volatile("mv a0, %0" : : "r" (ret));
        }
        else if (return_code == 0x24) {
            Sem* id = (Sem*)arg1;
            int ret = Sem::signal(id);
            asm volatile("mv a0, %0" : : "r" (ret));
        }
        else if (return_code == 0x26) {
            Sem* id = (Sem*)arg1;
            int ret = Sem::trywait(id);
            asm volatile("mv a0, %0" : : "r" (ret));
        }
        else if (return_code == 0x41) {
            char c = __getc();
            asm volatile("mv a0,%0" : : "r" (c));
        }
        else if (return_code == 0x42) {
            char c;
            asm volatile("mv %0,a1" : "=r" (c));
            __putc(c);
        }
        else if (return_code==0x51){
            CCB::supUserFlag=false;
            status &= ~0x100;
        }
        else if (return_code==0x52){
            CCB::supUserFlag=true;
            status |= 0x100;
        }

        __asm__ volatile("sd a0, 10*8(s0)");
        w_sepc(pc);
        w_sstatus(status);
    }
    else if (scause == 0x8000000000000001UL) { //prekid od tajmera
        status=r_sstatus();
        pc=r_sepc();
        //CCB::dispatch();
        mc_sip(SIP_SSIE);
        w_sstatus(status);
        w_sepc(pc);
    }
    else if (scause == 0x8000000000000009UL) { //prekid od konzole
        console_handler();
    }
//    else
//    {
//        if (scause == 0x0000000000000002UL)
//        {
//            printString("Ilegalna instrukcija\n");
//        }
//        if (scause == 0x0000000000000005UL)
//        {
//            printString("Nedozvoljena adresa citanja\n");
//        }
//        if (scause == 0x0000000000000007UL)
//        {
//            printString("Nedozvoljena adresa upisa\n");
//        }
//    }
}