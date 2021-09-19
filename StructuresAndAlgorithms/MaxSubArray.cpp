#include <iostream>
#include <tuple>
#include <vector>

template<typename T>
std::tuple<int, int, T> FindMaxCrossingSubArray(const std::vector<T>& A, int low, int mid, int high)
{
	T left_sum = A[mid] - 1,
		right_sum = A[mid + 1] - 1,
		sum = 0;
	int max_right, max_left;
	for (int i = mid; i >= low; i--)
	{
		sum += A[i];
		if (sum > left_sum)
		{
			left_sum = sum;
			max_left = i;
		}
	}
	sum = 0;
	for (int i = mid + 1; i <= high; i++)
	{
		sum += A[i];
		if (sum > right_sum)
		{
			right_sum = sum;
			max_right = i;
		}
	}
	return std::make_tuple(max_right, max_left, left_sum + right_sum);
}

template<typename T>
std::tuple<int, int, T> FindMaxSubArray(const std::vector<T>& A, int low, int high)
{
	if (high == low)
		return std::make_tuple(low, high, A[low]);
	int mid = (high + low) / 2;
	auto left_result = FindMaxSubArray(A, low, mid);
	auto right_result = FindMaxSubArray(A, mid + 1, high);
	auto cross_result = FindMaxCrossingSubArray(A, low, mid, high);
	if (std::get<2>(left_result) > std::get<2>(right_result) && std::get<2>(left_result) > std::get<2>(cross_result))
		return left_result;
	else if (std::get<2>(right_result) > std::get<2>(left_result) && std::get<2>(right_result) > std::get<2>(cross_result))
		return right_result;
	else
		return cross_result;
}

//Ex. 4.1.5. Реализовать итеративный алгоритм. Сложность O(n)
template<typename T>
std::tuple<int, int, T> FindMaxSubArrayIter(const std::vector<T>& A, int low, int high)
{
	std::tuple<int, int, T> max_result = { low , low , A[low] };
	decltype(max_result) result;
	int left_max = low, left_min = low;
	T sum = A[low], max_sum = A[low];
	for (int j = low+1; j <= high; j++)
	{
		sum += A[j];
		if (sum > 0)
		{
			if (sum > max_sum)
			{
				left_max = j;
				max_sum = sum;
			}
		}
		else
		{
			result = { left_max, left_min, max_sum };
			if (std::get<2>(result) > std::get<2>(max_result))
				max_result = result;
			left_min = left_max = j + 1;
			sum = 0;
			max_sum = A[j + 1] - 1;
		}
	}
	result = { left_max, left_min, max_sum };
	if (std::get<2>(result) > std::get<2>(max_result))
		max_result = result;
	return max_result;
}

int main()
{
	std::vector<double> A;
	//A = { 13.1,-3,-25,9,11,-3,-16,-23,18,1.5,19,-7,12,-5,-22,15,-4};
	//A = {1 };
	//A = { -3,-4,-1,-9, -2 };
	A = { 1,1,1,-1,-1,1,1,-1,-1,-1,-1,-1,-1,1,1,-1,1,1,1,-1,-1,1 };
	for (int i = 0; i <A.size(); i++)
		std::cout << A[i] << ' ';
	std::cout << '\n';

	std::cout << "Recurrent algorithm\n";
	auto result = FindMaxSubArray(A, 0, A.size() - 1);
	for (int i = std::get<1>(result); i <= std::get<0>(result); i++)
		std::cout << A[i] << ' ';
	std::cout << "\nSum: " << std::get<2>(result) << '\n';

	std::cout << "Iterative algorithm\n";
	result = FindMaxSubArrayIter(A, 0, A.size() - 1);
	for (int i = std::get<1>(result); i <= std::get<0>(result); i++)
		std::cout << A[i] << ' ';
	std::cout << "\nSum: " << std::get<2>(result) << '\n';
	return 0;
}
