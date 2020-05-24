#ifndef _pcblist_h_
#define _pcblist_h_
#include "define.h"

class PCB;


class PCBList {
private:
    struct PCBNode {
        PCB* data;
        PCBNode* next;
        PCBNode(PCB* p) {
            data = p;
            next = nullptr;
        }
    };
    PCBNode* head;
    PCBNode* tail;
public:
    PCBList() {
        head = tail = nullptr;
    }
    ~PCBList();
    bool isEmpty() const {
        if(head == nullptr) return true;
        else return false;
    }
    void Add(PCB* p);
    PCB* Delete();
    bool ManualDelete(PCB* p);
    void startAll();
    PCB* getPCBById(ID id);
    bool isDeadlocked(PCB* p);
    void releasePCB(PCB* p);

};







#endif