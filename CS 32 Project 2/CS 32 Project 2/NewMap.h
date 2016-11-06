#pragma once
#include <iostream>
typedef std::string KeyType;
typedef  double ValueType;
const int DEFAULT_MAX_ITEMS = 200;

struct Data
{
	KeyType  KeyData;
	ValueType ValueData;
	bool is_empty; // true if data object is empty.. 
};

class Map
{
public:
	Map();         // Create an empty map (i.e., one with no key/value pairs)
	Map(int n);
	bool empty() const;  // Return true if the map is empty, otherwise false.
	int size() const;    // Return the number of key/value pairs in the map.
	bool insert(const KeyType& key, const ValueType& value);
	bool update(const KeyType& key, const ValueType& value);
	bool insertOrUpdate(const KeyType& key, const ValueType& value);
	bool erase(const KeyType& key);
	bool contains(const KeyType& key) const;
	bool get(const KeyType& key, ValueType& value);
	bool get(int i, KeyType& key, ValueType& value);
	void swap(Map& other);
	void dump() const;
private:
	Data* list;
	int m_n;
};


