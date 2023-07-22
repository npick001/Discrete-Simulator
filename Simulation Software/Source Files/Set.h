#pragma once
#include "Utility.h"

#include <vector>

/*
Set - First In/First Out
This class handles objects for a FIFO LL scheme.
Mainly used for easy search and removal of object
*/
template <class T>
class Set
{
public:
	inline Set() {}

	// Adds item to the end of the list
	inline void Add(T* t)
	{
		m_items.push_back(t);
	}

	// Remove the first item from the set
	inline T* GetFirst()
	{
		T* first = m_items.front();
		m_items.erase(m_items.begin());
		return first;
	}

	// get the specific item from the set
	inline T* Get(T* toFind)
	{
		for (int i = 0; i < GetSize(); i++) {
			if (m_items[i] == toFind) {

				T* foundData = m_items[i];
				m_items.erase(m_items.begin() + i);
				return foundData;
			}
		}
		return nullptr;
	}

	T* GetRandom()
	{
		srand(time(NULL));
		int dataIndex = -1;
		
		if ((GetSize() > 1)) {
			dataIndex = rand() % GetSize();
		}
		else {
			dataIndex = 0;
		}

		T* returnData = m_items[dataIndex];
		m_items.erase(m_items.begin() + dataIndex);

		return returnData;
	}

	inline bool Has(T* toFind)
	{
		for (int i = 0; i < GetSize(); i++) {
			if (m_items[i] == toFind) {
				return true;
			}
		}
		return false;
	}

	// Delete all members in the Set
	inline void Clear()
	{
		m_items.clear();
	}

	// view the first item in the set
	inline T* View()
	{
		return(m_items.front());
	}

	// Do a deep copy of the passed Set to this one
	inline Set<T>& operator=(const Set<T>& other)
	{
		if (this != &other) {
			m_items = other.m_items;
		}
		return *this;
	}

	inline bool IsEmpty() { return (m_items.size() == 0); }
	inline int GetSize() { return m_items.size(); }
private:
	std::vector<T*> m_items;
};
