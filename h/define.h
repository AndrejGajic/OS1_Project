#ifndef _define_h_
#define _define_h_

#include <iostream.h>
#include <dos.h>
#include "SCHEDULE.H"


// LOCKOVI -> koriste se za zakljucavanje kriticnih sekcija
// onemogucavanje prekida
#define HARD_LOCK asm {pushf; cli; }
#define HARD_UNLOCK asm {popf;}
// soft lockovi
void dispatch();
extern volatile int lockVal;
#define LOCK {++lockVal;}
#define UNLOCK if((--lockVal == 0) && (running->timeLeft == 0) && (!running->unlimited))  {dispatch();}

// final lock
extern volatile int finalLock;
#define FINAL_LOCK {finalLock = 1;}
#define FINAL_UNLOCK {finalLock = 0;}
// novi tipovi
typedef unsigned int word;
typedef unsigned int Time; //dato (time, x 55ms)
typedef unsigned long StackSize; //dato
typedef int ID; //dato
typedef void interrupt (*pInterrupt)(...);
typedef int bool;
typedef unsigned char IVTNo; // dato

// konstante
extern const StackSize maxStackSize;
extern const StackSize minStackSize;
extern const StackSize defaultStackSize; //dato
extern const Time defaultTimeSlice; //dato (default = 2*55ms)
extern const int PSW_VAL;
extern const IVTNo IVT_SIZE; 

// pomocni tipovi
#define nullptr 0
#define true 1
#define false 0

// globalne promenljive
class PCB;
extern PCB* running;
extern PCB* mainPCB;
extern PCB* idlePCB;
extern volatile pInterrupt oldTimer;

extern volatile bool dispatched; // da li je zatrazen dispatch

#endif
