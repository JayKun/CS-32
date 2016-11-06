#include "WeightMap.h"
#include "Map.h"
#include <iostream>
#include <string>
using namespace std; 

WeightMap::WeightMap()
{
}

bool WeightMap::enroll(std::string  name, double startWeight)
{	// If a person with the given name is not currently in the map, 
          // there is room in the map, and the startWeight is not negative,
          // add an entry for that person and weight and return true.
          // Otherwise make no change to the map and return false.
	if (startWeight > 0 && m_map.insert(name, startWeight))
		return true;
	else
		return false; 

}

double WeightMap::weight(std::string name) const
{
	// If a person with the given name is in the map, return that
	// person's weight; otherwise, return -1.
	double tempweight=-1;
	if (m_map.contains(name))
	{ 
		m_map.get(name,tempweight);
		return tempweight; 
	}
	else 
		return tempweight; 
}

bool WeightMap:: adjustWeight(std::string name, double amt)
{
	
	// If no person with the given name is in the map or if
	// weight() + amt is negative, make no change to the map and return
	// false.  Otherwise, change the weight of the indicated person by
	// the given amount and return true.  For example, if amt is -8.2,
	// the person loses 8.2 pounds; if it's 3.7, the person gains 3.7
	// pounds.
	if (m_map.contains(name) && weight(name) + amt < 0)
		return false;
	else
		m_map.update(name, weight(name) + amt); 
	return true;
}

int WeightMap::size() const
{
	// Return the number of people in the WeightMap.
	return m_map.size(); 

}

void WeightMap::print() const
{	
	int i = 0;
	KeyType print1; 
	ValueType print2;
	while (i < DEFAULT_MAX_ITEMS)
	{
		m_map.get(i, print1, print2);
		cout << print1 << " " << print2 << endl; 
		i++;
	}
	
}
