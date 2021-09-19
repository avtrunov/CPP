#include <iostream>
#include <vector>
#include <algorithm>

template <typename T>
class Heap
{
private:
	std::vector<T>* A;
	int Parent(int i);
	int Left(int i);
	int Right(int i);
public:
	int size;
	Heap() :A(nullptr), size(0) {}
	Heap(std::vector<T>* arr);
	void MaxHeapifyRec(int i);
	void MaxHeapifyIter(int i);
	Heap& operator= (Heap& A) = delete;
	Heap(Heap& A) = delete;
};

template <typename T>
Heap<T>::Heap(std::vector<T>* arr)
{
	size = arr->size();
	A = arr;
	for (int i = size / 2 - 1; i >= 0; i--)
		MaxHeapifyIter(i);
}

template <typename T>
int Heap<T>::Parent(int i)
{
	return static_cast<int>((i + 1) / 2) - 1;
}

template <typename T>
int Heap<T>::Left(int i)
{
	return 2 * i + 1;
}

template <typename T>
int Heap<T>::Right(int i)
{
	return 2 * i + 2;
}

template <typename T>
void Heap<T>::MaxHeapifyRec(int i)
{
	int largest;
	int left = Left(i);
	int right = Right(i);
	if (left < size && (*A)[left] >(*A)[i])
		largest = left;
	else
		largest = i;
	if (right < size && (*A)[right] >(*A)[largest])
		largest = right;
	if (largest != i)
	{
		std::swap(*A[i], *A[largest]);
		MaxHeapifyRec(largest);
	}
}

//6.2.5. Написать итеративную версию MaxHeapify
template <typename T>
void Heap<T>::MaxHeapifyIter(int i)
{
	int largest;
	for (int j = i;; j = largest)
	{
		int left = Left(j);
		int right = Right(j);
		if (left < size && (*A)[left] >(*A)[j])
			largest = left;
		else
			largest = j;
		if (right < size && (*A)[right] >(*A)[largest])
			largest = right;
		if (largest != j)
			std::swap((*A)[j], (*A)[largest]);
		else
			break;
	}
}

template <typename T>
void HeapSort(std::vector<T>& arr)
{
	Heap<T> A(&arr);
	for (int i = A.size - 1; i > 0; i--)
	{
		std::swap(arr[0], arr[i]);
		A.size--;
		A.MaxHeapifyIter(0);
	}
}

int main()
{
	std::vector<double> A;
	A = { 2.3, 3.2, 0.1, 3.12, 9.43, 8.43, 5.19, 7.67, 4.47, 0.33 };
	for (int i = 0; i < A.size(); i++)
		std::cout << A[i] << ' ';
	std::cout << "\nResult:\n";
	HeapSort(A);
	for (int i = 0; i < A.size(); i++)
		std::cout << A[i] << ' ';
	std::cout << '\n';
	return 0;
}
