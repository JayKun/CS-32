
#include "NewMap.h"
#include <cassert>
#include <iostream>
using namespace std;

int main() 
{
	Map a(1000);   // a can hold at most 1000 key/value pairs
	Map b(5);      // b can hold at most 5 key/value pairs
	Map c;         // c can hold at most DEFAULT_MAX_ITEMS key/value pairs
	KeyType k[6] = { "lol", "wtf", "a", "b", "c", "d" };
	ValueType v = 3;

	// No failures inserting pairs with 5 distinct keys into b
	for (int i = 0; i < 5; i++)
	assert(b.insert(k[i], v));

	// Failure if we try to insert a pair with a sixth distinct key into b
	assert(!b.insert(k[5], v));

	// When two Maps' contents are swapped, their capacities are swapped
	// as well:
	a.swap(b);
	assert(!a.insert(k[5], v) && b.insert(k[5], v));
}