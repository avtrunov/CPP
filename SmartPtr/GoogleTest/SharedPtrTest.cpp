#include "pch.h"
#include "SharedPtr.hpp"
#include <utility>
#include <string>

class SharedPtrTest : public ::testing::Test
{
protected:
	std::pair<int, char>* p = new std::pair<int, char>(1, 'a');
	SharedPtr<std::pair<int, char>> test{ p };
};

TEST_F(SharedPtrTest, DereferenceOperator)
{
	EXPECT_EQ(test->first, 1);
	EXPECT_EQ(test->second, 'a');
	test->first = 10;
	test->second = 'b';
	EXPECT_EQ(test->first, 10);
	EXPECT_EQ(test->second, 'b');

	EXPECT_EQ((*test).first, 10);
	EXPECT_EQ((*test).second, 'b');
	test->first = 100;
	test->second = 'c';
	EXPECT_EQ((*test).first, 100);
	EXPECT_EQ((*test).second, 'c');
}

TEST_F(SharedPtrTest, EqualityOperator)
{
	SharedPtr<int> zero;
	EXPECT_EQ(zero, nullptr);
	EXPECT_NE(test, nullptr);
	auto ptr = test;
	EXPECT_EQ(test, ptr);
}

TEST_F(SharedPtrTest, CopyConstructor)
{
	auto ptr1 = test;
	auto ptr2 = ptr1;
	EXPECT_EQ(test.Count(), 3);
	EXPECT_EQ(test, ptr2);
}

TEST_F(SharedPtrTest, MoveConstructor)
{
	auto ptr1 = std::move(test);
	EXPECT_NE(ptr1, test);
	EXPECT_EQ(test, nullptr);
	EXPECT_EQ(test.Count(), 0);
	EXPECT_EQ(ptr1.Count(), 1);
}

TEST_F(SharedPtrTest, CopyAssignment)
{
	SharedPtr<std::pair<int,char>> ptr1(new std::pair<int, char>{ 9,'s' });
	SharedPtr<std::pair<int, char>> ptr2(new std::pair<int, char>{ 7,'m' });
	ptr2 = ptr1;
	EXPECT_EQ(ptr1, ptr2);
	EXPECT_EQ(ptr2.Count(), 2);
	test = ptr2;
	EXPECT_EQ(test, ptr2);
	EXPECT_EQ(test.Count(), 3);
}

TEST_F(SharedPtrTest, MoveAssignment)
{
	SharedPtr<std::pair<int, char>> ptr1(new std::pair<int, char>{ 9,'s' });
	SharedPtr<std::pair<int, char>> ptr2(new std::pair<int, char>{ 7,'m' });
	ptr2 = ptr1;
	test = std::move(ptr2);
	EXPECT_NE(test, ptr2);
	EXPECT_EQ(test.Count(), 2);
	EXPECT_EQ(ptr2, nullptr);
}

TEST_F(SharedPtrTest, Reset)
{
	auto ptr1 = test;
	auto ptr2 = test;
	auto ptr3 = test;
	EXPECT_EQ(test.Count(), 4);
	ptr3.Reset();
	EXPECT_EQ(ptr3, nullptr);
	EXPECT_EQ(test.Count(), 3);
	ptr2.Reset();
	EXPECT_EQ(ptr2, nullptr);
	EXPECT_EQ(test.Count(), 2);
	ptr1.Reset();
	EXPECT_EQ(ptr1, nullptr);
	EXPECT_EQ(test.Count(), 1);
	test.Reset();
	EXPECT_EQ(test, nullptr);
	EXPECT_EQ(test.Count(), 0);
}

TEST_F(SharedPtrTest, CustomDeleter)
{
	auto customDeleter1 = [](int* num) {
		std::cout << "Testing customDeleter1.\n";
		delete num; };
	SharedPtr<int> ptr1(new int(1), customDeleter1);
	auto ptr2 = ptr1;

	auto customDeleter2 = [](std::pair<bool, int>* pr) {
		std::cout << "Testing customDeleter2.\n";
		delete pr; };
	SharedPtr<std::pair<bool, int>> ptr3(new std::pair<bool, int>{ true, 7 }, customDeleter2);
}
#include <memory>
TEST(MakeSharedTest, OneArg)
{
	auto ptr0 = MakeShared<int>();
	EXPECT_NE(ptr0, nullptr);
	EXPECT_EQ(*ptr0, 0);
	ptr0 = MakeShared<int>(10);
	EXPECT_EQ(*ptr0, 10);
	auto ptr2 = MakeShared<std::pair<bool, char>>({ true,'a' });
	EXPECT_EQ(ptr2->first, true);
	EXPECT_EQ(ptr2->second, 'a');
	auto ptr3 = MakeShared<std::string>("TEST");
	EXPECT_EQ(*ptr3, "TEST");
}

TEST(MakeSharedTest, TwoArg)
{
	auto customDeleter1 = [](int* num) {
		std::cout << "Testing customDeleter1.\n";
		delete num; };
	auto ptr1 = MakeShared<int>(100, customDeleter1);
	EXPECT_EQ(*ptr1, 100);

	auto customDeleter2 = [](std::string* pr) {
		std::cout << "Testing customDeleter2.\n";
		delete pr; };
	auto ptr2 = MakeShared<std::string>("TEST", customDeleter2);
	EXPECT_EQ(*ptr2, "TEST");
}