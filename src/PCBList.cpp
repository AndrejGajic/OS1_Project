#include "PCBList.h"
#include "PCB.h"
#include "SCHEDULE.h"

PCBList::~PCBList() {
	LOCK
	while (head != nullptr) {
		Delete();
	}
	head = tail = nullptr;
	UNLOCK
}

void PCBList::Add(PCB* p) {
    HARD_LOCK
    PCBNode* node = new PCBNode(p);
    HARD_UNLOCK
	if (head == nullptr) {
		head = tail = node;
	}
	else {
		tail->next = node;
		tail = node;
	}
}

PCB* PCBList::Delete() {
    if (head == nullptr) return nullptr;
    PCBNode* temp = head;
	PCB* p = head->data;
	head = head->next;
    /*FINAL_LOCK
	delete temp;
    FINAL_UNLOCK*/
	if (head == nullptr) tail = nullptr;
	return p;
}

bool PCBList::ManualDelete(PCB* p) {
    if (head == nullptr) return false;
	if (head->data == p) {
		if (head == tail) {
            HARD_LOCK
			delete head;
            HARD_UNLOCK
			head = tail = nullptr;
		}
		else {
			PCBNode* temp = head;
			head = head->next;
            HARD_LOCK
			delete temp;
            HARD_UNLOCK
		}
		return true;
	}
	PCBNode* temp = head;
	PCBNode* prev = nullptr;
	while (temp->data != p) {
		prev = temp;
		temp = temp->next;
		if (temp == nullptr) return false;
	}
	if (temp == tail) {
        HARD_LOCK
		delete temp;
        HARD_UNLOCK
		tail = prev;
		tail->next = nullptr;
	}
	else {
		prev->next = temp->next;
        HARD_LOCK
		delete temp;
        HARD_UNLOCK
	}
	return true;
}


void PCBList::startAll() {
    PCBNode* temp = head;
	while(temp != nullptr) {
		PCB* p = this->Delete();
		p->unblock();
		temp = temp->next;
	}
}

PCB* PCBList::getPCBById(ID id) {
    PCBNode* temp = head;
    while(temp != nullptr) {
        if(temp->data->myID == id) return temp->data;
        temp = temp->next;
    }
    return nullptr;
}

bool PCBList::isDeadlocked(PCB* p) {
	if(head == nullptr) return false;
	PCBNode* temp = head;
	while(temp != nullptr) {
		if(temp->data->myID == p->myID) return true;
		else temp = temp->next;
	}
	return false;
}


void PCBList::releasePCB(PCB* p) {
	if(head == nullptr) return;
	ManualDelete(p);
	PCBNode* temp = head;
	while(temp != nullptr) {
		temp->data->waitingList.ManualDelete(p);
		temp = temp->next;
	}
}

