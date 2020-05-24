#include "Semaphor.h"
#include "PCB.h"
#include "KernSem.h"

Semaphore::Semaphore(int init) {
    LOCK
    if(init < 0) init = 0;
    myImpl = new KernelSem(init);
    UNLOCK
}

Semaphore::~Semaphore() {
    LOCK
    if(myImpl != nullptr) {
        delete myImpl;
        myImpl = nullptr;
    }
    UNLOCK
}

int Semaphore::wait(Time maxTimeToWait) {
    int ret = -1;
    LOCK
    if(myImpl != nullptr) ret = myImpl->wait(maxTimeToWait);
    UNLOCK
    return ret;
}

int Semaphore::signal(int n) {
    int ret = -1;
    LOCK
    if(myImpl != nullptr) ret = myImpl->signal(n);
    UNLOCK
    return ret;
}

int Semaphore::val() const {
    int ret = -1;
    LOCK
    if(myImpl != nullptr) ret = myImpl->getVal();
    UNLOCK
    return ret;
}