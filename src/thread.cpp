#include "thread.h"
#include "PCB.h"
#include "timer.h"

// sve fje lockovati ako ne bude radilo!!!


void Thread::start() {
    LOCK
    if(myPCB != nullptr) myPCB->start();
    UNLOCK
}

void Thread::waitToComplete() {
    LOCK
    if(myPCB != nullptr) myPCB->waitToComplete();
    UNLOCK
}

Thread::Thread(StackSize stackSize, Time timeSlice) {
    LOCK
    myPCB = new PCB(stackSize, timeSlice, this);
    UNLOCK
}

Thread::~Thread() {
    this->waitToComplete();
    LOCK
    if(myPCB != nullptr) {
        myPCB->release();
        delete myPCB;
        myPCB = nullptr;
    }
    UNLOCK
}

ID Thread::getId() {
    if(myPCB != nullptr) return myPCB->getId();
    else return -1;
}

ID Thread::getRunningId() {
    if(running != nullptr) return PCB::getRunningId();
    else return -1;
}

Thread* Thread::getThreadById(ID id) {
    return PCB::getThreadById(id);
}

void dispatch() {
    HARD_LOCK
    dispatched = true;
    timerInterrupt();
    HARD_UNLOCK
}