#include "timer.h"
#include "PCB.h"
#include "KernSem.h"

volatile word tss;
volatile word tsp;
volatile word tbp;
void tick();

void interrupt timerInterrupt(...) {
    /*if(finalLock == 1)  {
        oldTimer();
        tick();
        KernelSem::timerDec();
        if((!running->unlimited) && (running->timeLeft > 0)) --running->timeLeft;
        return;
    }*/
    if(!dispatched) {
        oldTimer();
        tick();
        KernelSem::timerDec();
        if((!running->unlimited) && (running->timeLeft > 0)) --running->timeLeft;
    }

    if(dispatched || ((!running->unlimited) && (running->timeLeft == 0) && (!lockVal))) { // PROMENA KONTEKSTA
        asm {
            mov tss, ss
            mov tsp, sp
            mov tbp, bp
        }
        running->ss = tss;
        running->sp = tsp;
        running->bp = tbp;

        running->myLock = lockVal;
        if((running != idlePCB) && (running->state == PCB::RUNNING)){
            running->state = PCB::READY;
            Scheduler::put(running);
        }
        running = Scheduler::get();
        if(!running) running = idlePCB;
        running->timeLeft = running->timeSlice;
        running->state = PCB::RUNNING;
        lockVal = running->myLock;

        tss = running->ss;
        tsp = running->sp;
        tbp = running->bp;
        asm {
            mov ss, tss
            mov sp, tsp
            mov bp, tbp
        }
        dispatched = false;
    }
    
}


