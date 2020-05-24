#include "PCB.h"
#include "timer.h"
#include "KernSem.h"


ID PCB::staticID = 0;
PCBList PCB::allPCBs;

PCB::PCB(StackSize stackSize1, Time timeSlice1, Thread* thread1) {
    if(stackSize1 > maxStackSize) stackSize1 = maxStackSize;
    if(stackSize1 < minStackSize) stackSize1 = minStackSize;
    stackSize = stackSize1 / sizeof(word);
    if(timeSlice1 == 0) {
        unlimited = true;
        timeSlice = timeLeft = -1;
    } else {
        unlimited = false;
        timeSlice = timeLeft = timeSlice1;
    }
    myThread = thread1;
    myLock = 0;
    state = NEW;
    myID = ++staticID;
    semTimeUnblocked = false;
    if(myThread != nullptr) this->initStack(PCB::threadWrapper);
    allPCBs.Add(this);
}

PCB::~PCB() {
    this->release();
    this->state = TERMINATED;
    LOCK
    if(stack != nullptr) {
        delete[] stack;
        stack = nullptr;
    }
    UNLOCK
}

void PCB::initStack(void(*wrapper)()) {
    LOCK
    stack = new word[stackSize];
    stack[stackSize - 1] = 0x200; // PSW pocetna vrednost, I = 1
    stack[stackSize - 2] = FP_SEG(wrapper);
    stack[stackSize - 3] = FP_OFF(wrapper);
    // ostali registri
    ss = FP_SEG(stack + stackSize - 12);
    sp = FP_OFF(stack + stackSize - 12);
    bp = sp;
    UNLOCK
}

void PCB::threadWrapper() {
    running->myThread->run();
    LOCK
    running->release();
    running->state = TERMINATED;
    dispatch();
    UNLOCK
}

void PCB::idleWrapper() {
    while(1) {
        //dispatch();
    }
}

void PCB::start() {
    LOCK
    state = READY;
    Scheduler::put(this);
    UNLOCK
}

void PCB::waitToComplete() {
    LOCK
    if((running != this) && (this != idlePCB) && (this->state != TERMINATED) && (this->state != NEW) && (!this->isDeadlocked())) {
        running->state = BLOCKED;
        waitingList.Add(running);
        dispatch();
    }
    UNLOCK
}

ID PCB::getId() {
    return myID;
}

ID PCB::getRunningId() {
    return running->myID;
}

Thread* PCB::getThreadById(ID id) {
    PCB* temp = allPCBs.getPCBById(id);
    if(temp != nullptr) return temp->myThread;
    else return nullptr;
}


void PCB::block() {
    LOCK
    this->state = BLOCKED;
    UNLOCK
}

void PCB::unblock() {
    LOCK
    this->state = READY;
    Scheduler::put(this);
    UNLOCK
}

bool PCB::isDeadlocked() {
    LOCK
    bool x = running->waitingList.isDeadlocked(this);
    UNLOCK
    return x;
}


void PCB::release() {
    LOCK
    waitingList.startAll();
    allPCBs.releasePCB(this);
    KernelSem::releasePCB(this);
    UNLOCK
}