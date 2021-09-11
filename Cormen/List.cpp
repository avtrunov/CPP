#include <iostream>

template <typename T>
struct link
{
	T data;
	link* next;
	link* prev;
};
//Двусвязный список
template <typename T>
class List
{
protected:
	link<T>* first;
	link<T>* NewLink(T val);
	link<T>* Search(T val);
	void DeleteLink(link<T>* delLink);
public:
	List():first(nullptr){}
	List(List<T>& L);
	List<T>& operator=(List<T>& L);
	void PushBack(T val);
	void PushFront(T val);
	void Delete(T val);
	void Display();
	void Reverse();
	~List();
	template<typename U> friend class Stack;
	template<typename U> friend class Queue;
};

template <typename T>
void List<T>::DeleteLink(link<T>* delLink)
{
	if (!delLink->prev)
	{
		first = delLink->next;
		if (first != nullptr)
			first->prev = nullptr;
	}
	else if (!delLink->next)
		delLink->prev->next = nullptr;
	else
	{
		delLink->prev->next = delLink->next;
		delLink->next->prev = delLink->prev;
	}
	delete delLink;
}

template <typename T>
link<T>* List<T>::Search(T val)
{
	for (link<T>* current = first; current != nullptr; current = current->next)
		if (current->data == val)
			return current;
	return nullptr;
}

template <typename T>
link<T>* List<T>::NewLink(T val)
{
	link<T>* newlink = new link<T>;
	newlink->data = val;
	newlink->next = newlink->prev = nullptr;
	return newlink;
}

template <typename T>
List<T>::List(List<T>& L)
{
	first = nullptr;
	for (link<T>* current = L.first; current != nullptr; current = current->next)
		this->PushBack(current->data);
}

template <typename T>
List<T>& List<T>::operator=(List<T>& L)
{
	link<T>* current;
	if (this->first == L.first)
		return *this;
	if (this->first)
	{
		current = first;
		while (current)
		{
			link<T>* next = current->next;
			delete current;
			current = next;
		}
		first = nullptr;
	}
	for (current = L.first; current != nullptr; current = current->next)
		this->PushBack(current->data);
	return *this;
}

template <typename T>
void List<T>::Delete(T val)
{
	link<T>* delLink = Search(val);
	if (!delLink)
	{
		std::cout << "Cannot find element\n";
		return;
	}
	this->DeleteLink(delLink);
	std::cout << "Data has been deleted\n";
}

template <typename T>
void List<T>::PushFront(T val)
{
	if (first == nullptr)
		first = NewLink(val);
	else
	{
		link<T>* newfirst = NewLink(val);
		first->prev = newfirst;
		newfirst->next = first;
		first = newfirst;
	}
}

template <typename T>
void List<T>::PushBack(T val)
{
	if (first == nullptr)
		first = NewLink(val);
	else
	{
		link<T>* current;
		for (current = first; current->next != nullptr; current = current->next){}
		current->next = NewLink(val);
		current->next->prev = current;
	}
}

//Ex. 10.2.7. Обратите односвязный список нерекурсивно со временнем работы O(n). Расход памяти O(1).
template <typename T>
void List<T>::Reverse()
{
	link<T>* last = first;
	link<T>* current = first->next;
	while (last->next != nullptr)
	{
		last->next = current->next;
		current->next = first;
		first = current;
		current = last->next;
	}
}

template <typename T>
List<T>::~List()
{
	link<T>* current = first;
	while (current)
	{
		link<T>* next = current->next;
		std::cout << current->data << " Deleted\n";
		delete current;
		current = next;
	}
}

template <typename T>
void List<T>::Display()
{
	for (link<T>* current = first; current != nullptr; current = current->next)
		std::cout << current->data << ' ';
	std::cout << '\n';
}

//Ex. 10.2.2. Реализуйте стэк с помощью списка. Push и Pop за O(1)
template <typename T>
class Stack
{
private:
	List<T> L;
	link<T>* top;
public:
	Stack():top(nullptr){}
	bool Empty();
	void Push(T val);
	T Pop();
	void Display();
};

template <typename T>
bool Stack<T>::Empty()
{
	return top == nullptr;
}

template <typename T>
void Stack<T>::Push(T val)
{
	L.PushFront(val);
	top = L.first;
}

template <typename T>
T Stack<T>::Pop()
{
	if (Empty())
	{
		std::cerr << "Stack is empty\n";
		exit(1);
	}
	T temp = top->data;
	L.DeleteLink(top);
	top = L.first;
	return temp;
}

template <typename T>
void Stack<T>::Display()
{
	if (Empty())
		std::cout << "Stack is empty\n";
	L.Display();
}

//Ex. 10.2.3. Реализуйте очередь с помощью списка. Push и Pop за O(1)
template <typename T>
class Queue
{
private:
	List<T> L;
	link<T>* head;
	link<T>* tail;
public:
	Queue():head(nullptr),tail(nullptr){}
	bool Empty();
	void Push(T val);
	T Pop();
	void Display();
};

template <typename T>
bool Queue<T>::Empty()
{
	return head == nullptr;
}

template <typename T>
void Queue<T>::Push(T val)
{
	L.PushFront(val);
	if (Empty())
		head = L.first;
	tail = L.first;
}

template <typename T>
T Queue<T>::Pop()
{
	if (Empty())
	{
		std::cerr << "Queue is empty\n";
		exit(1);
	}
	T temp = head->data;
	link<T>* newhead = head->prev;
	L.DeleteLink(head);
	head = newhead;
	return temp;
}

template <typename T>
void Queue<T>::Display()
{
	if (Empty())
		std::cout << "Queue is empty\n";
	L.Display();
}

int main()
{
	List<double> L;
	int ch;
	double val;
	/*Queue<int> Q;
	//Stack<int> S;
	do
	{
		std::cout << "\n0.Exit\n1.Push\n2.Pop\n3.Display\n->";
		std::cin >> ch;
		switch (ch)
		{
		case 1:
		{
			std::cout << "Enter value: ";
			std::cin >> val;
			Q.Push(val);
			break;
		}
		case 2: std::cout << "Pop it value: " << Q.Pop(); break;
		case 3:Q.Display(); break;
		}
	} while (ch != 0);*/
	
	do
	{
		std::cout << "\n0.Exit\n1.Push front\n2.Push back\n3.Display\n4.Delete\n5.Reverse\n->";
		std::cin >> ch;
		switch (ch)
		{
		case 1:
		{
			std::cout << "Enter value: ";
			std::cin >> val;
			L.PushFront(val);
			break;
		}
		case 2:
		{
			std::cout << "Enter value: ";
			std::cin >> val;
			L.PushBack(val);
			break;
		}
		case 3:L.Display(); break;
		case 4:
		{
			std::cout << "Enter value: ";
			std::cin >> val;
			L.Delete(val);
			break;
		}
		case 5:L.Reverse(); std::cout << "List has been reversed\n"; break;
		}
	} while (ch != 0);
	return 0;
}