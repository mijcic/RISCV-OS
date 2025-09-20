//
// Created by os on 8/15/24.
//

#ifndef NOVI_PROJEKAT_SEM_HPP
#define NOVI_PROJEKAT_SEM_HPP

#include"../lib/hw.h"

class CCB;
class Sem {
public:
    void* operator new(size_t size);
    void* operator new[] (size_t size);
    void  operator delete(void* addr);
    void  operator delete[](void* addr);

    CCB* blockedHead;
    CCB* blockedTail;

    Sem(int val) : val(val) {
        blockedHead = nullptr;
        blockedTail = nullptr;
    }

    static int create(Sem** handle, int init);
    static int close(Sem* handle);
    static int wait(Sem* id);
    static int signal(Sem* id);
    //static int timedwait(Sem* id, time_t timeout);
    static int trywait(Sem* id);

    int value() const { return val; }
    void setVal(int v) { val = v; }
private:
    int val;
};

#endif //NOVI_PROJEKAT_SEM_HPP