#include "define.h"
#include "thread.h"
#include "pcb.h"
#include "timer.h"
#include "ivt.h"
#include "system.h"



int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {

    cout << "Main poceo!" << endl;
    System::start();
    int retValue = userMain(argc, argv);
    System::end();
    cout << "Main gotov!" << endl;

    
    return retValue;
}