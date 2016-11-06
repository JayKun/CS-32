#include "Map.h"
#include <iostream>
#include <string>
using namespace std;
Map::Map()        // Create an empty map (i.e., one with no key/value pairs)
{
	for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
	{
		list[i].is_empty = true;
	}
}

bool Map::empty() const  // Return true if the map is empty, otherwise false.
{
	for (int i = 0; i <DEFAULT_MAX_ITEMS; i++)
	{
		if (!list[i].is_empty)
			return false;
	}
	return true;
}

int Map::size() const   // Return the number of key/value pairs in the map.
{
	int counter = 0;
	for (int i = 0; i< DEFAULT_MAX_ITEMS; i++)
	{
		if (!list[i].is_empty)
			counter++;
	}
	return counter;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	bool a = false;
	for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
	{
		if (key != list[i].KeyData && this->size() != DEFAULT_MAX_ITEMS )
		{
			a = true;
		}
	}
	if (a)
	{
		list[this->size()].KeyData = key;
		list[this->size()].ValueData = value;
		list[this->size()].is_empty = false;
	}
	return a;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
	for (int i = 0; i<size(); i++)
	{
		if (key == list[i].KeyData && list[i].is_empty==false)
		{
			list[i].ValueData = value;
			return true;
		}
	}
	return false;
}
bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{   // If key is equal to a key currently in the map, then make that key no
	// longer map to the value it currently maps to, but instead map to
	// the value of the second parameter; return true in this case.
	// If key is not equal to any key currently in the map and if the
	// key/value pair can be added to the map, then do so and return true.
	// Otherwise, make no change to the map and return false (indicating
	// that the key is not already in the map and the map has a fixed
	// capacity and is full).
	for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
	{
		if (list[i].KeyData == key && list[i].is_empty==false)
		{
			update(key, value);
			return true;
		}
	}
	insert(key, value);
	return false;
}

bool Map::erase(const KeyType& key)
{
	// If key is equal to a key currently in the map, remove the key/value
	// pair with that key from the map and return true.  Otherwise, make
	// no change to the map and return false.
	for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
	{
		if (key == list[i].KeyData && list[i].is_empty==false)
		{
			list[i].is_empty = true;
			return true;
		}
	}
	return false;
}

bool Map::contains(const KeyType& key) const
{
	// Return true if key is equal to a key currently in the map, otherwise
	// false.
	for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
	{
		if (key == list[i].KeyData && list[i].is_empty == false)
			return true;
	}
	return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	// If key is equal to a key currently in the map, set value to the
	// value in the map that that key maps to, and return true.  Otherwise,
	// make no change to the value parameter of this function and return
	// false.
	for (int i = 0; i < size(); i++)
	{
		if (key == list[i].KeyData && list[i].is_empty==false)
		{
			value = list[i].ValueData;
			return true;
		}
	}
	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const

{ // If 0 <= i < size(), copy into the key and value parameters the
  // key and value of one of the key/value pairs in the map and return
  // true.  Otherwise, leave the key and value parameters unchanged and
  // return false.  (See below for details about this function.)
	if (i >= 0 && i < this->size() && list[i].is_empty == false)
	{
		key = list[i].KeyData;
		value = list[i].ValueData;
		return true;
	}
	else
		return false; 
}

void Map::swap(Map& other)
{// Exchange the contents of this map with the other one
	
	for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
	{	
		Map temp; 
		temp.list[i] = list[i];
		list[i] = other.list[i];
		other.list[i] = temp.list[i];
	}
	return;
}

