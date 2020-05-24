#ifndef _ivtentry_h_
#define _ivtentry_h_
#include "define.h"


class KernelEvent;


class IVTEntry {
public:
    IVTEntry(IVTNo ivtno, pInterrupt intEvent);
    ~IVTEntry();
    void setEvent(KernelEvent* ev);
    void restoreInterrupt();
    void callOldInterrupt();
    void signal();
    pInterrupt oldInterrupt;
    pInterrupt eventInterrupt;
    static IVTEntry* IVT[256];
private:
    IVTNo ivtNo;
    KernelEvent* myEvent;

};

#define PREPAREENTRY(ivtNum, callOld)\
extern IVTEntry ivtEntry##ivtNum;\
void interrupt interruptEvent##ivtNum(...) {\
    ivtEntry##ivtNum.signal();\
    if (callOld) ivtEntry##ivtNum.callOldInterrupt();\
    dispatch();\
}\
IVTEntry ivtEntry##ivtNum(ivtNum, interruptEvent##ivtNum);







#endif