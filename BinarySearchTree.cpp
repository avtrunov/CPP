#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>

template <typename T>
struct Node
{
	T key;
	Node* p;
	Node* left;
	Node* right;
};

template <typename T>
class BST
{
private:
	Node<T>* root;
	Node<T>* SearchRec(T data, Node<T>* node);
	Node<T>* SearchIter(T data, Node<T>* node);
	Node<T>* Minimum(Node<T>* node);
	Node<T>* Maximum(Node<T>* node);
	Node<T>* Successor(Node<T>* node);
	Node<T>* Predecessor(Node<T>* node);
	Node<T>* NewNode(T data);
	void Transplant(Node<T>* u, Node<T>* v);
	void DeleteNode(Node<T>* deleting);
	void InorderWalkRec(Node<T>* node);
	void InorderWalkIter(Node<T>* node);
	void PrintKey(Node<T>* node, int level, char delim);
public:
	BST() :root(nullptr) {}
	BST(const std::vector<T>& vec);
	void Insert(T data);
	void Delete(T data);
	void Display();
	std::vector<T> ToVector();
	~BST();
	BST(BST<T>& rhs) = delete;
	BST<T>& operator=(BST<T>& rhs) = delete;
};

template < typename T>
BST<T>::BST(const std::vector<T>& vec):root(nullptr)
{

	for (int i = 0; i < vec.size(); i++)
		Insert(vec[i]);
}

template <typename T>
BST<T>::~BST()
{
	while (root != nullptr)
		DeleteNode(root);
	std::cout << "Tree has been deleted\n";
}

template <typename T>
void BST<T>::PrintKey(Node<T>* node, int level, char delim)
{
	std::cout.width(3 * level);
	std::cout << node->key << delim;
}

int k = 0;
template <typename T>
void BST<T>::Display()
{
	if (root == nullptr)
		std::cout << "Tree is empty\n";
	else
	{
		InorderWalkRec(root);
		std::cout << '\n';
		k = 0;
	}
}

template <typename T>
void BST<T>::InorderWalkRec(Node<T>* node)
{
	k++;
	if (node != nullptr)
	{
		InorderWalkRec(node->right);
		PrintKey(node, k, '\n');
		InorderWalkRec(node->left);
	}
	k--;
}

//Ex. 12.2.7. Итеративный обход дерева в симметричном порядке
template <typename T>
void BST<T>::InorderWalkIter(Node<T>*node)
{
	auto current = Minimum(node);
	while (current)
	{
		PrintKey(current, 0, ' ');
		current = Successor(current);
	}
}

template <typename T>
Node<T>* BST<T>::SearchRec(T data, Node<T>* node)
{
	if (node == nullptr || node->key == data)
		return node;
	if (data < node->key)
		return SearchRec(data, node->left);
	else
		return SearchRec(data, node->right);
}

template <typename T>
Node<T>* BST<T>::SearchIter(T data, Node<T>* node)
{
	Node<T>* current = node;
	while (current != nullptr && current->key != data)
	{
		if (data < current->key)
			current = current->left;
		else
			current = current->right;
	}
	return current;
}

template <typename T>
Node<T>* BST<T>::Minimum(Node<T>* node)
{
	Node<T>* current;
	for (current = node; current->left != nullptr; current = current->left);
	return current;
}

template <typename T>
Node<T>* BST<T>::Maximum(Node<T>* node)
{
	Node<T>* current;
	for (current = node; current->right != nullptr; current = current->right);
	return current;
}

template <typename T>
Node<T>* BST<T>::Successor(Node<T>* node)
{
	if (node->right != nullptr)
		return Minimum(node->right);
	Node<T>* prev = node->p;
	while (prev != nullptr && node == prev->right)
	{
		node = prev;
		prev = prev->p;
	}
	return prev;
}

//Ex. 12.2.3. Реализация процедуры Predecessor
template <typename T>
Node<T>* BST<T>::Predecessor(Node<T>* node)
{
	if (node->left != nullptr)
		return Maximum(node->left);
	Node<T>* prev = node->p;
	while (prev != nullptr && node == prev->left)
	{
		node = prev;
		prev = prev->p;
	}
	return prev;
}

template <typename T>
Node<T>* BST<T>::NewNode(T data)
{
	Node<T>* newnode = new Node<T>;
	newnode->key = data;
	newnode->p = newnode->left = newnode->right = nullptr;
	return newnode;
}

template <typename T>
void BST<T>::Insert(T data)
{
	auto newnode = NewNode(data);
	Node<T>* current = root;
	Node<T>* prev = nullptr;
	while (current != nullptr)
	{
		prev = current;
		if (newnode->key < current->key)
			current = current->left;
		else
			current = current->right;
	}
	newnode->p = prev;
	if (prev == nullptr)
		root = newnode;
	else if (newnode->key < prev->key)
		prev->left = newnode;
	else
		prev->right = newnode;
}

template <typename T>
void BST<T>::Transplant(Node<T>* u, Node<T>* v)
{
	if (u->p == nullptr)
		root = v;
	else if (u == u->p->left)
		u->p->left = v;
	else
		u->p->right = v;
	if (v != nullptr)
		v->p = u->p;
}

template <typename T>
void BST<T>::DeleteNode(Node<T>* deleting)
{
	if (deleting->left == nullptr)
		Transplant(deleting, deleting->right);
	else if (deleting->right == nullptr)
		Transplant(deleting, deleting->left);
	else
	{
		auto sucsr = Minimum(deleting->right);
		if (sucsr->p != deleting)
		{
			Transplant(sucsr, sucsr->right);
			sucsr->right = deleting->right;
			sucsr->right->p = sucsr;
		}
		Transplant(deleting, sucsr);
		sucsr->left = deleting->left;
		sucsr->left->p = sucsr;
	}
	delete deleting;
}

template <typename T>
void BST<T>::Delete(T data)
{
	auto delNode = SearchIter(data, root);
	if (delNode != nullptr)
	{
		DeleteNode(delNode);
		std::cout << "Node " << data << " has been deleted\n";
	}
	else
		std::cout << "Wrong value\n";
}

template <typename T>
std::vector<T> BST<T>::ToVector()
{
	std::vector<T> result;
	auto current = Minimum(root);
	for (int i = 0; current != nullptr; i++)
	{
		result.emplace_back(current->key);
		current = Successor(current);
	}
	return result;
}

template <typename T> void Sort(std::vector<T>& vec);

int main()
{
	BST<int> tree;
	int ch;
	int data;
	do
	{
		std::cout << "0.Exit\n1.Insert\n2.Delete\n3.Display\n4.Sort vector\n->";
		std::cin >> ch;
		switch (ch)
		{
		case 1:
		{
			std::cout << "Enter data: "; 
			std::cin >> data;
			tree.Insert(data);
			break;
		}
		case 2:
		{
			std::cout << "Enter data: ";
			std::cin >> data;
			tree.Delete(data);
			break;
		}
		case 3:tree.Display(); break;
		case 4:
		{
			std::vector<double> V = { 6.33,7.32,0,23,3.0,6.1,9.5,3.6,5.52, 2.11 };
			std::for_each(V.cbegin(), V.cend(), [](auto x) {std::cout << x << ' '; });
			std::cout << '\n';
			Sort(V);
			std::for_each(V.cbegin(), V.cend(), [](auto x) {std::cout << x << ' '; });
			std::cout << '\n';
			break;
		}
		}
	} while (ch != 0);
	return 0;
}

//Ex.12.3.3. Отсортировать при помощи двоичного дерева
template <typename T>
void Sort(std::vector<T>& vec)
{
	BST<T> tree(vec);
	tree.Display();
	vec = tree.ToVector();
}
