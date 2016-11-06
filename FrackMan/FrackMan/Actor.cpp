#include "Actor.h"
#include "GraphObject.h"
#include "StudentWorld.h"
#include <algorithm>

using namespace std; 
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, int start_x, int start_y, Direction dir, StudentWorld* world, double size, int depth) :GraphObject(imageID, start_x, start_y, dir,size, depth )
{
	setVisible(true); 
	setDirection(dir); 
	m_Alive = true; 
	m_studentworld = world;
}



Actor::~Actor()
{
	;
}

bool Actor :: isAlive() const
{
	return m_Alive; 
}

void Actor::setDead()
{
	m_Alive = false; 
}

StudentWorld* Actor::getWorld() 
{
	return m_studentworld; 
}

int Actor::getPlayerX()
{
	return m_studentworld->getPlayer()->getX();
}
int Actor::getPlayerY()
{
	return m_studentworld->getPlayer()->getY();
}

bool Actor::isWithinRadius(int x, int y, int r)
{	
	if (this == nullptr)
		return false;
	int cx = getX();
	int cy = getY();
	if ((x - cx)*(x - cx) + (cy -y)*(cy - y) <= r*r)
		return true;
	return false;
}



//------------------------------------------------------------------------------------
// LivingObject CLASS  ////////
//.....................................................................................
LivingObject::LivingObject( StudentWorld*world, int startX, int startY, Direction dir, int imageID) : Actor(imageID, startX, startY, dir, world, 1.0, 0)
{
	m_hitpoints = 0; 

}

LivingObject::~LivingObject()
{
	;
}
void LivingObject:: setHitPoints(int value)
{
	m_hitpoints = value;
}

int LivingObject::getHealth() const
{
	return m_hitpoints;
}

void LivingObject::annoy(int amt)
{
	m_hitpoints-=amt; 
	
}

bool LivingObject::canMoveThrough(int x, int y)
{
	int boulderX, boulderY;; 
	vector <Boulder*> ::iterator it;
	it = getWorld()->getBoulder().begin(); 
	while (it != getWorld()->getBoulder().end())
	{
		if (*it != nullptr)
		{
			boulderX = (*it)->getX();
			boulderY = (*it)->getY();
			if ((boulderX - x)*(boulderX - x) + (boulderY - y)*(boulderY - y) <= 9)
				return false;
		}

				it++;
	}
	return true; 
	
}
bool LivingObject::isProtesterblocking(int x, int y)
{
	vector <Protester*> ::iterator it; 
	it = getWorld()->getProtester().begin(); 
	while (it != getWorld()->getProtester().end())
	{
		if (*it != nullptr)
		{
			int px = getX(); 
			int py = getY(); 
			if ((x-px)*(x-px) + (y -py)*(y-py) <= 9 ) 
			{
				return true;
			}
		}
		it++; 
	}
	return false;
}
// ------------------------------------------------------------------------------------
// FRACKMAN CLASS  ////////
//.....................................................................................

FrackMan::FrackMan(StudentWorld * world) :LivingObject(world, 30, 60, right, IID_PLAYER)
{
	setHitPoints(10); 
	 m_water = 5;
	 m_sonar = 1;
	 m_gold = 0;
}

FrackMan:: ~FrackMan()
{
	;
}

bool FrackMan::removeDirt(int cx, int cy)
{
	return getWorld()->clearDirt(cx, cy);  
}

bool FrackMan::isPlayer(int x, int y)
{
	for (int i = 0; i < 4; i++)
		for (int k = 0; k < 4; k++)
			if (x + i == getX() && y + k == getY())
				return true; 
	return false;
}

// .................................
// Accessors
//..................................
int FrackMan:: getSonar() const
{
	return m_sonar;
}

int FrackMan::getWater() const
{
	return m_water;
}
int FrackMan::getGold() const
{
	return m_gold;
}


// ................................
// Modifiers 
//..................................

void FrackMan::changeWater(int value)
{
	m_water += value;
}
void FrackMan::changeSonar(int value)
{
	m_sonar+= value;
}
void FrackMan::changeGold(int value)
{
	m_gold+= value;
}

//..................................

void FrackMan::movePlayer(int ch, int cx, int cy)
{
	switch (ch)
	{
	case KEY_PRESS_LEFT:
		if (!canMoveThrough(cx - 1, cy))
		{
			if (getDirection() != left)
				setDirection(left);
			else
				return; 
		}
		else
		{
			if (this->getDirection() == left)
			{

				if (cx == 0)
					this->moveTo(cx, cy);
				else
					this->moveTo(cx - 1, cy);
			}
			else
				this->setDirection(left);
		}

		break;
	case KEY_PRESS_RIGHT:
		if (!canMoveThrough(cx + 1, cy))
		{
			if (getDirection() != right)
				setDirection(right);
			else
				return;
		}
		else
		{
			if (this->getDirection() == right)
			{
				if (cx == 61)
					this->moveTo(cx, cy);
				else
					this->moveTo(cx + 1, cy);
			}
			else
			{
				this->setDirection(right);
			}
			
		}
		break;
	case KEY_PRESS_UP:
		if (!canMoveThrough(cx, cy+1))
		{
			if (getDirection() != up)
				setDirection(up);
			else
				return;
		}
		else 
		{
			if (this->getDirection() == up)
			{
				if (cy >= 60)
					this->moveTo(cx, cy);
				else
					this->moveTo(cx, cy + 1);
			}

			else
			{
				this->setDirection(up);
			}
		}
		break;
	case KEY_PRESS_DOWN:
		if (!canMoveThrough(cx, cy - 1))
		{
			if (getDirection() != down)
				setDirection(down);
			else
				return;
		}
		else 
		{
			if (this->getDirection() == down)
			{
				if (cy <= 0)
					this->moveTo(cx, cy);
				else
					this->moveTo(cx, cy - 1);
			}

			else
			{
				this->setDirection(down);
			}
		}
		break;
	case KEY_PRESS_SPACE:
	{
		if (getWater() != 0)
		{
			getWorld()->playSound(SOUND_PLAYER_SQUIRT);
			getWorld()->squirt();
			m_water--;
		}

		break;
	}

	case	'z' :
	case	'Z':

		if (m_sonar <= 0)
			return;
		else
		{
			m_sonar--; 
			setsVisibleWithinRadius();
		}
		break;

	case KEY_PRESS_TAB:

		if (m_gold <= 0)
			return;
		else
		{
			m_gold--; 
			getWorld()->bribe(); 
		}
	default:
		return;
	}

}

void FrackMan::doSomething()
{
	if (getHealth() <= 0)
		setDead();

	if (!this->isAlive())
		return; 
	int cx, cy;
	cx = this->getX();
	cy = this->getY();
	int ch = 0;
	if (getWorld()->getKey(ch)== true)
	{
		if(removeDirt(cx, cy))
			getWorld()->playSound(SOUND_DIG);
		movePlayer(ch, cx, cy);
	}
	

}



void FrackMan::addGold()
{
	m_gold++; 
}

void FrackMan::addSonar()
{
	m_sonar++;
}

void FrackMan::addWater()
{
	m_water += 5;
}

void FrackMan::setsVisibleWithinRadius()
{
	int cx = getX(); 
	int cy = getY(); 
	vector<Actor*> ::iterator it;
	it = getWorld()->getGoodies().begin(); 
	while (it != getWorld()->getGoodies().end())
	{
		if((*it)!= NULL)
		if (isWithinRadius((*it)->getX(), (*it)->getY(), 12))
		{
			if ((*it)->getID() == IID_BARREL || (*it)->getID() == IID_GOLD)
				(*it)->setVisible(true);
		}
		it++;

	}
	return;

}
////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------------------------------
// Protester CLASS  ////////
//.....................................................................................
//////////////////////////////////////////////////////////////////////////////////////////

Protester :: Protester (StudentWorld* world, int imageID) : LivingObject(world, 60,60 ,left, imageID)
{
	m_protesterState = 1 ; 
	m_protesterCounter = 15; 
	m_ticks = 0; 
	m_ticksToWait = max(0, 3 - int(getWorld()->getLevel()) / 4);
	m_turningCounter = 200; 
	numSquaresToMoveInCurrentDirection = rand() % 53 + 8;
}


int Protester::getState()
{
	return m_protesterState; 
}
void Protester::setState(int state)
{
	m_protesterState = state; 
}

int Protester::getTicksToWait() const
{
	return m_ticksToWait;
}
void Protester::setTicksToWait(int v)
{
	m_ticksToWait = v;
}
int Protester::getTicks()const
{
	return m_ticks; 
}
void Protester::addTicks()
{
	m_ticks++;
}
void Protester::resetTicks() {
	m_ticks = 0; 
}


int Protester::getCounter()
{
	return m_protesterCounter;
}
void Protester::addCounter()
{
	m_protesterCounter++;
}
void Protester::setCounter(int v)
{
	m_protesterCounter = v; 
}

void Protester:: setNumSquares(int v)
{
	numSquaresToMoveInCurrentDirection = v; 
}

int Protester::getNumSquares()
{
	return numSquaresToMoveInCurrentDirection;
}

void Protester:: setTurningCounter(int v)
{
	m_turningCounter = v;
}

int Protester::getTurningCounter() const
{
	return m_turningCounter; 
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//......................................................................................................
// REGULAR PROTESTER CLASS 
// ...................................................................................................
////////////////////////////////////////////////////////////////////////////////////////////////////////

RegularProtester::RegularProtester(StudentWorld* world) :Protester(world, IID_PROTESTER)
{
	setHitPoints(5);
}
bool RegularProtester::canMove(Direction dir)
{
	if (this == nullptr)
		return false;
	int cx = getX(); 
	int cy = getY();
	 
	switch (dir)
	{
	case up: 
		if (!canMoveThrough(cx, cy + 1) || (getWorld()->doesActorTouchDirt(cx, cy + 1)) || cy + 1 > 60 )
			return false; 
		break;
	case down:
		if (!canMoveThrough(cx, cy - 1) || (getWorld()->doesActorTouchDirt(cx, cy - 1)) || cy - 1 < 0)
			return false;
	case left:
		if (!canMoveThrough(cx - 1, cy) || (getWorld()->doesActorTouchDirt(cx - 1, cy)) || cx - 1 < 0)
			return false;
	case right:
		if (!canMoveThrough(cx + 1, cy) || (getWorld()->doesActorTouchDirt(cx + 1, cy)) || cx + 1 < 0)
			return false;
	default:
		break; 
	}
	return true; 
}
void RegularProtester::doSomething()
{
	
	if (!isAlive()) // if Protester is dead return immediately 
		return;
	
	if (getHealth() <= 0) // if Protester has less than zero hitpoints. set state to exit state. 
	{
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		setDead(); 
	}
	int cx = getX();
	int cy = getY(); 
	 


	if (getTicks() < getTicksToWait())
		setState(0); 
	else setState(1);

	if (getState() == 0) // if protester is in rest state then add ticks
	{
		addTicks(); 
		return; 
	}
	

	// REACHED EXIT POINT
	//if (getState() == 2) // protester is in exit state. 
	/*
	{
		setTicksToWait(0); 
		if (getX() == 60 && getY() == 60)
		{
			setDead();
			return; 
		}
		else
		{
			// Walk itself back to 60, 60.
			// moving one step and then return 
		}
	}
	*/

	
	// AFTER REACHING TICKS tO WAIT
	if (getTicks() >= getTicksToWait() && getState() == 1) // if the ticks equals to the ticks to wait
	{
		
		resetTicks();  // RESET TICKS 
		int turnDir = canProtesterTurn(); // TEST WHETHER PROTESTER IS AT AN INTERSECTION. 
		
		switch (turnDir)
		{
		case none:
			setTurningCounter(getTurningCounter() + 1); // INCREMENT TURNING COUNTER IF PROTESTER FAILS TO TURN.
			break;
		case up:
			setDirection(up);

			setNumSquares(rand() % 53 + 8);
			break;
		case down:
			setDirection(down);

			setNumSquares(rand() % 53 + 8);
			break;
		case right:
			setDirection(right);

			setNumSquares(rand() % 53 + 8);
			break;
		case left:
			setDirection(up);

			setNumSquares(rand() % 53 + 8);
			break;
		default:
			break;
		}

		// PLAYER IS WITHIN ATTACK RANGE !!!
		 // protester is not in exit state.
			if (getWorld()->getPlayer()->isWithinRadius(cx, cy, 4) && isFacingPlayer())
			{
				
				if (getCounter() >= 15)
				{
					getWorld()->playSound(SOUND_PROTESTER_YELL);
					setCounter(0);
					getWorld()->getPlayer()->annoy(2);
					return;
				}
				return; 
				addCounter();


			}

			// Protester Sees FrackMan. 
		if (clearSightOfPlayer() !=none)
		{
			
			setNumSquares(0);
				// MOVE TOWARDS FRACKMAN !!!!! 
			if (clearSightOfPlayer() == up && canMove(up) ) 
				moveProtester(cx, cy + 1, up);
			else if (clearSightOfPlayer() == down && canMove(down))
				moveProtester(cx, cy - 1, down);
			else if (clearSightOfPlayer() == left && canMove(left))
				moveProtester(cx - 1, cy, left);
			else if (clearSightOfPlayer() == right && canMove(right))
				moveProtester(cx + 1, cy, right);
		}
			
		
		

		

		if (getNumSquares() <= 0)
		{
			
			int newDir = pickNewDirection();
			setDirection(Direction(newDir));
			setNumSquares(rand() % 53 + 8);
		}
		Direction dir = getDirection();
		switch (dir)  // STEP 8 and STEP 9
		{
			case up:
			{

				if (canMoveThrough(cx, cy + 1) && !getWorld()->doesActorTouchDirt(cx, cy + 1) && cy + 1 <= 60 )
				{
					moveProtester(cx, cy + 1, dir);
					setNumSquares(getNumSquares() - 1);
					return;
				}
				else
					setNumSquares(0);
				return;
			}
			case down:
			{
				if (canMoveThrough(cx, cy - 1) && !getWorld()->doesActorTouchDirt(cx, cy - 1) && cy -1 >= 0)
				{
					moveProtester(cx, cy - 1, dir);
					setNumSquares(getNumSquares() - 1);
					return;
				}
				else
					setNumSquares(0);
				return;
			}
			case right:
			{
				if (canMoveThrough(cx + 1, cy) && !getWorld()->doesActorTouchDirt(cx + 1, cy) && cx + 1 <= 61)
				{
					moveProtester(cx + 1, cy, dir);
					setNumSquares(getNumSquares() - 1);
					return;
				}
				else
					setNumSquares(0);
				return;
			}
			case left:
			{
				
				if (canMoveThrough(cx - 1, cy) && !getWorld()->doesActorTouchDirt(cx - 1, cy) && cx -1 >= 0)
				{
		 
					moveProtester(cx - 1, cy, left);
					setNumSquares(getNumSquares() - 1);
					//return;
				}
				else
					setNumSquares(0);
				return;
			}
			default:
				break;
		}

	}

	
}

int RegularProtester :: pickNewDirection()
{
	if (this == nullptr)
		return none; 
	int cx = getX(); 
	int cy = getY(); 
	int newDir = rand() % 4 + 1; 
	while (true)
	{
		if (newDir == up && canMoveThrough(cx, cy + 1) && !(getWorld()->doesActorTouchDirt(cx, cy + 1)) && cy + 1 <=60)
			break;
		else if (newDir == down && canMoveThrough(cx, cy - 1) && !(getWorld()->doesActorTouchDirt(cx, cy - 1) ) && cy -1 >=0)
			break;
		else if (newDir == right && canMoveThrough(cx + 1, cy) && !(getWorld()->doesActorTouchDirt(cx + 1, cy)) && cx <= 60 )
			break;
		else if (newDir == left && canMoveThrough(cx - 1, cy) && !(getWorld()->doesActorTouchDirt(cx - 1, cy)) && cx >= 0)
			break;
		else
			newDir = rand() % 5 + 1; 
	}
	return newDir;
}

void RegularProtester::moveProtester(int x, int y, Direction dir)
{
	if (getDirection() != dir)
		setDirection(dir);
	if(canMoveThrough(x,y) && !getWorld()->doesActorTouchDirt(x,y))
		moveTo(x, y); 
	return; 
}


int RegularProtester::clearSightOfPlayer()
{
	int protesterX = getX(); 
	int protesterY = getY(); 
	int playerX = getPlayerX();
	int playerY = getPlayerY(); 
	//definitely a problem here. Check back later.  
	bool clearPath = true; 
	//check left
	for (int i = 0; i < 4; i++)
		for (int k = protesterX; k >= 0; k--)
		{
			if (getWorld()->isDirt(k, protesterY + i) || getWorld()->isBoulder(k, protesterY + i))
			{
				return none;
			}
			if (!getWorld()->isDirt(k, protesterY + i) && !getWorld()->isBoulder(k, protesterY + i))
			{

				if (getWorld()->isFrackMan(k, protesterY + i))
				{
					return left;
				}
				break;
			}
			else
				clearPath = false;

		}
	//check up:

		for (int i = 0; i < 4; i++)
			for (int k = protesterY; k < 63; k++)
			{
				if (!clearPath)
				{
					return none;
				}
				if (!getWorld()->isDirt(protesterX + i, k) && !getWorld()->isBoulder(protesterX + i, k))
				{
					
					if (getWorld()->isFrackMan(protesterX + i, k))
					{
						return up;
					}
				}
				else 
					clearPath = false;
				break; 
			}
	//check down:
		for (int i = 0; i < 4; i++)
			for (int k = protesterY; k >= 0; k--)
			{
				if (!clearPath)
				{
					return none;
				}
				if (!getWorld()->isDirt(protesterX + i, k) && !getWorld()->isBoulder(protesterX + i, k))
				{
					
					if (getWorld()->isFrackMan(protesterX + i, k))
					{
						return down;
					}

				}
				else
					clearPath = false;
				break; 
			}
	// check right:
		for (int i = 0; i < 4; i++)
			for (int k = protesterX; k <=63 ; k++)
			{
				if (!clearPath)
				{
					
					return none;
				}
				if (!getWorld()->isDirt(k, protesterY + i) && !getWorld()->isBoulder(k, protesterY + i))

				{
					
					if (getWorld()->isFrackMan(k, protesterY + i))
					{
						
						return right;
					}
				}
				else 
					clearPath = false;
				
			}
	// check left
		
		return none; 
	}
bool RegularProtester::isFacingPlayer()
{
	Direction dir = getDirection(); 
	if (dir == clearSightOfPlayer())
	{
		return true;
	}
	else return false;
}
int RegularProtester::canProtesterTurn() // returns a direction if protester can turn // Step 7
{
	if (this == nullptr)
		return none;
	if (getTurningCounter() < 200)
		return none; 
	
	int cx = getX(); 
	int cy = getY();
	
	Direction dir = getDirection(); 
	switch (dir)
	{
	case down:
	case up:
		
		if (canMoveThrough(cx - 1, cy) && !getWorld()->doesActorTouchDirt(cx - 1, cy) && canMoveThrough(cx + 1, cy) && !getWorld()->doesActorTouchDirt(cx + 1, cy)) // can turn left and right. 
		{
			int randDir = rand() % 2 + 3; 
			setTurningCounter(0);
			return randDir;
		}
		else if (canMoveThrough(cx - 1, cy) && !getWorld()->doesActorTouchDirt(cx - 1, cy) && !canMoveThrough(cx + 1, cy) && getWorld()->doesActorTouchDirt(cx + 1, cy))
		{
			setTurningCounter(0);
			return left;
		}
		else if (!canMoveThrough(cx - 1, cy) && getWorld()->doesActorTouchDirt(cx - 1, cy) && canMoveThrough(cx + 1, cy) && !getWorld()->doesActorTouchDirt(cx + 1, cy))
		{
			setTurningCounter(0);
			return right;
		}
		else
			return none; 
		break;
	case left:
	case right:

		if (canMoveThrough(cx, cy + 1) && !getWorld()->doesActorTouchDirt(cx , cy + 1) && canMoveThrough(cx, cy - 1) && !getWorld()->doesActorTouchDirt(cx, cy -1 )) // can turn left and right. 
		{
			int randDir = rand() % 2 + 1;	
			setTurningCounter(0);
			return randDir;
		}
		else if (canMoveThrough(cx, cy + 1) && !getWorld()->doesActorTouchDirt(cx, cy + 1) && !canMoveThrough(cx, cy -1 ) && getWorld()->doesActorTouchDirt(cx, cy -1))
		{
			setTurningCounter(0);
			return up;
		}
		else if (!canMoveThrough(cx, cy + 1) && getWorld()->doesActorTouchDirt(cx, cy + 1) && canMoveThrough(cx, cy -1) && !getWorld()->doesActorTouchDirt(cx , cy -1 ))
		{
			setTurningCounter(0);
			return down;
		}
		break;
		
	default:
		break;
	}
	return none; 
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ---------------------------------------------------------------------------------------------------------------
// DIRT CLASS 
//..............................................................................................................
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Dirt:: Dirt(int x, int y, StudentWorld* world):Actor(IID_DIRT, x , y, right, world, 0.25, 3)
{
	;
}

Dirt :: ~Dirt()
{
	;
}

void Dirt::doSomething()
{
	return;
}

// ..........................................................
// Boulder Class

Boulder:: Boulder(int x, int y, StudentWorld* world) : Actor(IID_BOULDER, x, y, down, world, 1.0, 1)
{
	m_state = STABLE; 
	getWorld()->clearDirt(x, y);
	m_counter = 0; 
}


Boulder :: ~ Boulder()
{}

int Boulder::getState() 
{
	return m_state; 
}

void Boulder::addCounter()
{
	m_counter++; 
}


void Boulder::doSomething()
{
	if (!isAlive())
		return;

	int cx = getX();
	int cy = getY();

	
	bool willfall = true;
	if (m_state == STABLE)
		for (int i = 0; i < 4; i++)
			if (getWorld()->isDirt(cx + i,cy -1))
				willfall = false; 
	
	if (willfall == true)
	{
		m_state = WAITING; // change state of boulder to waiting state
		addCounter();
	}


	if (m_counter >= 30) // The Boulder starts falling
	{
		m_state = FALLING;

		if (getWorld()->getPlayer()->isWithinRadius(cx, cy, 3))  // The boulder hits FrackMan.  // check surrounding for states. 
		{
			getWorld()->getPlayer()->setDead();
			
			return;
		}

		vector <Protester*>::iterator it;
		it = (getWorld()->getProtester()).begin();
		while (it != getWorld()->getProtester().end())// implement the Protester getting bribed a.k.a in bribe state. 
		{
			if ((*it)->isWithinRadius(cx, cy, 3) && *it != nullptr ) 
			{

				getWorld()->increaseScore(500); 
				(*it)->setHitPoints(0); 
			}
			it++;

		}
		// TODO : The boulder hits a Protester

		bool hasDirt = false;				// Boulder crashes into dirt 
		for (int i = 0; i < 4; i++)		
			if (getWorld()->isDirt(cx + i, cy - 1)) // TODO : The boulder hits another boulder. 
				hasDirt = true;
		if (hasDirt ||  cy - 1 < 0)   // Boulder reaches bottom of the field or hits a boulder. 
		{
			setDead();
			return;
			
		}
		
		else
		{
			getWorld()->playSound(SOUND_FALLING_ROCK);
			moveTo(cx, cy - 1);				// Boulder has open path down.  // Boulder rolls down. 
			return;
		}
	}


	return;

}


//////////////////////////////////////////////////////////////////////////////////////
// .....................................................
// GOODIE CLASS //
//.......................................................
/////////////////////////////////////////////////////////////////////////////////////

Goodie::Goodie(int imageID, int startX, int startY, StudentWorld* world, Direction dir, int depth) : Actor(imageID, startX, startY, dir, world, 1.0, depth)
{
	setVisible(false);
	m_ticks = 0; 
	m_ticksToLive = 0;


}

void Goodie::setTicksToLive(int t)
{
	m_ticksToLive = t;
}

void Goodie::addTicks()
{
	m_ticks++;
}
int Goodie:: getTicks() const
{
	return m_ticks;
}
int Goodie::getTicksToLive() const
{
	return m_ticksToLive;
}
//////////////////////////////////////////////////////////////////////////////////////
// .....................................................
//OIL BARREL CLASS //
//.......................................................
/////////////////////////////////////////////////////////////////////////////////////

OilBarrel::OilBarrel(int startX, int startY, StudentWorld* world):Goodie(IID_BARREL, startX, startY, world, right,2)
{
	
}

void OilBarrel::doSomething()
{
	if (!isAlive())
		return; 
	if (getWorld()->getPlayer()->isWithinRadius(getX(), getY(), 4))
	{
		setVisible(true);

		if (getWorld()->getPlayer()->isWithinRadius(getX(), getY(), 3))
		{
			setDead();
			getWorld()->playSound(SOUND_FOUND_OIL);
			getWorld()->subtractOilBarrel();
			getWorld()->increaseScore(1000);
			return;
		}
	}

	
	return;
}

//////////////////////////////////////////////////////////////////////////////////////
// .....................................................
//	GoldNugget CLASS //
//.......................................................
/////////////////////////////////////////////////////////////////////////////////////

GoldNugget::GoldNugget(int startX, int startY, StudentWorld* world):Goodie(IID_GOLD, startX, startY, world, right,2)
{}

void GoldNugget::doSomething()
{
	if (!isAlive())
		return; 
	if (getWorld()->getPlayer()->isWithinRadius(getX(), getY(), 4))
	{
		setVisible(true);

		if (getWorld()->getPlayer()->isWithinRadius(getX(), getY(), 3))
		{
			setDead();
			getWorld()->playSound(SOUND_GOT_GOODIE);
			getWorld()->getPlayer()->addGold();
			getWorld()->increaseScore(10);
		}
		return; 
	}

	
}
//////////////////////////////////////////////////////////////////////////////////////
// .....................................................
//	BRIBE CLASS //
//.......................................................
/////////////////////////////////////////////////////////////////////////////////////

Bribe::Bribe(StudentWorld* world,int startX, int startY): Goodie (IID_GOLD, startX, startY, world, right, 2)
{
	setVisible(true);
	int T =  max(100, int(300 - (10 * getWorld()->getLevel())));
	setTicksToLive(T);
}

void Bribe::doSomething()
{
	if (!isAlive())
		return;
	int bx = getX();
	int by = getY(); 
	vector <Protester*>:: iterator it; 
	it = (getWorld()->getProtester()).begin();
	while (it != getWorld()->getProtester().end())// implement the Protester getting bribed a.k.a in bribe state. 
	{
		if ((*it)->isWithinRadius(bx, by,3) && *it != nullptr)
		{
			setDead();
			getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
			//(*it)->addGold();
			getWorld()->increaseScore(25);
			(*it)->setDead();

		}
		it++;
			
	}

	if (getTicks() == getTicksToLive())
	{
		setDead();
	} 

	addTicks(); 
}

//////////////////////////////////////////////////////////////////////////////////////
// .....................................................
//	SONARKIT CLASS //
//.......................................................
///////////////////////////////////////////////////////////////////////////////////////

SonarKit::SonarKit(int startX, int startY, StudentWorld* world):Goodie(IID_SONAR, startX, startY, world, right, 2)
{
	setVisible(true); 
	int T = max(100,int (300 - (10 * getWorld()->getLevel()	)	)	); 
	setTicksToLive(T);
}

void SonarKit::doSomething()
{
	if (!isAlive())
		return;
	if (getWorld()->getPlayer()->isWithinRadius(getX(), getY(), 3))
	{
		setVisible(true);
		setDead(); 
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->getPlayer()->addSonar();
		getWorld()->increaseScore(75); 
		// weird thing in spec, not sure what to do. 
	}

	if (getTicks() == getTicksToLive())
	{
		setDead();
	}
	
	addTicks(); 
}



////////////////////////////////////////////////////////////////////////////////////
// .....................................................
//	WATERPOOL CLASS //
//.......................................................
///////////////////////////////////////////////////////////////////////////////////////


WaterPool::WaterPool(int startX, int startY, StudentWorld* world) :Goodie(IID_WATER_POOL, startX, startY, world, right, 2)
{
	setVisible(true);
	unsigned int to = 300 - 10 * getWorld()->getLevel();

	// ticks the waterpool remains
	int t = max(100, int(to)); 
	setTicksToLive(t);
}

void WaterPool::doSomething()
{
	if (!isAlive())
		return;
	if (getWorld()->getPlayer()->isWithinRadius(getX(), getY(), 3))
	{
		setDead(); // 2.e
		getWorld()->playSound(SOUND_GOT_GOODIE); // 2.f
		getWorld()->getPlayer()->addWater();  // 2.g
		getWorld()->increaseScore(100); 
		return; 
	}

	if (getTicks() == getTicksToLive())
	{
		setDead();
		return; 
	}

	addTicks(); 

}


////////////////////////////////////////////////////////////////////////////////////
// .....................................................
//	SQUIRT CLASS //
//.......................................................
/////////////////////////////////////////////////////////////////////////////////////

Squirt::Squirt( StudentWorld* world, int cx, int cy, Direction dir ):Actor(IID_WATER_SPURT,cx , cy , dir , world, 1.0, 1)
{

	setVisible(true);
	travelDistance = 4;
}

void Squirt::doSomething()
{
	if (travelDistance <= 0)
		setDead();

	int bx = getX();
	int by = getY();

	// Check whether it hits a protester. 


	vector <Protester*>::iterator it;
	it = (getWorld()->getProtester()).begin();
	while (it != getWorld()->getProtester().end())
	{
		if ((*it)->isWithinRadius(bx, by, 3) && *it != nullptr )
		{
			getWorld()->playSound(SOUND_PROTESTER_ANNOYED); 
			if((*it)->getID() == IID_PROTESTER || (*it)->getID() == IID_HARD_CORE_PROTESTER)
				(*it)->annoy(2); 
			setDead();	
			if ((*it)->getHealth() <= 0)
				getWorld()->increaseScore(100); 
			return;
		}
		it++;
	}

	if (getWorld()->doesActorTouchDirt(bx, by))
	{
		setDead();
		return; 
	}
	// Check whether it hits a boulder or a protester. 


	if (getDirection() == right)
	{
		if (bx != 61)
		{
			moveTo(bx + 1, by);
			travelDistance--;
			return;
		}
		else
		{
			setDead();
			return;
		}
	}

	else if (getDirection() == left)
	{
		if (bx != 0)
		{
			moveTo(bx - 1, by);
			travelDistance--;
			return; 
		}
		else
		{
			setDead();
			return;
		}
	}

	else if (getDirection() == up)
	{
		if (!(by >= 60))
		{
			moveTo(bx, by + 1);
			travelDistance--;
			return;
		}
		else
		{
			setDead();
			return;
		}
	}

	else if (getDirection() == down)
	{
		if (!(by >= 60))
		{
			moveTo(bx, by - 1);
			travelDistance--;
			return;
		}
		else
		{
			setDead();
			return;
		}
	}

	
	// check whether it has exceeded four steps 
	
}

