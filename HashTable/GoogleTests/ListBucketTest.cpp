#include "pch.h"
#include "../atrunov/Task1/ListBucket.h"
#include <string>
#include <utility>

class ListBucketTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		for (int i = 0; i < 3; i++)
			b1.Push(B1[i]);
		for (int i = 0; i < 4; i++)
			b2.Push(B2[i]);
		for (int i = 0; i < 3; i++)
			b3.Push(B3[i]);
	}

	ListBucket<int, int> b0;
	ListBucket<std::string, int> b1;
	Pair<const std::string, int> B1[3] = { {"Two",2},{"Five",5},{"Three",3} };
	ListBucket<int, std::string> b2;
	Pair<const int, std::string> B2[4] = { { 1,"One" } ,{ 2,"Two" } ,{ 6,"Six" }, { 9,"Nine" } };
	ListBucket<int, bool> b3;
	Pair<const int, bool> B3[3] = { { 1,true } ,{ 2,true },{ 3,true } };
};

TEST_F(ListBucketTest, IsEmptyInitially)
{
	EXPECT_EQ(b0.Begin(), nullptr);
}

TEST_F(ListBucketTest, PushFrontWorks)
{
	EXPECT_NE(b1.Begin(), nullptr);
	EXPECT_NE(b2.Begin(), nullptr);
	EXPECT_NE(b3.Begin(), nullptr);
	std::cout << "Test 1:"; b1.Print();
	std::cout << "\nTest 2:"; b2.Print();
	std::cout << "\nTest 3:"; b3.Print();
	std::cout << '\n';
}

TEST_F(ListBucketTest, SearchWorks)
{
	for (int i = 0; i < 3; i++)
	{
		auto iter1 = b1.Search(B1[i].key);
		EXPECT_EQ(B1[i].key, (*iter1).key) << "Key value: Expected " << B1[i].key << "|Received " << (*iter1).key;
		EXPECT_EQ(B1[i].item, (*iter1).item) << "Item value: Expected " << B1[i].item << "|Received " << (*iter1).item;
	}
	EXPECT_EQ(b1.Search("0"), nullptr);

	for (int i = 0; i < 4; i++)
	{
		auto iter2 = b2.Search(B2[i].key);
		EXPECT_EQ(B2[i].key, (*iter2).key) << "Key value: Expected " << B2[i].key << "|Received " << (*iter2).key;
		EXPECT_EQ(B2[i].item, (*iter2).item) << "Item value: Expected " << B2[i].item << "|Received " << (*iter2).item;
	}
	EXPECT_EQ(b2.Search(0), nullptr);

	for (int i = 0; i < 3; i++)
	{
		auto iter3 = b3.Search(B3[i].key);
		EXPECT_EQ(B3[i].key, (*iter3).key) << "Key value: Expected " << B3[i].key << "|Received " << (*iter3).key;
		EXPECT_EQ(B3[i].item, (*iter3).item) << "Item value: Expected " << B3[i].item << "|Received " << (*iter3).item;
	}
	EXPECT_EQ(b3.Search(0), nullptr);
}

TEST_F(ListBucketTest, IteratorWorks)
{
	auto iter1 = b1.Begin();
	(*iter1).item = 4;
	iter1 = b1.Begin();
	EXPECT_EQ((*iter1).item, 4) << "Item value: Expected " << 4 << "|Received " << (*iter1).item;

	auto iter2 = b2.Begin();
	(*iter2).item = "NINE";
	iter2 = b2.Begin();
	EXPECT_EQ((*iter2).item, "NINE") << "Item value: Expected " << "NINE" << "|Received " << (*iter2).item;

	auto iter3 = b3.Begin();
	(*iter3).item = false;
	iter3 = b3.Begin();
	EXPECT_EQ((*iter3).item, false) << std::boolalpha << "Item value: Expected " << false << "|Received " << (*iter3).item;
}

TEST_F(ListBucketTest, CopyConstructorWorks)
{
	auto b5 = b2;
	auto iter2 = b2.Begin();
	for (auto iter5 = b5.Begin(); iter5 != nullptr; iter5++)
	{
		EXPECT_EQ(iter2->key, iter5->key) << "Key value: Expected " << iter2->key << "|Received " << iter5->key;
		EXPECT_EQ(iter2->item, iter5->item) << "Item value: Expected " << iter2->item << "|Received " << iter5->item;
		iter2++;
	}
}

TEST_F(ListBucketTest, MoveConstructorWorks)
{
	auto b5 = std::move(b2);
	int i = 3;
	for (auto iter5 = b5.Begin(); iter5 != nullptr; iter5++)
	{
		EXPECT_EQ(B2[i].key, iter5->key) << "Key value: Expected " << B2[i].key << "|Received " << iter5->key;
		EXPECT_EQ(B2[i].item, iter5->item) << "Item value: Expected " << B2[i].item << "|Received " << iter5->item;
		i--;
	}
	EXPECT_EQ(b2.Begin(), nullptr);
}

TEST_F(ListBucketTest, CopyAssignmentWorks)
{
	ListBucket<int, std::string> b5;
	b5.Push({ 0,"Zero" });
	b5.Push({ 3,"Three" });
	b5 = b2;
	auto iter2 = b2.Begin();
	for (auto iter5 = b5.Begin(); iter5 != nullptr; iter5++)
	{
		EXPECT_EQ(iter2->key, iter5->key) << "Key value: Expected " << iter2->key << "|Received " << iter5->key;
		EXPECT_EQ(iter2->item, iter5->item) << "Item value: Expected " << iter2->item << "|Received " << iter5->item;
		iter2++;
	}
}

TEST_F(ListBucketTest, MoveAssignmentWorks)
{
	ListBucket<int, std::string> b5;
	b5.Push({ 0,"Zero" });
	b5.Push({ 3,"Three" });
	b5 = std::move(b2);
	int i = 3;
	for (auto iter5 = b5.Begin(); iter5 != nullptr; iter5++)
	{
		EXPECT_EQ(B2[i].key, iter5->key) << "Key value: Expected " << B2[i].key << "|Received " << iter5->key;
		EXPECT_EQ(B2[i].item, iter5->item) << "Item value: Expected " << B2[i].item << "|Received " << iter5->item;
		i--;
	}
	EXPECT_EQ(b2.Begin(), nullptr);
}