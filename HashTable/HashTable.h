#pragma once
#include "ListBucket.h"
#include "ArrayBucket.h"
#include <stdexcept>
#include <functional>


template <typename Key, typename Item,class Bucket = ListBucket<Key,Item>, class Hash = std::hash<Key>>
class HashTable
{
private:
	const unsigned int size;
	std::unique_ptr<Bucket[]> heads;
	void Copy(const HashTable& rhs);
public:
	HashTable() :size(67)
	{
		heads = std::make_unique<Bucket[]>(size);
	}
	explicit HashTable(int val) :size(val)
	{
		heads = std::make_unique<Bucket[]>(size);
	}
	HashTable(const HashTable& rhs):size(rhs.size) { this->Copy(rhs); }
	HashTable(HashTable&& rhs) noexcept :size(rhs.size), heads(std::move(rhs.heads)) {  }
	HashTable& operator=(const HashTable& rhs)
	{
		if (size != rhs.size)
			throw std::length_error("Trying to assign hash table of different size");
		if (this == &rhs)
			return *this;
		if (size != 0)
			heads.reset();
		this->Copy(rhs);
		return *this;
	}
	HashTable& operator=(HashTable&& rhs)
	{
		if (size != rhs.size)
			throw std::length_error("Trying to move hash table of different size");
		if (this == &rhs)
			return *this;
		heads = std::move(rhs.heads);
		return *this;
	}

	class iterator
	{
	private:
		Pair<const Key, Item>* ptr;
	public:
		iterator() :ptr(nullptr) {}
		iterator(Pair<const Key, Item>* pointer) :ptr(pointer) {}
		iterator(typename Bucket::iterator& iter) { ptr = &(*iter); }
		Pair<const Key, Item>& operator*() { return *ptr; }
		Pair<const Key, Item>* operator->() { return ptr; }
		bool operator==(const iterator& rhs) const { return ptr == rhs.ptr; }
		bool operator!=(const iterator& rhs) const { return !(ptr == rhs.ptr); }
	};

	void Insert(const Key& key, const Item& val);
	Item At(const Key& key) const;
	iterator Find(const Key& key) const;
	Item& operator[](const Key& key);
};

template <typename Key, typename Item, class Bucket, class Hash>
void HashTable<Key, Item, Bucket, Hash>::Copy(const HashTable<Key, Item, Bucket, Hash>& rhs)
{
	heads = std::make_unique<Bucket[]>(size);
	for (unsigned int i = 0; i < size; i++)
		heads[i] = rhs.heads[i];
}

template <typename Key, typename Item,class Bucket, class Hash>
void HashTable<Key, Item,Bucket, Hash>::Insert(const Key& key, const Item& val)
{
	int idx = Hash()(key) % size;
	auto iter = heads[idx].Search(key);
	if (iter == nullptr)
	{	
		Pair<const Key, Item> newData{ key,val };
		heads[idx].Push(newData);
	}
	else
		(*iter).item = val;
}

template <typename Key, typename Item,class Bucket, class Hash>
Item HashTable<Key, Item,Bucket, Hash>::At(const Key& key) const
{
	auto idx = Hash()(key) % size;
	auto iter = heads[idx].Search(key);
	if (iter != nullptr)
		return (*iter).item;
	throw std::out_of_range("Cannot find the element");
}

template <typename Key, typename Item, class Bucket, class Hash>
typename HashTable<Key,Item,Bucket,Hash>::iterator HashTable<Key, Item,Bucket, Hash>::Find(const Key& key) const
{
	auto idx = Hash()(key) % size;
	auto iter = heads[idx].Search(key);
	if (iter == nullptr)
		return nullptr;
	return iter;
}

template <typename Key, typename Item, class Bucket, class Hash>
Item& HashTable<Key, Item, Bucket,Hash>::operator[](const Key& key)
{
	auto idx = Hash()(key) % size;
	auto iter = heads[idx].Search(key);
	if (iter == nullptr)
	{
		Item item = { 0 };
		Pair<const Key, Item> defaultPair{ key, item };
		heads[idx].Push(defaultPair);
		iter = heads[idx].Search(key);
	}
	return (*iter).item;
}