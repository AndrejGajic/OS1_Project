#ifndef _timer_h_
#define _timer_h_
#include "define.h"

extern volatile word tss;
extern volatile word tsp;
extern volatile word tbp;

void interrupt timerInterrupt(...);

#endif