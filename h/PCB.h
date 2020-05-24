#ifndef _pcb_h_
#define _pcb_h_
#include "define.h"
#include "thread.h"
#include "PCBList.h"


class PCB {
public: // atributi
    // stek
    word* stack;
    word ss, sp, bp; // stack segment, stack offset, base pointer
    StackSize stackSize;
    // stanje niti
    enum threadState {NEW, READY, RUNNING, BLOCKED, TERMINATED};
    threadState state;
    // ID
    static ID staticID;
    ID myID;
    // nit omotac
    Thread* myThread;
    // lock
    int myLock;
    // Time
    Time timeSlice;
    Time timeLeft;
    bool unlimited;
    // liste
    static PCBList allPCBs;
    PCBList waitingList;
    // sem
    bool semTimeUnblocked;
    
    // metodi
    PCB(StackSize stackSize1, Time timeSlice1, Thread* thread1);
    ~PCB();
    void initStack(void(*wrapper)());
    static void threadWrapper();
    static void idleWrapper();

    void start();
    void waitToComplete();
    ID getId();
    static ID getRunningId();
    static Thread* getThreadById(ID id);

    threadState getState() const {
        return this->state;
    }
    void setState(threadState s) {
        this->state = s;
    }
    // pomocne fje za blokiranje i odblokiranje niti, koriste se u listama i semaforima
    void block();
    void unblock();
    // proverava da li dolazi do deadlock-a
    bool isDeadlocked();
    // oslobadja PCB iz svih listi i vrsi dealokaciju memorije
    void release();
    


};



#endif