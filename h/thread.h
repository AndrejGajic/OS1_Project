#ifndef _thread_h_
#define _thread_h_
#include "define.h"

class PCB;
class Thread {
public:

    void start();
    void waitToComplete();
    virtual ~Thread();

    ID getId();
    static ID getRunningId();
    static Thread* getThreadById(ID id);

protected:
    friend class PCB;
    Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
    virtual void run() {}

private:
    PCB* myPCB;

};
void dispatch();


#endif