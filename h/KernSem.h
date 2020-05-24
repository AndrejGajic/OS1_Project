#ifndef _kernsem_h_
#define _kernsem_h_
#include "Semaphore.h"
#include "define.h"
#include "PCBList.h"
#include "TimeList.h"


class KernelSem {
private:
    int val;
    PCBList orderList;
    static TimeList timeList;
public:
    KernelSem(int init);
    ~KernelSem();
    int wait(Time maxTimeToWait);
    int signal(int n);
    int getVal() const {
        return val;
    }
    void incVal() {
        ++val;
    }
    
    static void timerDec();
    static TimeList allSems;
    static void releasePCB(PCB* p);
    void release(PCB* p);
    
};




#endif