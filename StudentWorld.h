#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <queue>
#include <iostream>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

//									NOTE To TAs:

//			The appropiate comments for each of the following methods and classes are in the .cpp files


class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);

	virtual int init();

	virtual int move();

	bool deleteDirt(int xVal, int yVal);

	virtual void cleanUp();
	
	double distance(int x1, int x2, int y1, int y2);

	bool isThereDirt(int x, int y);

	void makeASquirt(int x, int y, GraphObject::Direction dir);

	void makeAGold(int xval, int yval);

	void interactWithFrack(int xval, int yval, int actorCalling);

	bool doSomethingWithActor(int xval, int yval, int actorCalling);

	void positionOfFrack(int& xval, int& yval);

	bool check4Dirts(int x, int y);

	virtual ~StudentWorld();

	void findHardCorePath(int x, int y);

	GraphObject::Direction getProtDirToMove(int x, int y);

	GraphObject::Direction getHardCoreProtDir(int x, int y);

	bool protMovesAway(int getX, int getY, int mMoves);

	bool isDirGood(int getX, int getY, GraphObject::Direction dir);

private:
	FrackMan* m_frack;
	Dirt* m_dirtCounter[64][64];
	std::vector<Actor*> m_Actors;
	GraphObject::Direction m_shortestPath[64][64];
	int m_shortestLength[64][64];
	GraphObject::Direction m_HardCorePath[64][64];
	int m_ticksToSpawnProt;
	int m_maxTicks;
	int m_targetNumProtestors;
	int m_numProtestors;

	void setDisplayText();

	std::string textFormatter(int score, int lvl, int live, int hp, int squirts, int gold, int sonar, int oilLeft);

	void checkRandomizeLocation(int &x, int &y);

	void createRandomLocation(int &xval, int &yval);

	void findShortestPath(int x, int y);

	int numberOilBarrelsLeft();

	void makePoolLocation(int &xval, int &yval);

	void createRandomPoolLocation(int &xval, int& yval);

	struct Coord
	{
	public:
		Coord(int x_val, int y_val)
		{
			m_x = x_val;
			m_y = y_val;
		}
		int x()
		{
			return m_x;
		}
		int y()
		{
			return m_y;
		}
	private:
		int m_x;
		int m_y;
	};
};

#endif // STUDENTWORLD_H_
