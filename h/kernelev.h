#ifndef _kernelev_h_
#define _kernelev_h_
#include "event.h"
#include "Semaphore.h"


class PCB;


class KernelEvent {
public:
    KernelEvent(IVTNo ivtno);
    ~KernelEvent();
    void wait();
    void signal();
private:
    PCB* eventPCB; // PCB niti koja se blokira (odblokira) na ovaj dogadjaj
    IVTNo ivtNo; // redni broj ulaza u IV tabelu
    Semaphore binSem;


};




#endif