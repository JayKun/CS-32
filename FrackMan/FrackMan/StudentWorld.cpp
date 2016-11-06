#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cmath>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir)
{

}

StudentWorld::~StudentWorld()
{

	delete m_player; //delete player

	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 60; y++)
		{
			if (m_dirt[x][y] != NULL)
			{
				delete m_dirt[x][y];    // delete Dirt
				m_dirt[x][y] = NULL;
			}
		}
	}



	vector<Boulder*>::iterator it;
	it = m_boulders.begin();
	while (it != m_boulders.end()) // destruct Boulders
	{
		if ((*it) != NULL)
		{
			delete *it;
			(*it) = NULL;
		}
		it++;
	}

	vector<Actor*> ::iterator itt;
	itt = m_goodies.begin();
	while (itt != m_goodies.end())
	{
		if ((*itt) != NULL)
		{
			delete *itt;
			(*itt) = NULL;
		}
		itt++;
	}
	vector<Protester*> ::iterator ittt;
	ittt = m_protesters.begin();
	while (ittt != m_protesters.end())
	{
		if ((*ittt) != NULL)
		{
			delete *ittt;
			(*ittt) = NULL;
		}
		ittt++;
	}

	m_boulders.clear();
	m_protesters.clear(); 
	m_goodies.clear();

}

int StudentWorld::init()
{
	TicksToAddProtester = max(25, 200 - int(getLevel()));
	nOilBarrels = min(int(2 + getLevel()), 20);
	int n_boulders = min(int(getLevel() / 2 + 2), 6);
	int n_gold = max(5 - int(getLevel()) / 2, 2);
	P = min(15, 2 + int(getLevel()* 1.5));
	m_player = new FrackMan(this);
	currentProtesterTicks = 0; 
	m_protesters.push_back(new RegularProtester(this)); 

	for (int x = 0; x < 64; x++)  // create Dirt
		for (int y = 0; y < 60; y++)
			m_dirt[x][y] = new Dirt(x, y, this);

	for (int i = 30; i <= 33; i++) // create Mineshaft 
		for (int k = 4; k <= 59; k++)
		{
			delete m_dirt[i][k];
			m_dirt[i][k] = NULL;
		}

	

	for (int i = 0; i < n_boulders; i++) // create Boulders
		{
			int x = randX();
			int y = randY();
			if (correctDistance(x,y,6)) // you deleted isBoulder
				m_boulders.push_back(new Boulder(x, y, this));
			else
			{
				i--;
				continue;
			}
		}


	for (int i = 0; i < nOilBarrels; i++) // create OilBarrels
	{
		int x = randX();
		int y = randY();
		if (correctDistance(x, y,6)) // you deleted isBoulder
			m_goodies.push_back(new OilBarrel(x, y, this));
		else
		{
			i--;
			continue;
		}
	}


	for (int i = 0; i < n_gold; i++) // create GoldNuggets
	{
		int x = randX();
		int y = randY();
		if (correctDistance(x, y,6))
			m_goodies.push_back(new GoldNugget(x, y, this));
		else
		{
			i--;
			continue;
		}
	}

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld:: move()
{
	setDisplayText();

	if (!(getPlayer()->isAlive()))
	{
		playSound(SOUND_PLAYER_GIVE_UP);
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	if (willAddGoodie())
		addGoodie(); 


	if (nOilBarrels == 0)  /// Check to See whether the Player has won the game. 
	{

		return GWSTATUS_FINISHED_LEVEL;
	}
	
	for (unsigned int i = 0; i < m_boulders.size(); i++) // boulders do something
		if(m_boulders[i] != nullptr)
			m_boulders[i]->doSomething(); 

	for (unsigned int i = 0; i < m_goodies.size(); i++) // goodies do something
		if (m_goodies[i] != nullptr)
			m_goodies[i]->doSomething();

	getPlayer()->doSomething(); // player do something
	
	if (TicksToAddProtester <= currentProtesterTicks)
	{
		if (m_protesters.size() < P)
		{
			m_protesters.push_back(new RegularProtester(this));
			currentProtesterTicks = 0;
		}
	}
	
	currentProtesterTicks++;

	vector<Protester*> ::iterator ittt;
	ittt = m_protesters.begin();
	while (ittt != m_protesters.end())
	{
		if ((*ittt) != NULL)
		{
			(*ittt)->doSomething();
			
		}
		ittt++;
	}

	for (unsigned int i = 0; i < m_boulders.size(); i++) // Remove dead characters
	{
		if (m_boulders[i] != NULL)
			if (!(m_boulders[i]->isAlive()) )
			{
				delete m_boulders[i];
				m_boulders[i] = NULL;
			}
	}
	for (unsigned int i = 0; i < m_protesters.size(); i++) // Remove dead characters
	{
		if (m_protesters[i] != NULL)
			if (!(m_protesters[i]->isAlive()))
			{
				playSound(SOUND_PROTESTER_GIVE_UP);
				delete m_protesters[i];
				m_protesters[i] = NULL;
			}
	}
	for (unsigned int i = 0; i < m_goodies.size(); i++) // Remove dead characters
	{
		if (m_goodies[i] != NULL)
			if (	(!m_goodies[i]->isAlive()) )
			{
				delete m_goodies[i];
				m_goodies[i] = NULL;
			}
	}
	return GWSTATUS_CONTINUE_GAME;

	
}

void StudentWorld::cleanUp()
{
	delete m_player; //delete player

	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 60; y++)
		{
			if (m_dirt[x][y] != NULL)
			{
				delete m_dirt[x][y];    // delete Dirt
				m_dirt[x][y] = NULL;
			}
		}
	}



	vector<Boulder*>::iterator it;
	it = m_boulders.begin();
	while (it != m_boulders.end()) // destruct Boulders
	{
		if ((*it) != NULL)
		{
			delete *it;
			(*it) = NULL;
		}
		it++;
	}

	vector<Actor*> ::iterator itt;
	itt = m_goodies.begin();
	while (itt != m_goodies.end())
	{
		if ((*itt) != NULL)
		{
			delete *itt;
			(*itt) = NULL;
		}
		itt++;
	}
	
	vector<Protester*> ::iterator ittt;
	ittt = m_protesters.begin();
	while (ittt != m_protesters.end())
	{
		if ((*ittt) != NULL)
		{
			delete *ittt;
			(*ittt) = NULL;
		}
		ittt++; 
	}
	

	m_boulders.clear();
	m_protesters.clear(); 
	m_goodies.clear();
}

Dirt*  StudentWorld:: getDirt(int x, int y)
{
	return m_dirt[x][y];
}


FrackMan* StudentWorld::getPlayer()
{
	return m_player;
}

bool StudentWorld::clearDirt(int x, int y)
{
	bool Dig = false;
	for (int i = 0; i < 4; i++)
		for (int k = 0; k < 4; k++)
		{
			if (x + i > 63 || y + k > 59)
				continue;
			if (m_dirt[x + i][y + k] != NULL)
			{
				delete m_dirt[x + i][y + k];
				m_dirt[x + i][y + k] = NULL;
				Dig = true;
			}
		}
	return Dig;

}


void StudentWorld::setDisplayText()
{
	int score = getScore();
	int level = getLevel();
	int lives = getLives();
	int health = m_player->getHealth();
	int water = m_player->getWater();
	int gold = m_player->getGold();
	int sonar = m_player->getSonar();

	// Next, create a string from your statistics, of the form: // ¡°Scr: 0321000 Lvl: 52 Lives: 3 Hlth: 80% Water: 20 Gld: 3 Sonar: 1 Oil Left: 2¡±
	string s = generateText(score, level,lives, health, water, gold, sonar);
	setGameStatText(s); 
}

string StudentWorld::generateText(int score, int level, int lives, int health, int water, int gold, int sonar)
{	
	
	std::stringstream str_score;
	str_score << std::setfill('0') << std::setw(6) << score;
	string s = "Scr: " + str_score.str() + "  Lvl: " + to_string(level) + "  Lives: " + to_string(lives) + "  Hlth: " + to_string(health/10 *100) + "%" + "  Water: " + to_string(water)
		+ "  Gld: " + to_string(gold) + "  Sonar: " + to_string(sonar) + "  Oil Left: " + to_string(nOilBarrels); 
	return s; 
}

int StudentWorld:: randX()
{
	int v = rand() % 61;
	while (v + 3 >= 30 && v <= 33)
	{
		v = rand() % 61;
	}
	return v;
}

int StudentWorld::randY()
{
	int v = rand() % 57;
	while (v < 20 || v > 56)
		v = rand() % 57; 
	return v;
}


bool StudentWorld::correctDistance(int x, int y, int r)
{
	vector<Actor*> ::iterator it;
	it = m_goodies.begin();
	while (it != m_goodies.end())
	{
		if ((*it) != nullptr)
		{
			int cx = (*it)->getX();
			int cy = (*it)->getY();
			if ((x - cx)*(x - cx) + (y - cy)*(y - cy) <= r*r)
				return false;
		}
		it++;
	}

	vector<Boulder*> ::iterator ib;
	ib = m_boulders.begin();
	while (ib != m_boulders.end())
	{
		if ((*ib) != nullptr)
		{
			int cx = (*ib)->getX();
			int cy = (*ib)->getY();
			if ((x - cx)*(x - cx) + (y - cy)*(y - cy) <= r*r)
				return false;
		}
		ib++;
	}
	return true;
}

bool StudentWorld::isDirt(int x, int y)
{
	if (x < 0 || x > 63 || y < 0 || y > 59 || m_dirt[x][y] == NULL)
		return false;

	return true;
}

bool StudentWorld::isGoodie(int x, int y) 
{
	vector<Actor*> ::iterator it;
	it = m_goodies.begin();
	while (it != m_goodies.end())
	{
		if (*it != NULL)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int k = 0; k < 4; k++)

					if (x == (*it)->getX() + i && (*it)->getY() + k == y && (*it)->getID() == IID_BARREL || (*it)->getID() == IID_GOLD
						|| (*it)->getID()== IID_SONAR || (*it)->getID() == IID_WATER_POOL)
						return true;
			}

		}
		it++;
	}
	return false;
	
}

bool StudentWorld::isBoulder(int x, int y)
{
	vector<Boulder*> ::iterator it;
	it = m_boulders.begin();
	while (it != m_boulders.end())
	{
		for (int i = 0; i < 4; i++)
		{
			for (int k = 0; k < 4; k++)
				if(*it!= NULL)
					if (x == (*it)->getX() + i && (*it)->getY() + k == y)
						return true;
		}


		it++;
	}
	return false;
}

bool StudentWorld::isProtester(int x, int y)
{
	vector<Protester*> ::iterator it;
	it = m_protesters.begin();
	while (it != m_protesters.end())
	{
		for (int i = 0; i < 4; i++)
		{
			for (int k = 0; k < 4; k++)
				if (x == (*it)->getX() + i && (*it)->getY() + k == y)
					return true;
		}


		it++;
	}
	return false;
}

bool StudentWorld::isFrackMan(int x, int y)
{
	for (int i = 0; i < 4; i++)
	{
		for (int k = 0; k < 4; k++)
			if (x == (m_player)->getX() + i && (m_player)->getY() + k == y)
				return true;
	}
	return false; 
}


bool StudentWorld::willAddGoodie()
{
	int G = getLevel() * 25 + 300; 
	int i = rand() % G + 1; 
	if (i == 1)
		return true;
	else
		return false; 
}

void StudentWorld::addGoodie()
{
	int n = rand() % 5 + 1;
	if (n == 1)
		addSonar();
	else
		addWaterPool(); 
	return; 
}

bool StudentWorld::doesActorTouchDirt(int x, int y)
{
	for (int i = 0; i < 4; i++)
		for (int k = 0; k < 4; k++)
			if (isDirt(x + i, y + k))
				return true; 
	return false;
}
void StudentWorld::addWaterPool()
{
	int x = rand() % 61; 
	int y = rand() % 61;

	while (doesActorTouchDirt(x, y) || !correctDistance(x, y, 3) )
	{
		x = rand() % 61;
		y = rand() % 61;
	}
		
	Goodie *ptr = new WaterPool(x, y, this);
	m_goodies.push_back(ptr);
	return;
		
		
	
}

void StudentWorld::addSonar()
{
	Goodie * ptr = new SonarKit(0, 60, this); 
	m_goodies.push_back(ptr);
}


void StudentWorld::squirt()
{


	if (getPlayer()->getDirection() == up) //TODO : CONDITIONS to prevent out of map
	{
		Actor *ptr = new Squirt(this, getPlayer()->getX(), getPlayer()->getY()+4, getPlayer()->getDirection());
		m_goodies.push_back(ptr);
	}
	else if (getPlayer()->getDirection() == down)
	{
		Actor *ptr = new Squirt(this, getPlayer()->getX(), getPlayer()->getY()-4, getPlayer()->getDirection());
		m_goodies.push_back(ptr);
	}
	else if (getPlayer()->getDirection() == 3)
	{
		Actor *ptr = new Squirt(this, getPlayer()->getX()-4, getPlayer()->getY(), getPlayer()->getDirection());
		m_goodies.push_back(ptr);
	}
	else if (getPlayer()->getDirection() == 4)
	{
		Actor *ptr = new Squirt(this, getPlayer()->getX()+4, getPlayer()->getY(), getPlayer()->getDirection());
		m_goodies.push_back(ptr);
	}

	
}

void StudentWorld::bribe()
{
	Actor *ptr = new Bribe(this, getPlayer()->getX(), getPlayer()->getY());
	m_goodies.push_back(ptr); 
}