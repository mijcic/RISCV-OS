//
// Created by os on 8/18/24.
//
#include "../h/syscall_cpp.hpp"
#include "../h/ccb.hpp"

void* operator new(size_t size) {
    return mem_alloc(size);
}

void operator delete(void* addr) noexcept {
    mem_free(addr);
}

void* operator new[](size_t size) {
    return mem_alloc(size);
}

void operator delete[](void* addr) noexcept {
    mem_free(addr);
}

Thread::Thread (void (*body)(void*), void* arg) {
    asm volatile("mv a3, %0" : : "r" (arg));
    asm volatile("mv a2, %0" : : "r" (body));
    asm volatile("mv a1, %0" : : "r" (&myHandle));
    asm volatile("mv a0, %0" : : "r" (0x11));
    asm volatile("ecall");
}

Thread::~Thread() {
    delete myHandle;
}

int Thread::start() {
    asm volatile("mv a1, %0" : : "r" (myHandle));
    asm volatile("mv a0, %0" : : "r" (0x14));
    asm volatile("ecall");
    return 0;
}

void Thread::dispatch() {
    thread_dispatch();
}

Thread::Thread (){
    asm volatile("mv a3, %0" : : "r" (this));
    asm volatile("mv a2, %0" : : "r" (wrapper));
    asm volatile("mv a1, %0" : : "r" (&myHandle));
    asm volatile("mv a0, %0" : : "r" (0x11));
    asm volatile("ecall");
}

void Thread::wrapper(void *arg) {
    Thread* tmp=(Thread*)arg;
    tmp->run();
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle,init);
}
Semaphore::~Semaphore() {
    sem_close(myHandle);
    delete myHandle;
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::tryWait() {
    return sem_trywait(myHandle);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    return ::putc(c);
}