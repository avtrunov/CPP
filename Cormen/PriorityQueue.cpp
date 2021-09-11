//Ex. 6.5.7. Реализация очереди с приоритетом
#include <iostream>
#include <algorithm>
#include <list>
#include <vector>

template <typename T>
class PQueue
{
private:
	static const int N = 20;
	T arr[N];
	int size;
	void MaxHeapify(int i);
	void IncreaseKey(T key);
	int Parent(int i);
	int Left(int i);
	int Right(int i);
public:
	PQueue() : size(0) {}
	void Push(T key);
	T Max();
	T Pop();
	T Delete(int i);
};

template <typename T>
int PQueue<T>::Parent(int i)
{
	return static_cast<int>((i + 1) / 2) - 1;
}

template <typename T>
int PQueue<T>::Left(int i)
{
	return 2 * i + 1;
}

template <typename T>
int PQueue<T>::Right(int i)
{
	return 2 * i + 2;
}

template <typename T>
void PQueue<T>::IncreaseKey(T key)
{
	for (int i = size - 1; Parent(i) >= 0; i = Parent(i))
	{
		if (arr[i] > arr[Parent(i)])
		{
			std::swap(arr[i], arr[Parent(i)]);
		}
		else
			break;
	}
}

template <typename T>
void PQueue<T>::MaxHeapify(int i)
{
	int largest;
	for (int j = i;; j = largest)
	{
		int left = Left(j);
		int right = Right(j);
		if (left < size && arr[left] >arr[j])
			largest = left;
		else
			largest = j;
		if (right < size && arr[right] >arr[largest])
			largest = right;
		if (largest != j)
			std::swap(arr[j], arr[largest]);
		else
			break;
	}
}

template <typename T>
void PQueue<T>::Push(T key)
{
	if (size == N)
	{
		std::cerr << "Queue is full\n";
		return;
	}
	arr[size++] = key;
	IncreaseKey(key);
}

template <typename T>
T PQueue<T>::Max()
{
	return arr[0];
}

template <typename T>
T PQueue<T>::Pop()
{
	return Delete(0);
}

//Ex. 6.5.8. Удаление i-го узла за O(log(n))
template <typename T>
T PQueue<T>::Delete(int i)
{
	if (i >= size)
	{
		std::cerr << "Array index out of bounds\n";
		exit(1);
	}
	T temp = arr[i];
	std::swap(arr[i], arr[size - 1]);
	size--;
	MaxHeapify(i);
	return temp;
}

//Ex. 6.5.9. Слияние k отсортированных списков за O(n+k log(k))
template <typename T>
struct Pair
{
	T key;
	int index;
	bool operator>(const Pair& p)
	{
		return key > p.key;
	}
};

template <typename T>
std::list<T> MergeLists(std::vector< std::list<T> >& v)
{
	PQueue< Pair<T> > heap;
	std::list<T> result;
	int n = 0, k = v.size();
	Pair<T> pair;
	for (int i = 0; i < k; i++)
	{
		n += v[i].size();
		pair.index = i;
		pair.key = v[i].front();
		v[i].pop_front();
		heap.Push(pair);
	}
	for (int i = 0; i < n-k; i++)
	{
		auto pair = heap.Pop();
		result.push_front(pair.key);
		pair.key = v[pair.index].front();
		v[pair.index].pop_front();
		heap.Push(pair);
	}
	for (int i = 0; i < k; i++)
	{
		auto pair = heap.Pop();
		result.push_front(pair.key);
	}
	return result;
}

template <typename T>
void printList(const std::list<T>& list)
{
	for (auto const& i : list) {
		std::cout << i << ' ';
	}
	std::cout << '\n';
}

int main()
{
	std::list<double> L1 = { 8,5,3,1 },
		L2 = { 7,6,4,2,1 },
		L3 = { 10,9,8,2 };
	printList(L1);
	printList(L2);
	printList(L3);
	std::vector< std::list<double> > v;
	v.push_back(L1);
	v.push_back(L2);
	v.push_back(L3);
	auto result = MergeLists(v);
	printList(result);
	return 0;
}