#pragma once
#include <iostream>
#include <memory>

template <typename Key, typename Item>
struct Pair
{
	Key key;
	Item item;
	Pair(const Key& k, const Item& i) :key(k), item(i) {}
};

//Node containing Key+Value
template <typename Key, typename Item>
struct Node
{
	Pair<const Key, Item> data;
	Node(const Pair<const Key, Item>& dt) :data(dt) {}
	virtual ~Node()
	{
		//std::cout << "Node with data " << data.key << '-' << data.item << " has been deleted\n";
	}
};

//Single-linked node
template <typename Key, typename Item>
struct SLNode :public Node<Key, Item>
{
	std::unique_ptr<SLNode> next;
	SLNode(const Pair<const Key, Item>& dt) : Node<Key, Item>(dt), next(nullptr) {}
};

//Dynamic array node
template <typename Key, typename Item>
struct DANode :public Node<Key, Item>
{
	DANode* next;
	DANode(const Pair<const Key, Item>& dt) : Node<Key, Item>(dt), next(nullptr) {}
};
