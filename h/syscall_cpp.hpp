//
// Created by os on 8/18/24.
//

#ifndef NOVI_PROJEKAT_SYSCALL_CPP_HPP
#define NOVI_PROJEKAT_SYSCALL_CPP_HPP

#include "../h/syscall_c.hpp"
#include "../h/sem.hpp"

void* operator new(size_t size);
void* operator new[] (size_t size);
void  operator delete(void* addr) noexcept;
void  operator delete[](void* addr) noexcept;

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    static void dispatch ();
protected:
    Thread ();
    virtual void run () {}
private:
    thread_t myHandle;
    void (*body)(void*); void* arg;
    static void wrapper(void* arg);
};

class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
    int tryWait();
private:
    sem_t myHandle;
};

class Console {
public:
    static char getc ();
    static void putc (char);
};

#endif //NOVI_PROJEKAT_SYSCALL_CPP_HPP
