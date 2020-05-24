#include "kernelev.h"
#include "IVTentry.h"
#include "PCB.h"


KernelEvent::KernelEvent(IVTNo ivtno1) : eventPCB(running), ivtNo(ivtno1), binSem(0) {
    LOCK
    IVTEntry::IVT[ivtNo]->setEvent(this);
    UNLOCK
}

KernelEvent::~KernelEvent() {
    LOCK
    eventPCB = nullptr;
    IVTEntry::IVT[ivtNo]->restoreInterrupt();
    UNLOCK
}

void KernelEvent::wait() { // mozda ovde?
    LOCK
    if(eventPCB == running) {
        binSem.wait(0);
    }
    UNLOCK
    
}

void KernelEvent::signal() {
   LOCK
   if(binSem.val() < 0) {
       binSem.signal(0);
   }
   UNLOCK
}