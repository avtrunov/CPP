#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <tuple>

template <typename T>
int Partition(std::vector<T>& A, int p, int r)
{
	T x = A[r];
	int i = p, j;
	for (j = p; j < r; j++)
		if (A[j] <= x)
			std::swap(A[i++], A[j]);
	std::swap(A[i], A[r]);
	return i;
}

template <typename T>
void QuickSort(std::vector<T>& A, int begin, int end)
{
	if (begin < end)
	{
		int q = Partition(A, begin, end);
		QuickSort(A, begin, q - 1);
		QuickSort(A, q + 1, end);
	}
}

//Рандомизированная
template <typename T>
int RandomizedPartition(std::vector<T>& A, int p, int r)
{
	std::random_device rd;  
	std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> distrib(p, r);
	int i = distrib(gen);
	std::swap(A[i], A[r]);
	return Partition(A, p, r);
}

template <typename T>
void RandomizedQuickSort(std::vector<T>& A, int begin, int end)
{
	if (begin < end)
	{
		int q = RandomizedPartition(A, begin, end);
		RandomizedQuickSort(A, begin, q - 1);
		RandomizedQuickSort(A, q + 1, end);
	}
}

//Ex 7.2.б. Быстрая сортировка с равными элементами
template <typename T>
std::tuple<int,int> PartitionEq(std::vector<T>& A, int p, int r)
{
	T x = A[r];
	int i = p, j, k = p;
	for (j = p; j < r; j++)
	{
		if (A[j] < x)
		{
			std::swap(A[i], A[j]);
			std::swap(A[i++], A[k++]);
		}
		else if (A[j] == x)
			std::swap(A[i++], A[j]);
	}
	std::swap(A[i], A[r]);
	return std::make_tuple(k, i);
}

template <typename T>
void QuickSortEq(std::vector<T>& A, int begin, int end)
{
	if (begin < end)
	{
		auto q_t = PartitionEq(A, begin, end);
		QuickSort(A, begin, std::get<0>(q_t) - 1); //...begin, q - 1)
		QuickSort(A, std::get<1>(q_t) + 1, end); //...,t + 1, end)
	}
}

//Ex 7.2.в. Рандомизированная быстрая сортировка с равными элементами
template <typename T>
std::tuple<int, int> RandomizedPartitionEq(std::vector<T>& A, int p, int r)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(p, r);
	int i = distrib(gen);
	std::swap(A[i], A[r]);
	return PartitionEq(A, p, r);
}

template <typename T>
void RandomizedQuickSortEq(std::vector<T>& A, int begin, int end)
{
	if (begin < end)
	{
		auto q_t = RandomizedPartitionEq(A, begin, end);
		RandomizedQuickSortEq(A, begin, std::get<0>(q_t) - 1); //...begin, q - 1)
		RandomizedQuickSortEq(A, std::get<1>(q_t) + 1, end); //...,t + 1, end)
	}
}

int main()
{ 
	std::vector<int> A = { 1,5,2,10,9,8,3,0,4 },
		B = { 1,5,3,2,4,1,4,4,2,5,3,1,3 };
	std::for_each(A.begin(), A.end(), [](auto x) {std::cout << x << ' '; });
	std::cout << '\n';
	QuickSort(A, 0, A.size() - 1);
	std::for_each(A.begin(), A.end(), [](auto x) {std::cout << x << ' '; });
	std::cout << '\n';

	std::for_each(B.begin(), B.end(), [](auto x) {std::cout << x << ' '; });
	std::cout << '\n';
	RandomizedQuickSortEq(B, 0, B.size() - 1);
	std::for_each(B.begin(), B.end(), [](auto x) {std::cout << x << ' '; });
	return 0;
}
