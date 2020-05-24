#include "define.h"

volatile int lockVal = 0;
volatile int finalLock = 0;
const StackSize maxStackSize = 65535;
const StackSize minStackSize = 256;
const StackSize defaultStackSize = 4096; //dato
const Time defaultTimeSlice = 2; //dato (default = 2*55ms)
const int PSW_VAL = 0x200;
const IVTNo IVT_SIZE = 256;
PCB* mainPCB = nullptr;
PCB* idlePCB = nullptr;
PCB* running = nullptr;
volatile bool dispatched = false;
