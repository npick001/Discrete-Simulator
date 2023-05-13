#pragma once

// Min heap by default, going to add switching later
template <class T>
class Heap {
public:
	// the type is true for Min Heap, false for Max Heap
	Heap(int heapsize, bool type) {
		_heap = new Node * [heapsize];
		_heapType = type;
	}

	T* GetElement()
	{
		T* root = _heap[0];
		_heap[0] = _heap[_size - 1];
		_size--;
		Heapify(0);
		return root;
	}

	void Insert(T* t) {
		_heap[_size] = new Node(t);
		_size++;
	}

	void SetHeapSize(int heapsize) {

		for (int i = 0; i < _size; i++) {
			delete _heap[i];
		}
		delete _heap;

		_heap = new Node * [heapsize];
	}

private:
	Heapify(int start) {
		// lc = 2i+1
		// Rc = 2i+2
		// P = floor[(i-1)/2]

		int i = currNode;
		int lc = 2 * i + 1;
		int rc = 2 * i + 2;

		switch (_heapType) {
		case true:
			// check if left child is smaller than current 
			if (lc < _size && _heap[lc]->_time < _heap[i]->_time) {
				i = lc;
			}
			// check if right child is smaller than current 
			if (rc < _size && _heap[rc]->_time < _heap[i]->_time) {
				i = rc;
			}
			break;

		case false:
			// check if left child is smaller than current 
			if (lc > _size && _heap[lc]->_time > _heap[i]->_time) {
				i = lc;
			}
			// check if right child is smaller than current 
			if (rc > _size && _heap[rc]->_time > _heap[i]->_time) {
				i = rc;
			}
			break;
		}


		if (i != currNode) {
#if SIM_OUTPUT
			std::cout << "SWAPPING " << _heap[i]->data << " AND " << _heap[currNode]->data << "\t\t HEAP SIZE: " << _size << std::endl;
#endif // SIM_OUTPUT
			std::swap(_heap[currNode], _heap[i]);

			Heapify(i);
		}
	}

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

	Node** _heap;
	bool _heapType;
};