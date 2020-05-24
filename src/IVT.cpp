#include "ivt.h"
#include "timer.h"

volatile pInterrupt oldTimer = nullptr; 

void IVT::setTimer() {
    HARD_LOCK
    oldTimer = getvect(0x8);
    setvect(0x8, timerInterrupt);
    HARD_UNLOCK
}

void IVT::resetTimer() {
    HARD_LOCK
    setvect(0x8, oldTimer);
    HARD_UNLOCK
}