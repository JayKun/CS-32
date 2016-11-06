#pragma once
#include "Map.h"
#ifndef WEIGHTMAP_H
#define WEIGHTMAP_H
class WeightMap
{
public:
	WeightMap();       // Create an empty weight map.
	bool enroll(std::string name,double startWeight);
	double weight(std::string  name) const;
	bool adjustWeight(std::string  name, double amt);
	int size() const;  
	void print() const;

private:
	Map m_map; 
};
#endif