//
// Created by os on 7/20/24.
//
#include "../h/syscall_c.hpp"

void* mem_alloc(size_t size) {
    void* ret;
    asm volatile("mv a1, %0" : : "r" (size));
    asm volatile("mv a0, %0" : : "r" (0x01));
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int mem_free(void* addr) {
    int ret;
    asm volatile("mv a1, %0" : : "r" (addr));
    asm volatile("mv a0, %0" : : "r" (0x02));
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg) {
    int ret;
    asm volatile("mv a3, %0" : : "r" (arg));
    asm volatile("mv a2, %0" : : "r" (start_routine));
    asm volatile("mv a1, %0" : : "r" (handle));
    asm volatile("mv a0, %0" : : "r" (0x11));
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r" (ret));
    if (ret != -1) {
        asm volatile("mv a1, %0" : : "r" (*handle));
        asm volatile("mv a0, %0" : : "r" (0x14));
        asm volatile("ecall");
    }
    return ret;
}

int thread_exit() {
    int ret;
    asm volatile("mv a0, %0" : : "r" (0x12));
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

void thread_dispatch() {
    asm volatile("mv a0, %0" : : "r" (0x13));
    asm volatile("ecall");
}

int sem_open(sem_t* handle, unsigned init) {
    int ret;
    asm volatile("mv a2, %0" : : "r" (init));
    asm volatile("mv a1, %0" : : "r" (handle));
    asm volatile("mv a0, %0" : : "r" (0x21));
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_close(sem_t handle) {
    int ret;
    asm volatile("mv a1, %0" : : "r" (handle));
    asm volatile("mv a0, %0" : : "r" (0x22));
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_wait(sem_t id) {
    int ret;
    asm volatile("mv a1, %0" : : "r" (id));
    asm volatile("mv a0, %0" : : "r" (0x23));
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_signal(sem_t id) {
    int ret;
    asm volatile("mv a1, %0" : : "r" (id));
    asm volatile("mv a0, %0" : : "r" (0x24));
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_trywait(sem_t id) {
    int ret;
    asm volatile("mv a1, %0" : : "r" (id));
    asm volatile("mv a0, %0" : : "r" (0x26));
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

char getc() {
    char ret;
    asm volatile("mv a0, %0" : : "r" (0x41));
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

void putc(char c) {
    asm volatile("mv a1, %0" : : "r" (c));
    asm volatile("mv a0, %0" : : "r" (0x42));
    asm volatile("ecall");
}

void modeUser(){
    asm volatile("mv a0, %0" : : "r" (0x51));
    asm volatile("ecall");
}

void modeSupervisor(){
    asm volatile("mv a0, %0" : : "r" (0x52));
    asm volatile("ecall");
}