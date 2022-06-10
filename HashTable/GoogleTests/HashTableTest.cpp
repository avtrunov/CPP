#include "pch.h"
#include "../atrunov/Task1/HashTable.h"
#include <string>
#include <stdexcept>

class HashTableTest :public ::testing::Test
{
protected:
	void SetUp() override
	{
		std::string a("a");
		for (int i = 0; i < 20; i++)
		{
			h1.Insert(a, i);
			h2.Insert(i, a);
			a += "b";
		}
	}
	HashTable<std::string, int> h1;
	HashTable<int, std::string, ArrayBucket<int, std::string>> h2;
};

TEST_F(HashTableTest, AtWorks)
{
	std::string a("a");
	for (int i = 0; i < 20; i++)
	{
		EXPECT_EQ(h1.At(a),i) << "Item value: Expected " << i << "|Received " << h1.At(a);
		EXPECT_EQ(h2.At(i), a) << "Item value: Expected " << a << "|Received " << h2.At(i);
		a += "b";
	}
	EXPECT_THROW(h1.At("ThrowUp"), std::out_of_range);
	EXPECT_THROW(h2.At(-1), std::out_of_range);
}

TEST_F(HashTableTest, FindWorks)
{
	std::string a("a");
	for (int i = 0; i < 20; i++)
	{
		auto iter1 = h1.Find(a);
		EXPECT_EQ(iter1->key, a) << "Key value: Expected " << a << "|Received " << iter1->key;
		EXPECT_EQ(iter1->item, i) << "Item value: Expected " << i << "|Received " << iter1->item;
		auto iter2 = h2.Find(i);
		EXPECT_EQ(iter2->key, i) << "Key value: Expected " << i << "|Received " << iter2->key;
		EXPECT_EQ(iter2->item, a) << "Item value: Expected " << a << "|Received " << iter2->item;
		a += "b";
	}
	auto iter1 = h1.Find("NONE");
	EXPECT_EQ(iter1, nullptr);
	auto iter2 = h2.Find(-1);
	EXPECT_EQ(iter2, nullptr);
}

TEST_F(HashTableTest, IteratorWorks)
{
	auto iter1 = h1.Find("abbbb");
	iter1->item = -4;
	EXPECT_EQ(h1.At("abbbb"), -4) << "Item value: Expected " << -4 << "|Received " << h1.At("abbbb");
	auto iter2 = h2.Find(5);
	iter2->item = "CHANGED";
	EXPECT_EQ(h2.At(5), "CHANGED") << "Item value: Expected " << "CHANGED" << "|Received " << h2.At(5);
}

TEST_F(HashTableTest, AccessOperatorWorks)
{
	std::string a("a");
	for (int i = 0; i < 20; i++)
	{
		EXPECT_EQ(h1[a], i) << "Item value: Expected " << i << "|Received " << h1[a];
		auto iter2 = h2.Find(i);
		EXPECT_EQ(h2[i], a) << "Item value: Expected " << a << "|Received " << h2[i];
		a += "b";
	}
	h1["abbb"] = 100;
	EXPECT_EQ(h1["abbb"],100) << "Item value: Expected " << 100 << "|Received " << h1[a];
	h2[19] = "b";
	EXPECT_EQ(h2[19], "b") << "Item value: Expected " << "b" << "|Received " << h2[19];
	h1["NonExistingKey"] = 666;
	EXPECT_EQ(h1["NonExistingKey"], 666) << "Item value: Expected " << 666 << "|Received " << h1["NonExistingKey"];
	h2[-1] = "Negative";
	EXPECT_EQ(h2[-1], "Negative") << "Item value: Expected " << "Negative" << "|Received " << h2[-1];
}

TEST_F(HashTableTest, CopyConstuctorWorks)
{
	auto h3 = h1;
	auto h4 = h2;
	std::string a("a");
	for (int i = 0; i < 20; i++)
	{
		auto iter1 = h1.Find(a);
		auto iter3 = h3.Find(a);
		EXPECT_EQ(iter1->key, iter3->key) << "Key value: Expected " << iter1->key << "|Received " << iter3->key;
		EXPECT_EQ(iter1->item, iter3->item) << "Item value: Expected " << iter1->item << "|Received " << iter3->item;
		a += "b";
		auto iter2 = h2.Find(i);
		auto iter4 = h4.Find(i);
		EXPECT_EQ(iter2->key, iter4->key) << "Key value: Expected " << iter2->key << "|Received " << iter4->key;
		EXPECT_EQ(iter2->item, iter4->item) << "Item value: Expected " << iter2->item << "|Received " << iter4->item;
	}
}

TEST_F(HashTableTest, CopyAssignmentWorks)
{
	EXPECT_NO_FATAL_FAILURE(h1 = h1);
	HashTable<std::string, int> h3;
	h3 = h1;
	HashTable<int, std::string, ArrayBucket<int, std::string>> h4;
	h4[0] = "Stuff";
	h4[5] = "String";
	h4[10] = "Something";
	h4 = h2;
	std::string a("a");
	for (int i = 0; i < 20; i++)
	{
		auto iter1 = h1.Find(a);
		auto iter3 = h3.Find(a);
		EXPECT_EQ(iter1->key, iter3->key) << "Key value: Expected " << iter1->key << "|Received " << iter3->key;
		EXPECT_EQ(iter1->item, iter3->item) << "Item value: Expected " << iter1->item << "|Received " << iter3->item;
		a += "b";
		auto iter2 = h2.Find(i);
		auto iter4 = h4.Find(i);
		EXPECT_EQ(iter2->key, iter4->key) << "Key value: Expected " << iter2->key << "|Received " << iter4->key;
		EXPECT_EQ(iter2->item, iter4->item) << "Item value: Expected " << iter2->item << "|Received " << iter4->item;
	}
	HashTable<std::string, int> s(10);
	EXPECT_THROW(s = h1, std::length_error);
}

TEST_F(HashTableTest, MoveConstructorWorks)
{
	auto h3 = std::move(h1);
	auto h4 = std::move(h2);
	std::string a("a");
	for (int i = 0; i < 20; i++)
	{
		auto iter3 = h3.Find(a);
		EXPECT_EQ(iter3->key, a) << "Key value: Expected " << a << "|Received " << iter3->key;
		EXPECT_EQ(iter3->item, i) << "Item value: Expected " << i << "|Received " << iter3->item;
		auto iter4 = h4.Find(i);
		EXPECT_EQ(iter4->key, i) << "Key value: Expected " << i << "|Received " << iter4->key;
		EXPECT_EQ(iter4->item, a) << "Item value: Expected " << a << "|Received " << iter4->item;
		a += "b";
	}
}

TEST_F(HashTableTest, MoveAssignmentWorks)
{
	EXPECT_NO_FATAL_FAILURE(h1 = std::move(h1));
	HashTable<std::string, int> h3;
	h3 = std::move(h1);
	HashTable<int, std::string, ArrayBucket<int, std::string>> h4;
	h4[0] = "Stuff";
	h4[5] = "String";
	h4[10] = "Something";
	h4 = std::move(h2);
	std::string a("a");
	for (int i = 0; i < 20; i++)
	{
		auto iter3 = h3.Find(a);
		EXPECT_EQ(iter3->key, a) << "Key value: Expected " << a << "|Received " << iter3->key;
		EXPECT_EQ(iter3->item, i) << "Item value: Expected " << i << "|Received " << iter3->item;
		auto iter4 = h4.Find(i);
		EXPECT_EQ(iter4->key, i) << "Key value: Expected " << i << "|Received " << iter4->key;
		EXPECT_EQ(iter4->item, a) << "Item value: Expected " << a << "|Received " << iter4->item;
		a += "b";
	}
	HashTable<std::string, int> h5(10);
	EXPECT_THROW(h5 = std::move(h3), std::length_error);
}
