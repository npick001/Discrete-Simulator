#pragma once

/*
* Sorted Linked List
Defined using a template so that can be used to hold anything.  Type supplied when object is instantiated.
Template structure requires implementation to be provided in the header.
*/
template <class T>
class LinkedList
{
public:
	LinkedList()
	{
		_head = 0;
		_tail = 0;
		_size = 0;
	}

	void AddNode(T* t)
	{
		Node* e = new Node(t);
		if (_list == 0) {
			//event list empty
			_list = e;
		}
		else if (time < _eventList->_time) {
			//goes at the head of the list
			e->_nextEvent = _eventList;
			_eventList = e;
		}
		else {
			//search for where to put the event
			Event* curr = _eventList;
			while ((curr->_nextEvent != 0) ? (e->_time >= curr->_nextEvent->_time) : false) {
				curr = curr->_nextEvent;
			}
			if (curr->_nextEvent == 0) {
				//goes at the end of the list
				curr->_nextEvent = e;
			}
			else {
				e->_nextEvent = curr->_nextEvent;
				curr->_nextEvent = e;
			}
		}
	}

	T* GetNode()
	{
		if (_head == 0) return 0;
		else {
			Node* n = _head;
			T* t = _head->t;
			_head = _head->next;
			_size--;
			return t;
		}
	}

	T* ViewNode()
	{
		return(_head->t);
	}

	bool IsEmpty() { return (_size == 0); }
	int GetSize() { return _size; }
private:
	struct Node
	{
		Node(T* t) {
			this->t = t;
			next = 0;
		}
		T* t;
		Node* next;
	};

	Node* _head;
	Node* _tail;
	int _size;
};
