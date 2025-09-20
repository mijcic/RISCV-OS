//
// Created by marko on 20.4.22..
//

#include "../h/ccb.hpp"

CCB* Scheduler::head=nullptr;
CCB* Scheduler::tail=nullptr;

CCB* Scheduler::get()
{
    CCB* tmp = head;
    if (head == tail)
        head = tail = nullptr;
    else
        head = head->next;

    tmp->next = nullptr;
    tmp->state = CCB::RUNNING;
    return tmp;
}

void Scheduler::put(CCB *ccb)
{
    ccb->state = CCB::READY;
    ccb->next = nullptr;
    if(tail == nullptr)
        head = tail = ccb;
    else {
        tail->next = ccb;
        tail = ccb;
    }
}