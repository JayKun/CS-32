#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld; 

class Actor:public GraphObject
{
public: 
	Actor(int imageID, int start_x, int start_y, Direction dir, StudentWorld* world, double size, int depth); 
	virtual ~Actor(); 
	virtual void doSomething()=0 ; 
	bool isAlive() const;
	void setDead(); 
	StudentWorld* getWorld(); // so that all derived classes can call on StudentWorld
	bool isWithinRadius(int x, int y, int r);
	int getPlayerX(); 
	int getPlayerY(); 

private:
	bool m_Alive;
	StudentWorld* m_studentworld; 
	
};

//////////////////////////////////////////////////////////////////////////////////
//................................................................................
//LivingObject class
//...............................................................................
/////////////////////////////////////////////////////////////////////////////////

class LivingObject : public Actor
{
public:
	LivingObject(StudentWorld* world, int startX, int startY, Direction dir, int imageID);
	virtual ~LivingObject();
	virtual void annoy(int amt);
	// virtual void addGold();
	void setHitPoints(int value);
	int getHealth() const;
	bool canMoveThrough( int x, int y); 
	bool isProtesterblocking(int x, int y);
private:
	int m_hitpoints;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// --------------------------------------------------------------------------------
// FrackMan class
//..................................................................................
/////////////////////////////////////////////////////////////////////////////////////////////////


class FrackMan : public LivingObject
{
public:
	FrackMan(StudentWorld * world);
	virtual ~FrackMan();
	virtual void doSomething();
	int getSonar() const;
	int getWater() const;
	int getGold() const; 
	bool isPlayer(int x, int y);
	void setsVisibleWithinRadius(); 
	
	void addGold(); 
	void addWater();
	void addSonar(); 
	
private:
	void changeWater(int value);
	void changeSonar(int value);
	void changeGold(int value);
	void movePlayer(int ch, int cx, int cy);
	bool removeDirt(int cx, int cy);

	int m_water; 
	int m_sonar; 
	int m_gold;
};
/////////////////////////////////////////////////////////////
//..........................................................
/// Protester Class 
//.............................................................
///////////////////////////////////////////////////////////////



class Protester : public LivingObject
{
public:
	Protester(StudentWorld* world, int imageID);

	//void attackPlayer(); 

	//void leaveOilField(); 
	//virtual void addGold(); 

	int getState(); 
	void setState(int state);

	int getTicksToWait() const;
	void setTicksToWait(int v); 
	int getTicks() const;
	void resetTicks();
	void addTicks();

	int getCounter(); 
	void addCounter(); 
	void setCounter(int v);

	void setNumSquares(int v);
	int getNumSquares();
	int getTurningCounter() const; 
	void setTurningCounter(int v);
	

private:
	int m_protesterState;
	int m_protesterCounter;
	int m_ticksToWait;
	int m_ticks; 
	int m_turningCounter; 
	int numSquaresToMoveInCurrentDirection;

};

class RegularProtester : public Protester
{
public:
	RegularProtester(StudentWorld* world); 
	virtual void doSomething(); 
	int clearSightOfPlayer(); // returns the direction of FrackMan if FrackMan is in clear sight. 
	virtual void moveProtester(int x, int y, Direction dir);
	bool isFacingPlayer();
	int pickNewDirection();
	int canProtesterTurn();
	bool canMove(Direction dir); 

};


/*
class HardcoreProtester : public Protester
{
public:

};
*/
/////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// Dirt class
//..............................................................................
/////////////////////////////////////////////////////////////////////////////////

class Dirt : public Actor
{

public:
	Dirt(int x, int y, StudentWorld* world); 
	virtual ~Dirt();
	virtual void doSomething();  // more like Do NOTHING LOL ! 
};
///////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------
// Boulder Class
//---------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////

const int STABLE = 0;
const int WAITING = 1;
const int FALLING = 2; 

class Boulder : public Actor
{

public:
	Boulder(int x, int y, StudentWorld* world);
	virtual ~Boulder(); 
	virtual void doSomething(); 
	void addCounter(); 
	int getState(); 
	
private:
	int m_state; 
	int m_counter; 

};

//////////////////////////////////////////////////////////////////////////////////////
// .....................................................
// GOODIE CLASS //
//.......................................................
/////////////////////////////////////////////////////////////////////////////////////
class Goodie : public Actor
{
public:
	Goodie(int imageID, int startX, int startY, StudentWorld* world, Direction dir, int depth);
	void setTicksToLive(int t);
	void addTicks();
	int getTicks() const; 
	int getTicksToLive() const;
private:
	int m_ticksToLive; 
	int m_ticks;
};

class OilBarrel : public Goodie
{
public:
	OilBarrel(int startX, int startY, StudentWorld* world);
	virtual void doSomething(); 
	
};

class GoldNugget : public Goodie // 
{
public:
	GoldNugget(int startX, int startY, StudentWorld* world);
	virtual void doSomething(); 

};

class SonarKit : public Goodie 
{
public:
	SonarKit(int startX, int startY, StudentWorld* world);
	virtual void doSomething(); 
};

class WaterPool : public Goodie // 
{
public:
	WaterPool(int startX, int startY, StudentWorld* world);
	virtual void doSomething();
};


class Bribe : public Goodie
{
public:
	Bribe(StudentWorld* world, int startX, int startY);
	virtual void doSomething(); 
};

////////////////////////////////////////////////////////////////////////////////////
// .....................................................
//	SQUIRT CLASS //
//.......................................................
/////////////////////////////////////////////////////////////////////////////////////

class Squirt:public Actor
{
public:
	Squirt(StudentWorld* world, int cx, int cy, Direction dir ); 
	virtual void doSomething(); 
private:
	int travelDistance; 
};

#endif // ACTOR_H_
