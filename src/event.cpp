#include "event.h"
#include "kernelev.h"
#include "pcb.h"

Event::Event(IVTNo ivtno) {
    LOCK
    myImpl = new KernelEvent(ivtno);
    UNLOCK
}

Event::~Event() {
    LOCK
    delete myImpl;
    myImpl = nullptr;
    UNLOCK
}

void Event::wait() {
    LOCK
    myImpl->wait();
    UNLOCK
}

void Event::signal() {
    LOCK
    myImpl->signal();
    UNLOCK
}

