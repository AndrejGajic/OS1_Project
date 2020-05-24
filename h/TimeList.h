#ifndef _timelist_h_
#define _timelist_h_
#include "define.h"

class PCB;
class KernelSem;

class TimeList {
private:
	struct TimeNode {
		PCB* pcb;
		KernelSem* sem;
		Time timeLeft;
		TimeNode* next;
		TimeNode(PCB* p, KernelSem* s, Time time) {
			pcb = p;
			sem = s;
			timeLeft = time;
			next = nullptr;
		}
	};
	TimeNode* head;
	TimeNode* tail;
public:
	TimeList() {
		head = tail = nullptr;
	}
	~TimeList();
	void Add(PCB* p, KernelSem* s, Time time);
	PCB* Delete();
	bool ManualDelete(PCB* p);
	bool isEmpty() const {
		if (head == nullptr) return true;
		else return false;
	}
	bool readyToTake();
	void timerDec() {
		if(head == nullptr) return;
		if(head->timeLeft > 0) head->timeLeft--;
	}
	PCB* DeleteOnly();
	PCB* DeleteSem(KernelSem* s);
	void startAll(KernelSem* s);
	void releasePCB(PCB* p);

};









#endif