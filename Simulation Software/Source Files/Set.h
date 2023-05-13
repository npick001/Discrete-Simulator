#pragma once

template <class T>
class Set
{
public:
	Set()
	{
		_head = 0;
		_tail = 0;
		_size = 0;
	}

	// Adds item to the end of the list
	void AddEntity(T* t)
	{
		Node* node = new Node(t);
		if (_head == NULL) {	//empty list
			_head = _tail = node;
		}
		else {
			_tail = _tail->next = node;
		}
		_size++;
	}

	// Get the first item from the set
	T* GetEntity()
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

	// get the specific item from the set
	T* GetEntity(T* toFind)
	{
		if (_head == 0) return 0;
		else if (_head->t == toFind) {
			Node* n = _head;
			_head = _head->next;
			delete n;
			return toFind;
		}
		else {

			Node* prev = _head;
			Node* n = _head->next;
			T* t = _head->data;

			while (n != nullptr) {
				if (n->data == toFind) {
					t = n->data;
					prev->next = n->next;
					delete n;
					_size--;
					return t;
				}
				n = n->next;
			}
		}
	}

	bool HasEntity(T* toFind) {
		if (_head == 0) return false;
		else {

			Node* n = _head;

			while (n != nullptr) {
				if (n->data == toFind) {
					return true;					
				}
				n = n->next;
			}
			return false;
		}
	}

	// view the first item in the set
	T* ViewEntity()
	{
		return(_head->t);
	}

	bool IsEmpty() { return (_size == 0); }
	int GetSize() { return _size; }
private:
	struct Node
	{
		Node(T* t) {
			this->data = t;
			next = 0;
		}
		T* data;
		Node* next;
	};

	Node* _head;
	Node* _tail;
	int _size;
};
