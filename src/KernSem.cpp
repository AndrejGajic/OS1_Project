#include "KernSem.h"
#include "PCB.h"
#include "thread.h"

TimeList KernelSem::timeList;
TimeList KernelSem::allSems;

KernelSem::KernelSem(int init) {
    val = init;
    allSems.Add(nullptr, this, -1);
}

KernelSem::~KernelSem() {
    orderList.startAll();
    timeList.startAll(this);
    allSems.DeleteSem(this);
}

int KernelSem::wait(Time maxTimeToWait) {
    if(maxTimeToWait < 0) return -1;
    LOCK
    int ret = 1;
    if(--val < 0) { // ako je val >= 0 running ne treba da se blokira na ovom semaforu
        running->block();
        if(maxTimeToWait == 0) {
            orderList.Add(running);
        }
        else if(maxTimeToWait > 0) {
            timeList.Add(running, this, maxTimeToWait);
        }
        dispatch();
        
        if(running->semTimeUnblocked == true) {
            running->semTimeUnblocked = false;
            ret = 0;
        }
        
    }
    UNLOCK
    return ret;
}



int KernelSem::signal(int n) {
    if (n < 0) return n;
    LOCK
    int ret = 0;
    bool flag = false;
    if(n == 0) {
        ++val;
        n = 1;
        flag = true;
    } else val+=n;
    while (ret < n) {
        if(orderList.isEmpty()) break;
        PCB* p = orderList.Delete();
        if(p == nullptr) break;
        p->unblock();
        ret++;
    }

    while (ret < n) {
        if(timeList.isEmpty()) break;
        PCB* p = timeList.DeleteSem(this);
        if(p == nullptr) break;
        p->unblock();
        ret++;
    }
    if (flag) ret=0;
    UNLOCK
    return ret;
}



void KernelSem::timerDec() {
    LOCK
    timeList.timerDec();
	while(timeList.readyToTake()) {
		PCB* p = timeList.Delete();
        if(p == nullptr) break;
        p->semTimeUnblocked = true;
		p->unblock();
	}
    UNLOCK
}

void KernelSem::releasePCB(PCB* p) {
    allSems.releasePCB(p);
}

void KernelSem::release(PCB* p) {
    orderList.ManualDelete(p);
    timeList.ManualDelete(p);
}