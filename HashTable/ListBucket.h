#pragma once
#include "Node.h"
#include <utility>
#include <iostream>

template <typename Key, typename Item>
class ListBucket
{
private:
	std::unique_ptr <SLNode<Key, Item>> first;
	void PopFront();
	void Copy(const ListBucket& rhs);
public:
	ListBucket() :first(nullptr) {}
	ListBucket(const ListBucket& rhs) { this->Copy(rhs); }
	ListBucket(ListBucket&& rhs) noexcept :first(std::move(rhs.first)) {}
	ListBucket& operator=(const ListBucket& rhs);
	ListBucket& operator=(ListBucket&& rhs) noexcept;
	~ListBucket() = default;

	class iterator
	{
	private:
		SLNode<Key, Item>* ptr;
	public:
		iterator() :ptr(nullptr) {}
		iterator(SLNode<Key, Item>* pointer) :ptr(pointer) {}
		Pair<const Key, Item>& operator*() { return ptr->data; }
		Pair<const Key, Item>* operator->() { return &ptr->data; }
		void operator++() { ptr = (ptr->next).get(); }
		void operator++(int) { ptr = (ptr->next).get(); }
		bool operator==(const iterator& rhs) const { return ptr == rhs.ptr; }
		bool operator!=(const iterator& rhs) const { return !(ptr == rhs.ptr); }
	};

	void Push(const Pair<const Key, Item>& data);
	iterator Search(const Key& data) const;
	iterator Begin() const;
	void Print() const;
};

template <typename Key, typename Item>
void ListBucket<Key, Item>::PopFront()
{
	if (first != nullptr)
		first = std::move(first->next);
}

template <typename Key, typename Item>
void ListBucket<Key, Item>::Copy(const ListBucket<Key, Item>& rhs)
{
	ListBucket<Key, Item> stack;
	for (auto iter = rhs.Begin(); iter != nullptr; iter++)
		stack.Push(*iter);
	for (auto iter = stack.Begin(); iter != nullptr; iter = stack.Begin())
	{
		this->Push(*iter);
		stack.PopFront();
	}
}

template <typename Key, typename Item>
ListBucket<Key,Item>& ListBucket<Key,Item>::operator=(const ListBucket<Key, Item>& rhs)
{
	if (this == &rhs)
		return *this;
	if (first != nullptr)
		first.reset();
	this->Copy(rhs);
	return *this;
}

template <typename Key, typename Item>
ListBucket<Key, Item>& ListBucket<Key, Item>::operator=(ListBucket<Key, Item>&& rhs) noexcept
{
	if (this == &rhs)
		return *this;
	first = std::move(rhs.first);
	return *this;
}

template <typename Key, typename Item>
inline typename ListBucket<Key, Item>::iterator ListBucket<Key, Item>::Begin() const
{
	return first.get();
}

template <typename Key, typename Item>
void ListBucket<Key, Item>::Push(const Pair<const Key, Item>& data)
{
	if (first == nullptr)
		first = std::make_unique<SLNode<Key, Item>>(data);
	else
	{
		auto newFirst = std::make_unique<SLNode<Key, Item>>(data);
		newFirst->next = std::move(first);
		first = std::move(newFirst);
	}
}

template <typename Key, typename Item>
typename ListBucket<Key, Item>::iterator ListBucket<Key, Item>::Search(const Key& val) const
{
	for (auto iter = Begin(); iter != nullptr; iter++)
	{
		if ((*iter).key == val)
			return iter;
	}
	return nullptr;
}

template <typename Key, typename Item>
void ListBucket<Key, Item>::Print() const
{
	for (auto iter = Begin(); iter != nullptr; iter++)
		std::cout << (*iter).key << '-' << (*iter).item << ' ';
	if (first == nullptr)
		std::cout << "The list is empty.\n";
}