#ifndef _event_h_
#define _event_h_
#include "define.h"
#include "IVTEntry.h"

class KernelEvent;

class Event  {
public:
    Event(IVTNo ivtno);
    ~Event();

    void wait();

protected:
    friend class KernelEvent;
    void signal(); //can call KernelEv

private:
    KernelEvent* myImpl;

};




#endif