#pragma once

/*
Set - First In/First Out
This class handles objects for a FIFO LL scheme.
Mainly used for easy search and removal of object
*/
template <class T>
class Set
{
public:
	inline Set()
	{
		_head = 0;
		_tail = 0;
		_size = 0;
	}

	// Adds item to the end of the list
	inline void Add(T* t)
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

	// Remove the first item from the set
	inline T* GetFirst()
	{
		if (_head == 0) return 0;
		else {
			Node* n = _head;
			T* data = _head->data;
			_head = _head->next;
			_size--;
			return data;
		}
	}

	// get the specific item from the set
	inline T* Get(T* toFind)
	{
		if (_head == 0) return 0;
		else if (_head->data == toFind) {
			Node* n = _head;
			_head = _head->next;
			delete n;
			return toFind;
		}
		else {

			Node* prev = _head;
			Node* n = _head->next;
			T* data = _head->data;

			while (n != nullptr) {
				if (n->data == toFind) {
					data = n->data;
					prev->next = n->next;
					delete n;
					_size--;
					return data;
				}
				n = n->next;
			}
		}
		return nullptr; // if not found
	}

	inline bool Has(T* toFind)
	{
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

	// Delete all members in the Set
	inline void Empty()
	{
		while (_head != nullptr) {
			auto temp = GetFirst();
		}
		_size = 0;
	}

	// view the first item in the set
	inline T* View()
	{
		return(_head->t);
	}

	// Do a deep copy of the passed Set to this one
	inline Set<T>& operator=(const Set<T>& other)
	{
		if (this != &other) {
			Empty();
			Node* n = other._head;

			// traverse the set and copy all members to this
			while (n != nullptr) {
				Add(n->data);
				n = n->next;
			}
		}
		return *this;
	}

	inline bool IsEmpty() { return (_size == 0); }
	inline int GetSize() { return _size; }
private:
	struct Node
	{
		Node(T* data) {
			this->data = data;
			next = 0;
		}
		T* data;
		Node* next;
	};

	Node* _head;
	Node* _tail;
	int _size;
};
