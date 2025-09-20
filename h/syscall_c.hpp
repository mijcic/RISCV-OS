//
// Created by os on 7/20/24.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_C_HPP
#define PROJECT_BASE_V1_1_SYSCALL_C_HPP

#include "../lib/hw.h"

void* mem_alloc(size_t size);
int mem_free(void*);

class CCB;
typedef CCB* thread_t;
int thread_create(CCB** handle, void(*start_routine)(void*), void* arg);
int thread_exit();
void thread_dispatch();

void putc(char chr);

char getc();
class Sem;
typedef Sem* sem_t;
int sem_open(sem_t* handle, unsigned init);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);
//int sem_timedwait(sem_t id, time_t timeout);
int sem_trywait(sem_t id);

const int EOF = -1;
char getc();
void putc(char);

void modeUser();
void modeSupervisor();
#endif //PROJECT_BASE_V1_1_SYSCALL_C_HPP
