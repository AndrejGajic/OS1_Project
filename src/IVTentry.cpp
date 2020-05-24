#include "ivtentry.h"
#include "kernelev.h"
#include "pcb.h"


IVTEntry* IVTEntry::IVT[256] = {0}; // inicijalizujemo praznu


IVTEntry::IVTEntry(IVTNo ivtno, pInterrupt intEvent) {
    LOCK
    if(ivtno < 0) ivtno = 0;
    if(ivtno > 255) ivtno = 255;
    ivtNo = ivtno;
    eventInterrupt = intEvent;
    myEvent = nullptr;
    IVTEntry::IVT[ivtNo] = this;
    UNLOCK
}

IVTEntry::~IVTEntry() {
    restoreInterrupt();
}

void IVTEntry::setEvent(KernelEvent* ev) {
    HARD_LOCK
    myEvent = ev;
    oldInterrupt = getvect(ivtNo);
    setvect(ivtNo, eventInterrupt);
    HARD_UNLOCK
}

void IVTEntry::restoreInterrupt() {
    HARD_LOCK
    myEvent = nullptr;
    setvect(ivtNo, oldInterrupt);
    IVTEntry::IVT[ivtNo] = nullptr;
    HARD_UNLOCK
}

void IVTEntry::callOldInterrupt() {
    this->oldInterrupt();
}

void IVTEntry::signal() {
    if(myEvent != nullptr) {
        myEvent->signal();
    }
}