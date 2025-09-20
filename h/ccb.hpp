//
// Created by marko on 20.4.22..
//

#ifndef NOVI_PROJEKAT_CCB_HPP
#define NOVI_PROJEKAT_CCB_HPP

#include "scheduler.hpp"
#include "riscv.hpp"

// Coroutine Control Block
class CCB
{
public:
    CCB* next = nullptr;

    static bool supUserFlag; //true -> sis rezim, false -> korisnicki rezim

    void* operator new(size_t size);
    void* operator new[] (size_t size);
    void  operator delete(void* addr);
    void  operator delete[](void* addr);

    CCB() = default;

    ~CCB() { delete[] stack; }

    using Body = void (*)(void*);

    Body getBody() { return body; }

    static int createCCB(CCB** handle, Body body, void* arg, void* stack_space);

    static int exit();

    static void yield();

    static void dispatch();

    static void threadWrapper();

    static CCB *running;

    static uint64 constexpr STACK_SIZE = 1024;

private:

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body=nullptr;
    void* arg=nullptr;
    uint64 *stack=nullptr;
    Context context;

public:

    enum State {READY, RUNNING, BLOCKED, FINISHED};
    State state;

private:

    explicit CCB(Body body, void *arg, uint64* stack) :
            next(nullptr),
            body(body),
            arg(arg),
            stack(body != nullptr ? stack : nullptr),
            context({body != nullptr ? (uint64) &threadWrapper : 0,
                     stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    }),
            state(READY)
    {

    }

    static void contextSwitch(Context *oldContext, Context *runningContext);
};

#endif //NOVI_PROJEKAT_CCB_HPP