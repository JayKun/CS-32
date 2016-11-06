#include <iostream>
#include <string>
using namespace std;


class Landmark
{
public:
	Landmark(string name)
	{
		m_name = name; 
	}
	virtual string name() const
	{
		return m_name; 
	}
	virtual string color() const
	{
		return "yellow"; 
	}
	
	virtual string icon()const =0; 

	virtual ~Landmark() {}; 
private:
	string m_name;
};

class Restaurant :public Landmark
{
public:
	~Restaurant()
	{
		cout << "Destroying the restaurant " << Landmark::name() << "." << endl; 
	}

	Restaurant(string name, int capacity) : Landmark(name)
	{	
		m_capacity = capacity; 
	}

	string icon() const
	{
		if (m_capacity < 40)
			return "small knife";
		else 
			return "large knife"; 
	}
private:
	int m_capacity;
};

class Hotel : public Landmark
{
public:
	
	Hotel(string name) :Landmark(name)
	{}

	~Hotel()
	{
		cout << "Destroying the hotel " << Landmark::name() << "." << endl;
	}
	string icon() const
	{
		return "bed"; 
	}

};

class Hospital : public  Landmark
{
public:
	Hospital(string name) :Landmark(name) 
	{}
	~Hospital()
	{
		cout << "Destroying the hospital " << Landmark::name() << "." << endl;
	}
	virtual string color() const
	{
		return "blue";
	}
	string icon() const
	{
		return "H"; 
	}
private:
};

void display(const Landmark* lm)
{
	cout << "Display a " << lm->color() << " " << lm->icon() << " icon for "
		<< lm->name() << "." << endl;
}
