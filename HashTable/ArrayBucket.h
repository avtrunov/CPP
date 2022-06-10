#pragma once
#include "Node.h"
#include <utility>
#include <iostream>

template <typename Key, typename Item>
class ArrayBucket
{
private:
	std::unique_ptr<std::unique_ptr<DANode<Key, Item>>[]> arr;
	unsigned int size;
	unsigned int capacity;
	void Reallocate();
	void Copy(const ArrayBucket<Key, Item>& rhs);
public:
	ArrayBucket() :size(0), capacity(0), arr(nullptr) {}
	ArrayBucket(const ArrayBucket& rhs) { this->Copy(rhs); }
	ArrayBucket(ArrayBucket&& rhs) noexcept :arr(std::move(rhs.arr)), size(rhs.size), capacity(rhs.capacity)
	{rhs.size = 0; rhs.capacity = 0;}
	ArrayBucket& operator=(const ArrayBucket& rhs);
	ArrayBucket& operator=(ArrayBucket&& rhs) noexcept;
	~ArrayBucket() = default;

	class iterator
	{
	private:
		DANode<Key, Item>* ptr;
	public:
		iterator() :ptr(nullptr){}
		iterator(DANode<Key, Item>* pointer) :ptr(pointer) {}
		Pair<const Key, Item>& operator*() { return ptr->data; }
		Pair<const Key, Item>* operator->() { return &ptr->data; }
		void operator++() { ptr = ptr->next; }
		void operator++(int) { ptr = ptr->next; }
		bool operator==(const iterator& rhs) const { return ptr == rhs.ptr; }
		bool operator!=(const iterator& rhs) const { return !(ptr == rhs.ptr); }
	};
	void Push(const Pair<const Key, Item>& data);
	iterator Search(const Key& data) const;
	iterator Begin() const;
	void Print() const;
};

template <typename Key, typename Item>
void ArrayBucket<Key, Item>::Copy(const ArrayBucket<Key, Item>& rhs)
{
	capacity = rhs.capacity;
	size = 0;
	arr = std::make_unique<std::unique_ptr<DANode<Key, Item>>[]>(capacity);
	for (auto iter = rhs.Begin(); iter != nullptr; iter++)
		this->Push(*iter);
}

template <typename Key, typename Item>
ArrayBucket<Key, Item>& ArrayBucket<Key, Item>::operator=(const ArrayBucket<Key, Item>& rhs)
{
	if (this == &rhs)
		return *this;
	if (size != 0)
		arr.reset();
	this->Copy(rhs);
	return *this;
}

template <typename Key, typename Item>
ArrayBucket<Key, Item>& ArrayBucket<Key, Item>::operator=(ArrayBucket<Key, Item>&& rhs) noexcept
{
	if (this == &rhs)
		return *this;
	size = rhs.size;
	capacity = rhs.capacity;
	arr = std::move(rhs.arr);
	rhs.size = 0;
	rhs.capacity = 0;
	return *this;
}

template <typename Key, typename Item>
void ArrayBucket<Key, Item>::Reallocate()
{
	capacity += 5;
	auto newArray = std::make_unique<std::unique_ptr<DANode<Key, Item>>[]>(capacity);
	for (unsigned int i = 0; i < size; i++)
		newArray[i] = std::move(arr[i]);
	arr = std::move(newArray);
	//std::cout << "Reallocated succesfully\n";
}

template <typename Key, typename Item>
void ArrayBucket<Key,Item>::Push(const Pair<const Key, Item>& data)
{
	if (size == capacity)
		Reallocate();
	arr[size] = std::make_unique<DANode<Key,Item>>(data);
	if (size != 0)
	{
		auto prevNode = arr[size - 1].get();
		prevNode->next = arr[size].get();
	}
	size++;
}

template <typename Key, typename Item>
typename ArrayBucket<Key,Item>::iterator ArrayBucket<Key, Item>::Search(const Key& data) const
{
	if (size == 0)
		return nullptr;
	for (auto iter = Begin(); iter != nullptr; iter++)
		if ((*iter).key == data)
			return iter;
	return nullptr;
}

template <typename Key, typename Item>
inline typename ArrayBucket<Key, Item>::iterator ArrayBucket<Key, Item>::Begin() const
{
	return (size == 0) ? nullptr : arr[0].get();
}

template <typename Key, typename Item>
void ArrayBucket<Key,Item>::Print() const
{
	if (size == 0)
	{
		std::cout << "The array is empty\n";
		return;
	}
	for (auto iter = Begin(); iter != nullptr; iter++)
		std::cout << (*iter).key << "-" << (*iter).item << ' ';
}