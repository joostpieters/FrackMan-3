#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <cmath>
#include <algorithm>
#include <queue>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

//									NOTE To TAs:

//			The Appropiate Comments for each of the following methods and classes are in the .cpp files


const int BOULDER = 1;
const int SQUIRT = 2;
const int OIL_BARREL = 3;
const int GOLD = 4;
const int WATER_POOL = 5;
const int REG_PROTESTOR = 6;
const int HARDCORE_PROTESTOR = 7;
const int SONAR_KIT = 8;
const int FRACKMAN_TYPE = 9;

class StudentWorld;

class Actor : public GraphObject
{
	public: 
		Actor(int ImageId, int startX, int startY, StudentWorld* currWorld, Direction dir = right, double size = 1.0, unsigned int depth = 0) : GraphObject(ImageId, startX, startY, dir, size, depth)
		{
			m_studentWorld = currWorld;
			m_isDead = false;
			GraphObject::setVisible(true);
		}

		StudentWorld* getWorld();				

		virtual ~Actor();

		virtual void doSomething()=0;

		bool isDead();

		void setDead();

		void addPoints(int n);

		virtual bool canBeAnnoyed() = 0;

		virtual bool canBePicked() = 0;

		virtual bool canBlockPath() = 0;

		virtual void annoy(int annoyance) {}

		virtual void bribe() {}

		virtual int getType();

		virtual bool getLeaveStatus();

		virtual void setLeaveStatus();

	private:
		StudentWorld* m_studentWorld;
		bool m_isDead;

};



class Dirt : public Actor
{
public:
	Dirt(int startX, int startY, StudentWorld* stWorld);
	
	virtual bool canBeAnnoyed();

	virtual bool canBePicked();

	virtual bool canBlockPath();

	virtual void doSomething();

	virtual ~Dirt();

};




class Person : public Actor
{
public: 
	Person(int ImageId, int startX, int startY, StudentWorld* currWorld, Direction dir);

	void setHealth(int health);

	int getHealth();

	void reduceHealth(int health);

	virtual bool canBeAnnoyed();

	virtual bool canBePicked();

	virtual bool canBlockPath();

	virtual void doSomething();

	virtual ~Person()
	{}
private:
	int m_health;
};


class FrackMan : public Person
{
public:
	FrackMan(StudentWorld* stWorld) : Person(IID_PLAYER, 30, 60, stWorld, right)
	{
		setHealth(10);
		m_gold = 0;
		m_water = 5;
		m_sonarcharge = 1;
	}

	virtual ~FrackMan();

	void addGold();

	int getGold();

	void addSquirt();

	int getSquirt();

	int getSonar();

	void addSonar();

	virtual void annoy(int annoyance);

	virtual void doSomething();

private:
	int m_gold;
	int m_sonarcharge;
	int m_water;
};




class Protestor: public Person
{
public: 
	Protestor(int id, StudentWorld* currWorld, int currLevel, int health);
	
	bool getLeaveStatus();

	virtual void doSomething();

	virtual void annoy(int annoyance);

	virtual void bribe();

	void setRestingState(bool b);

	void setLeaveStatus();

	int getCurrLevel();

	void setRestingTickCount(int a);

	void setSpacesToMove(int a);

private: 
	int m_level;
	int m_spacesToMove;
	bool m_leaveState;
	int m_currentTick;
	int m_nonRestingTickCount;
	int m_perpendicularTickCount;
	int m_ticksToWait;
	bool m_restingState;
	int m_restingTickCount;
	
	virtual bool checkHardCore();

	bool inStraightLine(int xval, int yval, int frackX, int frackY);

	bool getRestingState();
};




class HardCoreProtestor :public Protestor
{
public:
	HardCoreProtestor(int id, StudentWorld* currWorld, int currLevel);

	virtual void bribe();

	virtual void annoy(int annoyance);

	virtual void doSomething();

private: 
	virtual bool checkHardCore();

};




class Goodies:public Actor
{
public:
	Goodies(int ImageId, int startX, int startY, StudentWorld* currworld) : Actor(ImageId, startX, startY, currworld, right, 1, 2)
	{
		setVisible(false);
	}

	void makeVisible();

	virtual ~Goodies();

	virtual bool canBeAnnoyed();

	virtual bool canBePicked();

	virtual bool canBlockPath();

	virtual void doSomething();
	
private:
};




class Oil: public Goodies
{
public:
	Oil(int startX, int startY, StudentWorld* currworld);

	int getType();

	virtual void doSomething();

private:
	bool m_isVisible;
};




class Gold: public Goodies
{
public: 
	Gold(int startX, int startY, StudentWorld* currWorld, bool canProtPick);

	virtual void doSomething();

private:
	bool m_tempState;
	bool m_isVisible;
	int m_tickTime;
};




class Sonar: public Goodies
{
public:
	Sonar(int startX, int startY, StudentWorld* currWorld, int ticks);

	virtual void doSomething();

private: 
	int m_tickTime;
};



class WaterPool: public Goodies
{
public:
	WaterPool(int startX, int startY, StudentWorld* currWorld, int ticks);

	virtual void doSomething();

private:
	int m_tickTime;
};



class Boulder: public Actor
{
public:
	Boulder(int startX, int startY, StudentWorld* currWorld); 

	virtual void doSomething();

	virtual bool canBeAnnoyed();

	virtual bool canBePicked();

	virtual bool canBlockPath();

private:
	bool m_stable;
	int m_tickTime;
	bool m_falling;
};



class Squirt: public Actor
{
public:
	Squirt(int startX, int startY, StudentWorld* currWorld, Direction dir); 

	virtual void doSomething();

	virtual bool canBeAnnoyed();

	virtual bool canBePicked();

	virtual bool canBlockPath();

private:
	int m_maxMoves;
	Direction m_dir;
};


#endif // ACTOR_H_
