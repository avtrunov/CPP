#include <iostream>

enum class Color {black, red};

template <typename T>
struct Node
{
	Color color;
	T key;
	Node* p;
	Node* left;
	Node* right;
};

template <typename T>
class RBT
{
private:
	Node<T>* root;
	Node<T> nil;
	Node<T>* NewNode(T data);
	void PrintKey(Node<T>* node);
	void InorderWalk(Node<T>* node);
	Node<T>* Minimum(Node<T>* node);
	Node<T>* Search(T data, Node<T>* node);
	void LeftRotate(Node<T>* node);
	void RightRotate(Node<T>* node);
	void InsertFixup(Node<T>* node);
	void Transplant(Node<T>* u, Node<T>* v);
	void DeleteNode(Node<T>* deleting);
	void DeleteFixup(Node<T>* node);
public:
	RBT():root(&nil){}
	void Display();
	void Insert(T data);
	void Delete(T data);
	~RBT();
	RBT(const RBT<T>& rhs) = delete;
	RBT<T>& operator=(const RBT<T>& rhs) = delete;
};

template <typename T>
RBT<T>::~RBT()
{
	while (root != &nil)
		DeleteNode(root);
	std::cout << "Tree has been deleted\n";
}

int k = 0;
template <typename T>
void RBT<T>::PrintKey(Node<T>* node)
{
	char clr = node->color == Color::black ? 'b' : 'r';
	std::cout.width(2 * k);
	std::cout << node->key << clr << '\n';
}

template <typename T>
void RBT<T>::InorderWalk(Node<T>* node)
{
	k++;
	if (node != &nil)
	{
		InorderWalk(node->right);
		PrintKey(node);
		InorderWalk(node->left);
	}
	k--;
}

template <typename T>
void RBT<T>::Display()
{
	if (root != &nil)
		InorderWalk(root);
	else
		std::cout << "The tree is empty\n";
}

template <typename T>
Node<T>* RBT<T>::Minimum(Node<T>* node)
{
	while (node->left != &nil)
		node = node->left;
	return node;
}

template <typename T>
Node<T>* RBT<T>::Search(T data, Node<T>* node)
{
	auto current = node;
	while (current != &nil && current->key != data)
	{
		if (data < current->key)
			current = current->left;
		else
			current = current->right;
	}
	return current;
}

template <typename T>
void RBT<T>::LeftRotate(Node<T>* node)
{
	auto replace = node->right;
	node->right = replace->left;
	if (replace->left != &nil)
		replace->left->p = node;
	replace->p = node->p;
	if (node->p == &nil)
		root = replace;
	else if (node == node->p->left)
		node->p->left = replace;
	else
		node->p->right = replace;
	replace->left = node;
	node->p = replace;
}

//Ex.13.2.1. Đĺŕëčçŕöč˙ RightRotate
template <typename T>
void RBT<T>::RightRotate(Node<T>* node)
{
	auto replace = node->left;
	node->left = replace->right;
	if (replace->right != &nil)
		replace->right->p = node;
	replace->p = node->p;
	if (node->p == &nil)
		root = replace;
	else if (node == node->p->left)
		node->p->left = replace;
	else
		node->p->right = replace;
	replace->right = node;
	node->p = replace;
}

template <typename T>
Node<T>* RBT<T>::NewNode(T data)
{
	auto newnode = new Node<T>;
	newnode->color = Color::black;
	newnode->p = newnode->left = newnode->right = nullptr;
	newnode->key = data;
	return newnode;
}

template <typename T>
void RBT<T>::Insert(T data)
{
	auto newnode = NewNode(data);
	auto prev = &nil;
	auto current = root;
	while (current != &nil)
	{
		prev = current;
		if (data < current->key)
			current = current->left;
		else
			current = current->right;
	}
	newnode->p = prev;
	if (prev == &nil)
		root = newnode;
	else if (newnode->key < prev->key)
		prev->left = newnode;
	else
		prev->right = newnode;
	newnode->left = newnode->right = &nil;
	newnode->color = Color::red;
	InsertFixup(newnode);
}

template <typename T>
void RBT<T>::InsertFixup(Node<T>* node)
{
	while (node->p->color == Color::red)
	{
		if (node->p == node->p->p->left)
		{
			auto y = node->p->p->right;
			if (y->color == Color::red)
			{
				node->p->color = Color::black;
				y->color = Color::black;
				node->p->p->color = Color::red;
				node = node->p->p;
			}
			else 
			{
				if (node == node->p->right)
				{
					node = node->p;
					LeftRotate(node);
				}
			node->p->color = Color::black;
			node->p->p->color = Color::red;
			RightRotate(node->p->p);
			}
		}
		else
		{
			auto y = node->p->p->left;
			if (y->color == Color::red)
			{
				node->p->color = Color::black;
				y->color = Color::black;
				node->p->p->color = Color::red;
				node = node->p->p;
			}
			else
			{
				if (node == node->p->left)
				{
					node = node->p;
					RightRotate(node);
				}
				node->p->color = Color::black;
				node->p->p->color = Color::red;
				LeftRotate(node->p->p);
			}
		}
	}
	root->color = Color::black;
}

template <typename T>
void RBT<T>::Transplant(Node<T>* u, Node<T>* v)
{
	if (u->p == &nil)
		root = v;
	else if (u == u->p->left)
		u->p->left = v;
	else
		u->p->right = v;
	v->p = u->p;
}

template <typename T>
void RBT<T>::DeleteNode(Node<T>* deleting)
{
	Node<T>* x;
	auto y = deleting;
	auto y_original_color = y->color;
	if (deleting->left == &nil)
	{
		x = deleting->right;
		Transplant(deleting, deleting->right);
	}
	else if (deleting->right == &nil)
	{
		x = deleting->left;
		Transplant(deleting, deleting->left);
	}
	else
	{
		y = Minimum(deleting->right);
		y_original_color = y->color;
		x = y->right;
		if (y->p == deleting)
			x->p = y;
		else
		{
			Transplant(y, y->right);
			y->right = deleting->right;
			y->right->p = y;
		}
		Transplant(deleting, y);
		y->left = deleting->left;
		y->left->p = y;
		y->color = deleting->color;
	}
	if (y_original_color == Color::black)
		DeleteFixup(x);
	delete deleting;
}

template <typename T>
void RBT<T>::DeleteFixup(Node<T>* node)
{
	while (node != root && node->color == Color::black)
	{
		if (node == node->p->left)
		{
			auto w = node->p->right;
			if (w->color == Color::red)
			{
				w->color = Color::black;
				node->p->color = Color::red;
				LeftRotate(node->p);
				w = node->p->right;
			}
			if (w->left->color == Color::black && w->right->color == Color::black)
			{
				w->color = Color::red;
				node = node->p;
			}
			else
			{
				if (w->right->color == Color::black)
				{
					w->left->color = Color::black;
					w->color = Color::red;
					RightRotate(w);
					w = node->p->right;
				}
				w->color = node->p->color;
				node->p->color = Color::black;
				w->right->color = Color::black;
				LeftRotate(node->p);
				node = root;
			}
		}
		else
		{
			auto w = node->p->left;
			if (w->color == Color::red)
			{
				w->color = Color::black;
				node->p->color = Color::red;
				RightRotate(node->p);
				w = node->p->left;
			}
			if (w->right->color == Color::black && w->left->color == Color::black)
			{
				w->color = Color::red;
				node = node->p;
			}
			else
			{
				if (w->left->color == Color::black)
				{
					w->right->color = Color::black;
					w->color = Color::red;
					LeftRotate(w);
					w = node->p->left;
				}
				w->color = node->p->color;
				node->p->color = Color::black;
				w->left->color = Color::black;
				RightRotate(node->p);
				node = root;
			}
		}
	}
	node->color = Color::black;
}

template <typename T>
void RBT<T>::Delete(T data)
{
	auto to_delete = Search(data, root);
	if (to_delete == &nil)
		std::cout << "Cannot find the node\n";
	else
	{
		DeleteNode(to_delete);
		std::cout << "The node has been deleted\n";
	}
}

int main()
{
	RBT<int> tree;
	int ch;
	int data;
	do
	{
		std::cout << "0.Exit\n1.Insert\n2.Display\n3.Delete\n->";
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
		case 2:tree.Display(); break;
		case 3:
		{
			std::cout << "Enter data: ";
			std::cin >> data;
			tree.Delete(data);
			break;
		}
		}
	} while (ch != 0);
	return 0;
}
