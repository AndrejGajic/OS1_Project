#ifndef _ivt_h_
#define _ivt_h_
#include "define.h"


extern volatile pInterrupt oldTimer;

class IVT {
public:

    static void setTimer();
    static void resetTimer();

};



#endif