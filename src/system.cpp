#include "system.h"
#include "PCB.h"
#include "IVT.h"

void System::start() {
    mainPCB = new PCB(0, defaultTimeSlice, nullptr);
    mainPCB->state = PCB::RUNNING;
    running = mainPCB;
    idlePCB = new PCB(defaultStackSize, 1, nullptr);
    idlePCB->initStack(PCB::idleWrapper);

    IVT::setTimer();
}


void System::end() {
    IVT::resetTimer();
    /*if(mainPCB != nullptr) delete mainPCB;
    if(idlePCB != nullptr) delete idlePCB;
    if(running != nullptr) delete running; */
}