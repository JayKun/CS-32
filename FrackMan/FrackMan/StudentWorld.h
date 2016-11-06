#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace std; 

enum Direction { none, up, down, left, right };

class StudentWorld : public GameWorld
{

public:

	StudentWorld(std::string assetDir);
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();

	// FrackMan
	FrackMan* getPlayer();

	//Dirt
	Dirt* getDirt(int x, int y);
	bool clearDirt(int x, int y);
	
	//Texts
	void setDisplayText();
	string generateText(int score, int level, int lives, int health, int water, int gold, int sonar);

	//Position of all the Objects
	bool isBoulder(int x, int y);
	bool isGoodie(int x, int y);
	bool isProtester(int x, int y);
	bool isFrackMan(int x, int y);
	bool isDirt(int x, int y);
	bool willAddGoodie(); 
	void addWaterPool(); 
	void addSonar();
	void addGoodie(); 
	vector<Boulder*> &getBoulder()
	{
		return m_boulders; 
	}
	vector <Protester*> &getProtester()
	{
		return m_protesters; 
	}

	void subtractOilBarrel()
	{
		nOilBarrels--; 
	}

	int getOilBarrel()
	{
		return nOilBarrels; 
	}

	vector <Actor*> &getGoodies()
	{
		return m_goodies; 
	}

	bool doesActorTouchDirt(int x, int y);
	void squirt(); 
	void bribe(); 
private:
	FrackMan* m_player;
	Dirt* m_dirt[64][60]; 
	vector <Boulder*> m_boulders; // Almost done but need to check implementation of doSomething(); // and not let FrackMan pass through
	vector <Protester*> m_protesters; // Not yet implemented yet
	vector <Actor*> m_goodies; 
	int nOilBarrels;
	int objectCounter;
	int randX();
	int randY();
	int TicksToAddProtester ;
	int currentProtesterTicks; 
	int P;
	bool correctDistance(int x, int y, int r); 
	

};

#endif // STUDENTWORLD_H_
