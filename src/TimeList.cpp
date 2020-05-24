#include "TimeList.h"
#include "PCB.h"
#include "KernSem.h"


TimeList::~TimeList() {
	LOCK
	while (head != nullptr) {
		DeleteOnly();
	}
	head = tail = nullptr;
	UNLOCK
}

void TimeList::Add(PCB* p, KernelSem* s, Time time) {
	LOCK
	HARD_LOCK
	TimeNode* node = new TimeNode(p, s, time);
	HARD_UNLOCK
	if (head == nullptr) {
		head = tail = node;
	}
	else if(node->timeLeft <= head->timeLeft){
		node->next = head;
		head->timeLeft -= node->timeLeft;
		head = node;
	}
	else {
		TimeNode* prev = nullptr;
		TimeNode* temp = head;
		int tempTime = head->timeLeft;
		while (tempTime < node->timeLeft) {
			prev = temp;
			temp = temp->next;
			if (temp == nullptr) break;
			tempTime += temp->timeLeft;
		}
		if (temp == nullptr) {
			tail->next = node;
			node->timeLeft -= tempTime;
			tail = node;
		}
		else {
			tempTime -= temp->timeLeft;
			prev->next = node;
			node->next = temp;
			node->timeLeft -= tempTime;
			temp->timeLeft -= node->timeLeft;
		}
	}
	UNLOCK
}


PCB* TimeList::Delete() {
	if (head == nullptr) return nullptr;
	LOCK
	TimeNode* temp = head;
	head = head->next;
	if (head == nullptr) tail = nullptr;
	else head->timeLeft += temp->timeLeft;
	temp->sem->incVal();
	PCB* p = temp->pcb;
	HARD_LOCK
	delete temp;
	HARD_UNLOCK
	UNLOCK
	return p;
}

PCB* TimeList::DeleteOnly() {
	if(head == nullptr) return nullptr;
	LOCK
	TimeNode* temp = head;
	head = head->next;
	if(head == nullptr) tail = nullptr;
	else head->timeLeft += temp->timeLeft;
	PCB*p = temp->pcb;
	HARD_LOCK
	delete temp;
	HARD_UNLOCK
	UNLOCK
	return p;
}

PCB* TimeList::DeleteSem(KernelSem* s) {
	if(head == nullptr) return nullptr;
	LOCK
	PCB* p = nullptr;
	TimeNode* temp = head;
	if(head->sem == s) {
		head = head->next;
		if(head == nullptr) tail = nullptr;
		else head->timeLeft += temp->timeLeft;
		p = temp->pcb;
		HARD_LOCK
		delete temp;
		HARD_UNLOCK
	} else {
		TimeNode* prev = nullptr;
		while(temp->sem != s) {
			prev = temp;
			temp = temp->next;
			if(temp == nullptr) break;
		}
		if(temp != nullptr) {
			p = temp->pcb;
			if(temp == tail) {
				tail = prev;
				tail->next = nullptr;
				HARD_LOCK
				delete temp;
				HARD_UNLOCK
			}
			else {
				prev->next = temp->next;
				temp->next->timeLeft+=temp->timeLeft;
				HARD_LOCK
				delete temp;
				HARD_UNLOCK
			}
		}
	}
	UNLOCK
	return p;
}


bool TimeList::ManualDelete(PCB* p) {
	if (head == nullptr) return false;
	LOCK
	bool ret = true;
	if (head->pcb == p) {
		if (head == tail) {
			HARD_LOCK
			delete head;
			HARD_UNLOCK
			head = tail = nullptr;
		}
		else {
			TimeNode* temp = head;
			head = head->next;
			head->timeLeft += temp->timeLeft;
			temp->next = nullptr;
			HARD_LOCK
			delete temp;
			HARD_UNLOCK
		}
	}
	else {
		TimeNode* temp = head;
		TimeNode* prev = nullptr;
		while (temp->pcb != p) {
			prev = temp;
			temp = temp->next;
			if (temp == nullptr) break;
		}
		if (temp != nullptr) {
			if (temp == tail) {
				tail = prev;
				tail->next = nullptr;
			}
			else {
				prev->next = temp->next;
				temp->next->timeLeft += temp->timeLeft;
			}
			temp->next = nullptr;
			HARD_LOCK
			delete temp;
			HARD_UNLOCK
		}
		else ret = false;
	}
    UNLOCK
	return ret;
}


bool TimeList::readyToTake() {
	LOCK
	bool flag;
	if(head == nullptr) flag = false;
	else if (head->timeLeft <= 0) flag = true;
	else flag = false;
	UNLOCK
	return flag;
}


void TimeList::startAll(KernelSem* s) {
	if(head == nullptr) return;
	LOCK
	TimeNode* temp = head;
	TimeNode* ret = nullptr;
	PCB* p = nullptr;
	while(temp != nullptr) {
		if(temp->sem == s) {
			p = temp->pcb;
			p->unblock();
			if(temp == head) {
				head = head->next;
				if(head == nullptr) {
					tail = ret = nullptr;
				} else head->timeLeft+=temp->timeLeft;
				HARD_LOCK
				delete temp;
				HARD_UNLOCK
				temp = head;
			} else {
				ret->next = temp->next;
				if(temp == tail) {
					tail->next = nullptr;
					tail = ret;
				} else temp->next->timeLeft+=temp->timeLeft;
				HARD_LOCK
				delete temp;
				HARD_UNLOCK
				temp = ret->next;
			}
		}
		else {
			temp = temp->next;
		}
		ret = temp;
	}
	UNLOCK
}


void TimeList::releasePCB(PCB* p) {
	if(head == nullptr) return;
	TimeNode* temp = head;
	while(temp != nullptr) {
		temp->sem->release(p);
		temp = temp->next;
	}
};
