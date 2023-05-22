#ifndef FIFO_H
#define FIFO_H

/*
FIFO - First In/First Out
Defined using a template so that can be used to hold anything.  Type supplied when object is instantiated.
Template structure requires implementation to be provided in the header.
*/
template <class T>
class FIFO
{
public:
	FIFO()
	{
		_head = 0;
		_tail = 0;
		_size = 0;
	}

	void AddEntity(T *t)
	{
		Node *node = new Node(t);
		if (_head == 0) {	//empty list
			_head = _tail = node;
		}
		else {
			_tail = _tail->next = node;
		}
		_size++;
	}

	T *GetEntity()
	{
		if (_head == 0) return 0;
		else {
			Node *n = _head;
			T *t = _head->t;
			_head = _head->next;
//			delete n;
			_size--;
			return t;
		}
	}

	T *ViewEntity()
	{
		return(_head->t);
	}

	bool IsEmpty() { return (_size == 0); }
	int GetSize() { return _size; }
private:
	struct Node
	{
		Node(T *t) {
			this->t = t;
			next = 0;
		}
		T *t;
		Node *next;
	};

	Node *_head;
	Node *_tail;
	int _size;
};

#endif
