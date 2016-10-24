#include "Actor.h"
#include "StudentWorld.h"

using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

// ================================ ACTOR =============================================================

StudentWorld* Actor::getWorld()
{
	return m_studentWorld;			// returns the current student world of all the actors
}

Actor::~Actor()
{	
	setVisible(false);				// when an actor is destroyed, they should not be visible anymore
}

void Actor::doSomething()			// empty doSomething to be implemented for other subclasses later
{

}
void Actor::addPoints(int n)
{
	getWorld()->increaseScore(n);			// increases the score of the game
}

int Actor::getType()
{
	return 1;								// gets the type (to be overridden just by the Oil class
}

bool Actor::getLeaveStatus()
{
	return false;						// sets the leave status of the protestors
}

void Actor::setLeaveStatus()
{
	
}

bool Actor::isDead()
{
	return m_isDead;					// checks if any actor is dead
}

void Actor::setDead()
{
	m_isDead = true;					// sets any actor to be dead
}

// ========================================== DIRT =====================================================

Dirt::Dirt(int startX, int startY, StudentWorld* stWorld) : Actor(IID_DIRT, startX, startY, stWorld, right, .25, 3)
{
			// nothign to be initialized
}

bool Dirt::canBeAnnoyed()
{
	return false;			// dirt cannot be annoyed silly
}

bool Dirt::canBePicked()
{
	return false;				// dirt cannot be picked
}

bool Dirt::canBlockPath()
{
	return true;				// dirt can block path though
}

void Dirt::doSomething()
{		
								// dirt does nothing, it just sits there
}

Dirt::~Dirt()
{
								// calls default destructor of graphobject
}

// ========================================= PERSON CLASS =================================================

Person::Person(int ImageId, int startX, int startY, StudentWorld* currWorld, Direction dir) : Actor(ImageId, startX, startY, currWorld, dir, 1, 0)
{
	setVisible(true);				// persons must start visible
}

void Person::setHealth(int health)
{
	m_health = health;						// persons must set their health to somethign
}

int Person::getHealth()
{
	return m_health;						// to get their health
}

void Person::reduceHealth(int health)
{
	m_health -= health;						// to reduce the health
}

bool Person::canBeAnnoyed()
{
	return true;					// all persons can be annoyed
}

bool Person::canBePicked()
{
	return false;					// persons cannot be picked up
}

bool Person::canBlockPath()
{
	return false;					// persons cannot block path
}

void Person::doSomething()
{
									// to be later initialized by other classes
}



// ========================================= FRACKMAN ====================================================

FrackMan:: ~FrackMan()
{}

void FrackMan::addGold()
{
	m_gold++;							// adding gold to his inventory
}

int FrackMan::getGold()
{
	return m_gold;					// get the number of golds that he has
}

void FrackMan::addSquirt()
{
	m_water+= 5;						// adding 5 squirts if he finds them
}

int FrackMan::getSquirt()
{
	return m_water;						// getting the number of squirts that he has
}

int FrackMan::getSonar()
{
	return m_sonarcharge;					// getting the number of sonars he has
}

void FrackMan::addSonar()
{
	m_sonarcharge+= 2;					// adding 2 sonars if he finds them
}

void FrackMan::annoy(int annoyance)
{
	reduceHealth(annoyance);							// reducing health and checking to see if dead
	if (getHealth() <= 0)
	{
		setDead();
		getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
	}
}

void FrackMan::doSomething()
{
	if (isDead())									// if the frackman is dead, then should just quit
		return;
	int ch = 0;
	bool dirtFound = false;								// bool for dirtfound to play the sound, and int for the key pressed
	bool isDirtFound = false;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			dirtFound = getWorld()->deleteDirt(getX() + i, getY() + j);			// deletes all the dirt in the 4 by 4 region occupied by the Frackman
			if (dirtFound)
				isDirtFound = true;								// if there was dirtFound, then it plays the sound
		}
	}
	if (isDirtFound)
		getWorld()->playSound(SOUND_DIG);
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_LEFT:										// moving left, but changes direction first if it hasn't done so already
			if (getDirection() != left)
			{
				setDirection(left);
				break;
			}
			if (getX() - 1 < 0)
			{
				moveTo(getX(), getY());
				break;
			}
			if (getWorld()->doSomethingWithActor(getX() - 1, getY(), FRACKMAN_TYPE))
				break;
			moveTo(getX() - 1, getY());
			break;
		
		case KEY_PRESS_RIGHT:										// moving right, but changes direction first if it hasn't done so already
			if (getDirection() != right)
			{
				setDirection(right);
				break;
			}
			if (getX() + 1 > 60)
			{
				moveTo(getX(), getY());
				break;
			}
			if (getWorld()->doSomethingWithActor(getX() + 1, getY(), FRACKMAN_TYPE))
				break;
			moveTo(getX() + 1, getY());
			break;
		
		case KEY_PRESS_UP:										// moving UP, but changes direction first if it hasn't done so already
			if (getDirection() != up)
			{
				setDirection(up);
				break;
			}
			if (getY() + 1 > 60)
			{
				moveTo(getX(), getY());
				break;
			}
			if (getWorld()->doSomethingWithActor(getX(), getY() + 1, FRACKMAN_TYPE))
				break;
			moveTo(getX(), getY() + 1);
			break;
		
		case KEY_PRESS_DOWN:										// moving down, but changes direction first if it hasn't done so already
			if (getDirection() != down)
			{
				setDirection(down);
				break;
			}
			if (getY() - 1 < 0)
			{
				moveTo(getX(), getY());
				break;
			}
			if (getWorld()->doSomethingWithActor(getX(), getY()-1, FRACKMAN_TYPE))
				break;
			moveTo(getX(), getY() - 1);
			break;
		case KEY_PRESS_SPACE:
			if (m_water == 0)																// makes water, but only if he has some in his inventory
				break;
			else
			{
				m_water--;
				getWorld()->playSound(SOUND_PLAYER_SQUIRT);
				if (getDirection() == up)
				{
					getWorld()->makeASquirt(getX(), getY() + 4, up);						// asks studentworld to make the squirt, but tells it the correct direction and location
				}
				else if (getDirection() == down)
				{
					getWorld()->makeASquirt(getX(), getY() - 4, down);
				}
				else if (getDirection() == left)
				{
					getWorld()->makeASquirt(getX() - 4, getY(), left);
				}
				else if (getDirection() == right)
				{
					getWorld()->makeASquirt(getX() + 4, getY(), right);
				}
			}
			break;
		case KEY_PRESS_ESCAPE:												// pre-maturely kills the frackman
			setDead();
			break;
		case 'z':
		case 'Z':
			if (m_sonarcharge == 0)														// if sonar charge exists, then studentworld will use it in the appropiate way
				break;
			else
			{
				m_sonarcharge--;
				getWorld()->playSound(SOUND_SONAR);
				getWorld()->doSomethingWithActor(getX(), getY(), SONAR_KIT);
				break;
			}
		case KEY_PRESS_TAB:
			if (m_gold == 0)
				break;
			else
			{
				m_gold--;
				getWorld()->makeAGold(getX(), getY());									// if he has a gold, then he can press Tab to drop it where he is 
			}
		default:
			break;
		}
	}
}

// ============================= GOODIES =========================================
void Goodies::makeVisible()
{
	setVisible(true);									// some goodies start visible, others invisible. So i kept the visible
}

Goodies::~Goodies()
{

}

bool Goodies::canBeAnnoyed()
{
	return false;											// goodies cannot be annoyed
}

bool Goodies::canBePicked()
{
	return true;										// goodies can be picked up though
}

bool Goodies::canBlockPath()
{
	return false;										// goodies cannot block the path
}

void Goodies::doSomething()
{
											// left empty for other subclasses to change
}

// ============================  OIL ==============================================

Oil::Oil(int startX, int startY, StudentWorld* currworld) : Goodies(IID_BARREL, startX, startY, currworld)
{
	m_isVisible = false;										// oil is invisible by default
}

int Oil::getType()
{
	return OIL_BARREL;							// returns oilType to see how many oil barrels are left in the mine 
}

void Oil::doSomething()
{
	if (isDead())
		return;
	int frackDistX, frackDistY;
	getWorld()->positionOfFrack(frackDistX, frackDistY);
	double distanceBetween = getWorld()->distance(frackDistX, getX(), getY(), frackDistY);				// checks the distance between it and the frackman, if less than 4 makes visible
	if (m_isVisible == false && distanceBetween <= 4)
	{
		makeVisible();
	}
	if (distanceBetween <= 3)																			// if less than 3, kills it self, and adds the points
	{
		setDead();
		getWorld()->playSound(SOUND_FOUND_OIL);
		addPoints(1000);
	}
}



// ===================================== GOLD =====================================

Gold::Gold(int startX, int startY, StudentWorld* currWorld, bool canProtPick) : Goodies(IID_GOLD, startX, startY, currWorld)
{
	m_isVisible = false;													// gold has two possible states, temp and perm
	if (canProtPick == true)
	{
		makeVisible();
		m_isVisible = true;													// temp state means that the prot can pick up, so only 100 ticks left
		m_tempState = true;
		m_tickTime = 100;
	}
	else
		m_tempState = false;
}

void Gold::doSomething()
{
	if (isDead())
		return;
	int frackDistX, frackDistY;
	getWorld()->positionOfFrack(frackDistX, frackDistY);
	double distanceBetween = getWorld()->distance(frackDistX, getX(), getY(), frackDistY);
	if (!m_tempState)
	{
		if (m_isVisible == false && distanceBetween <= 4)				// only if not in a temp state, does it check with the frackman location (and interacts similar to OIL), but different sound
		{
			makeVisible();
		}
		if (distanceBetween <= 3)
		{
			setDead();
			getWorld()->playSound(SOUND_GOT_GOODIE);
			addPoints(10);
			getWorld()->interactWithFrack(getX(), getY(), GOLD);
		}
	}
	else
	{
		if(getWorld()->doSomethingWithActor(getX(), getY(), GOLD))					// otherwise it interacts with protestors if the gold is in a temporary state
		{
			setDead();
			getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
		}
		else
		{
			m_tickTime--;								// the gold dies by itself if its in a temp state and if a protestor does not find it in time
			if (m_tickTime == 0)
				setDead();
		}
	}
}


// ====================================== SONAR KIT =================================

Sonar::Sonar(int startX, int startY, StudentWorld* currWorld, int ticks) : Goodies(IID_SONAR, startX, startY, currWorld)
{
	m_tickTime = ticks;						// has an initial amount of ticks, but starts off visible (unlike gold or oil)
	makeVisible();
}

void Sonar::doSomething()
{
	if (isDead())
		return;
	int frackDistX, frackDistY;
	getWorld()->positionOfFrack(frackDistX, frackDistY);
	double distanceBetween = getWorld()->distance(frackDistX, getX(), getY(), frackDistY);
	if (distanceBetween <= 3)															// checks the distance with frackman, if less than 3, sets dead
	{
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->interactWithFrack(getX(), getY(), SONAR_KIT);
		addPoints(75);
	}
	else
	{
		m_tickTime--;																// otherwise it reduces the amount of time it is alive and eventually sets itself to dead
		if (m_tickTime == 0)
			setDead();
	}
}


// =================================== WATER POOL =================================

WaterPool::WaterPool(int startX, int startY, StudentWorld* currWorld, int ticks) : Goodies(IID_WATER_POOL, startX, startY, currWorld)
{
	m_tickTime = ticks;
	makeVisible();
}

void WaterPool::doSomething()
{
	if (isDead())
		return;
	int frackDistX, frackDistY;
	getWorld()->positionOfFrack(frackDistX, frackDistY);
	double distanceBetween = getWorld()->distance(frackDistX, getX(), getY(), frackDistY);
	if (distanceBetween <= 3)																	// checks the distance with frackman, if less than 3, sets dead
	{
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->interactWithFrack(getX(), getY(), WATER_POOL);
		addPoints(100);
	}
	else
	{
		m_tickTime--;																		// otherweise it reduces teh amount of time it is alive and dies
		if (m_tickTime == 0)																// similar to sonar kit, BUT it would be unwise to say that waterpool isA sonar kit (ie make the sonar kit the base class ... so similar code)
			setDead();
	}
}




// =========================================== BOULDER ===========================================


Boulder::Boulder(int startX, int startY, StudentWorld * currWorld) : Actor(IID_BOULDER, startX, startY, currWorld, down, 1, 1)
{
	m_stable = true;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			getWorld()->deleteDirt(getX() + i, getY() + j);												// deletes the dirt around it upon construction
	}
	m_falling = false;
	m_stable = true;														// it is stable when built and is not falling
}

void Boulder::doSomething()
{
	StudentWorld* currWorld = getWorld();
	if (isDead())
		return;
	if (m_stable)
	{
		int xval = getX();
		int yval = getY();
		if (currWorld->isThereDirt(xval, yval - 1) || currWorld->isThereDirt(xval + 1, yval - 1) || currWorld->isThereDirt(xval + 2, yval - 1) || currWorld->isThereDirt(xval + 3, yval - 1))	// checks the dirt underneath
			return;
		else 
		{
			m_stable = false;										// if no dirt underneath, then it should be unstable
			m_tickTime = 30;
			
			return;
		}
	}
	if (!m_falling)
	{
		m_tickTime -= 1;
		if (m_tickTime <= 0)
		{
			m_falling = true;
			currWorld->playSound(SOUND_FALLING_ROCK);										// after 30 ticks, goes from stable to falling
		}
	}
	if (m_falling)
	{
		int xval = getX();
		int yval = getY();
		currWorld->interactWithFrack(xval, yval, BOULDER);												// checks if frackman is underneath it

		bool killBoulder = currWorld-> doSomethingWithActor(xval,  yval, BOULDER);						// checks if other actors are around, including boulders
		if (yval <= 0 || killBoulder == true)
			setDead();
		if (currWorld->isThereDirt(xval, yval - 1) || currWorld->isThereDirt(xval + 1, yval - 1) || currWorld->isThereDirt(xval + 2, yval - 1) || currWorld->isThereDirt(xval + 3, yval - 1))
		{
			setDead();			// if dirt below it eventually, then dies
		}
		else
		{
			moveTo(xval, yval - 1);			// otherwise move it
		}		
	}
}

bool Boulder::canBeAnnoyed()
{
	return false;						// boulders cannot be annoyed
}	

bool Boulder::canBePicked()
{
	return false;									// boulders cannot be picked up
}

bool Boulder::canBlockPath()
{
	return true;								// boulders CAN block paths though
}





// ====================================== WATER SQUIRT CLASS ===================================



Squirt::Squirt(int startX, int startY, StudentWorld * currWorld, Direction dir) : Actor(IID_WATER_SPURT, startX, startY, currWorld, dir, 1, 1)
{
	m_maxMoves = 4;					// max moves for the water to move is 4
	m_dir = dir;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (currWorld->isThereDirt(getX() + i, getY() + j))					// if theres dirt in front of it or in the 4x4 place, then it uses the water, but is set to dead
			{
				setDead();
			}
		}
	}
	if (currWorld->doSomethingWithActor(getX(), getY(), REG_PROTESTOR))
		setDead();
}


void Squirt::doSomething()
{
	StudentWorld* currWorld = getWorld();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (currWorld->isThereDirt(getX() + i, getY() + j))					// if theres dirt in front of it or in the 4x4 place, then it uses the water, but is set to dead
			{
				setDead();
				return;
			}
		}
	}
	if (m_maxMoves <= 0)				// if all the moves are done, sets itself to dead
	{
		setDead();
		return;
	}

	if (currWorld->doSomethingWithActor(getX(), getY(), SQUIRT))					// oterwise it interacts with other actors
	{
		setDead();
		return;
	}

	if (m_dir == right)												// othterwise it moves itself (and checks the bounds so it doesnt move out of the mine)
	{
		if (getX() + 1 > 60)
		{
			setDead();
		}
		else
			moveTo(getX() + 1, getY());
	}
	else if (m_dir == left)
	{
		if (getX() - 1 < 0)
		{
			setDead();
		}
		else 
			moveTo(getX() - 1, getY());
	}
	else if (m_dir == up)
	{
		if (getY() + 1 > 60)
		{
			setDead();
		}
		moveTo(getX(), getY() + 1);
	}
	else if (m_dir == down)
	{
		if (getY() - 1 < 0)
		{
			setDead();
		}
		moveTo(getX(), getY() - 1);
	}
	m_maxMoves--;
}

bool Squirt::canBeAnnoyed()
{
	return false;						// squirts cannot be annoyed
}

bool Squirt::canBePicked()
{
	return false;							// squirts cannot be picked up
}

bool Squirt::canBlockPath()
{
	return false;						// squirts cannot block path
}



// ====================================== PROTESTOR CLASS ===========================================================

Protestor::Protestor(int id, StudentWorld * currWorld, int currLevel, int health): Person (id, 60, 60, currWorld, left)
{
	m_level = currLevel;
	m_leaveState = false;
	m_spacesToMove = rand() % 53 + 8;
	setHealth(health);															// initializing all the member variables for the protestor class
	m_currentTick = 1;
	m_nonRestingTickCount = 15;
	m_perpendicularTickCount = 0;
	m_restingTickCount = 0;
	m_ticksToWait = max(0, (3 - getCurrLevel() / 4)) + 1;
	m_restingState = false;
}

bool Protestor::getLeaveStatus()
{
	return m_leaveState;													// get the leaving status for
}

void Protestor::setLeaveStatus()
{
	m_leaveState = true;							// returns the leave status of the protestor
}

int Protestor::getCurrLevel()
{
	return m_level;						// gets the current level that the protestor is on
}

void Protestor::doSomething()
{
	StudentWorld* currWorld = getWorld();
	if (isDead())
	{
		return;
	}
	else if (m_restingState == true)									// checks if the prot is resting
	{
		m_restingTickCount--;
		m_nonRestingTickCount++;
		if(m_restingTickCount <= 0)
			m_restingState = false;
	}
	else if (m_currentTick % m_ticksToWait == 0)				// if not resting, then it tries to move
	{
		m_nonRestingTickCount++;
		m_perpendicularTickCount++;
		if (getLeaveStatus() == true)										
		{
			if (getX() == 60 && getY() == 60)											// if dead, then calls student world queue based algorithm for the optimal path back
				setDead();
			else
			{
				Direction dirtoMove = currWorld->getProtDirToMove(getX(), getY());
				switch (dirtoMove)
				{
					case right:
						setDirection(right);
						moveTo(getX() + 1, getY()); 
						break;
					case left:
						setDirection(left);
						moveTo(getX() - 1, getY());
						break;
					case up:
						setDirection(up);
						moveTo(getX(), getY() + 1);
						break;
					case down:
						setDirection(down);
						moveTo(getX(), getY() - 1);
						break;
					default:
						break;
				}
				m_currentTick++;
				return;
			}
			m_currentTick++;
		}
		int frackX, frackY;
		currWorld->positionOfFrack(frackX, frackY);
		if (currWorld->distance(frackX, getX(), frackY, getY()) <= 4)								// otherwise looks if the frackman is within 4 away
		{
			Direction currDirr = getDirection();
			if (frackX - getX() > 0 && currDirr == right && m_nonRestingTickCount > 15)
			{
				currWorld->interactWithFrack(getX(), getY(), REG_PROTESTOR);
				m_restingState = true;
				m_restingTickCount = 15 * m_ticksToWait;
				m_nonRestingTickCount = 0;
				return;
			}
			else if (frackX - getX() < 0 && currDirr == left && m_nonRestingTickCount > 15)
			{
				currWorld->interactWithFrack(getX(), getY(), REG_PROTESTOR);
				m_restingState = true;
				m_restingTickCount = 15 * m_ticksToWait;
				m_nonRestingTickCount = 0;
				return;
			}
			else if (frackY - getY() > 0 && currDirr == up && m_nonRestingTickCount > 15)
			{
				currWorld->interactWithFrack(getX(), getY(), REG_PROTESTOR);
				m_restingState = true;
				m_restingTickCount = 15 * m_ticksToWait;
				m_nonRestingTickCount = 0;
				return;
			}
			else if (frackY - getY() < 0 && currDirr == down && m_nonRestingTickCount > 15)
			{
				currWorld->interactWithFrack(getX(), getY(), REG_PROTESTOR);
				m_restingState = true;
				m_restingTickCount = 15 * m_ticksToWait;
				m_nonRestingTickCount = 0;
				return;
			}
		}
		else if (checkHardCore())																	// otherwise checks for hardcore (but does nothing in protestor)
		{
			Direction dirtoMove = currWorld->getHardCoreProtDir(getX(), getY());				// gets the optimal path to the frackman and hunts him down
			switch (dirtoMove)
			{
			case right:
				setDirection(right);
				moveTo(getX() + 1, getY());
				break;
			case left:
				setDirection(left);
				moveTo(getX() - 1, getY());
				break;
			case up:
				setDirection(up);
				moveTo(getX(), getY() + 1);
				break;
			case down:
				setDirection(down);
				moveTo(getX(), getY() - 1);
				break;
			default:
				break;
			}
			m_currentTick++;
			return;
		}
		else if (inStraightLine(getX(), getY(), frackX, frackY))					// otherwise looks in a straight line and tries to move in that direction
		{
			if (frackY == getY())
			{
				if (frackX - getX() > 0)
				{
					setDirection(right);
					moveTo(getX() + 1, getY());
				}
				if (frackX - getX() < 0)
				{
					setDirection(left);
					moveTo(getX() - 1, getY());
				}
			}
			else if (frackX == getX())
			{
				if (frackY - getY() > 0)
				{
					setDirection(up);
					moveTo(getX(), getY() + 1);
				}
				if (frackY - getY() < 0)
				{
					setDirection(down);
					moveTo(getX(), getY() - 1);
				}
			}
			m_spacesToMove = 0;
			m_currentTick++;
			return;
		}
		m_spacesToMove--;						
		if (!inStraightLine(getX(), getY(), frackX, frackY))
		{
			if (m_spacesToMove <= 0)
			{
				Direction dir = Direction(rand() % 4 + 1);																	// otherwise chooses a random direction to move in
				bool foundDir = false;
				while (foundDir == false)
				{
					dir = Direction(rand() % 4 + 1);
					foundDir = currWorld->isDirGood(getX(), getY(), dir);
				}
				setDirection(dir);
				m_spacesToMove = rand() % 53 + 8;
			}
			else if (m_perpendicularTickCount >= 200)														// or if it comes to the perpendicular crossroad, then it tries to turn in that direction
			{
				bool upGood, downGood, leftGood, rightGood;
				upGood = downGood = leftGood = rightGood = false;
				if (getDirection() == right)
				{
					if (currWorld->isDirGood(getX(), getY(), up))
					{
						upGood = true;
					}
					if (currWorld->isDirGood(getX(), getY(), down))
					{
						downGood = true;
					}
				}
				if (getDirection() == left)
				{
					if (currWorld->isDirGood(getX(), getY(), up))
					{
						upGood = true;
					}
					if (currWorld->isDirGood(getX(), getY(), down))
					{
						downGood = true;
					}
				}
				if (getDirection() == up)
				{
					if (currWorld->isDirGood(getX(), getY(), left))
					{
						leftGood = true;
					}
					if (currWorld->isDirGood(getX(), getY(), right))
					{
						rightGood = true;
					}
				}
				if (getDirection() == down)
				{
					if (currWorld->isDirGood(getX(), getY(), left))
					{
						leftGood = true;
					}
					if (currWorld->isDirGood(getX(), getY(), right))
					{
						rightGood = true;
					}
				}
				if (getDirection() == up || getDirection() == down)
				{
					if (leftGood == true && rightGood == true)
					{
						int i = rand() % 2;
						if (i == 0)
							setDirection(left);
						else
							setDirection(right);
						m_spacesToMove = rand() % 53 + 8;
						m_perpendicularTickCount = 0;
					}
					else
					{
						if (leftGood)
						{
							setDirection(left);
							m_spacesToMove = rand() % 53 + 8;
							m_perpendicularTickCount = 0;
						}
						if (rightGood)
						{
							setDirection(right);
							m_spacesToMove = rand() % 53 + 8;
							m_perpendicularTickCount = 0;
						}

					}
				}
				if (getDirection() == left || getDirection() == right)
				{
					if (upGood == true && downGood == true)
					{
						int i = rand() % 2;
						if (i == 0)
							setDirection(down);
						else
							setDirection(up);
						m_spacesToMove = rand() % 53 + 8;
						m_perpendicularTickCount = 0;
					}
					else
					{
						if (upGood)
						{
							setDirection(up);
							m_spacesToMove = rand() % 53 + 8;
							m_perpendicularTickCount = 0;
						}
						if (downGood)
						{
							setDirection(down);
							m_spacesToMove = rand() % 53 + 8;
							m_perpendicularTickCount = 0;
						}
					}
				}
			}
			if (currWorld->isDirGood(getX(), getY(), getDirection()))									// or it just moves in its current direction
			{
				if (getDirection() == right)
				{
					if (!(getX() + 1 > 60))
						moveTo(getX() + 1, getY());
				}
				if (getDirection() == left)
				{
					if (!(getX() - 1 < 0))
						moveTo(getX() - 1, getY());
				}
				if (getDirection() == up)
				{
					if (!(getY() + 1 > 60))
						moveTo(getX(), getY() + 1);
				}
				if (getDirection() == down)
				{
					if (!(getY() - 1 < 0))
						moveTo(getX(), getY() - 1);
				}
			}
			else
			{
				m_spacesToMove = 0;										// or it tries to change its direction in the next tick
			}
		}
	}
	m_currentTick++;
}

void Protestor::annoy(int annoyance)
{
	reduceHealth(annoyance);
	int a = max(50, 100 - getCurrLevel() * 10);
	setRestingTickCount(a);																		// annoys the protestor by the noted amount
	setRestingState(true);
	if (getHealth() < 0)
	{
		setLeaveStatus();
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);					// if health is lower than zero, then makes the protestor give up and adds some points
		setSpacesToMove(0);
		addPoints(100);
		setRestingState(false);
	}
}

void Protestor::bribe()
{
	getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);						// regular protestor is bribed, adds points and makes him move
	addPoints(25);
	setLeaveStatus();
}

void Protestor::setSpacesToMove(int a)
{
	m_spacesToMove = a;						// sets spaces to move
}

void Protestor::setRestingTickCount(int a)
{
	m_restingTickCount = a;							// sets resting tick count
}

bool Protestor::inStraightLine(int xval, int yval, int frackX, int frackY)
{
	StudentWorld* currWorld = getWorld();
	if (xval != frackX && yval != frackY)							 
	{
		return false;
	}
	else if (xval == frackX)
	{
		if (frackY - yval > 0)
		{
			for (int i = yval+1; i < frackY; i++)
			{
				if (!currWorld->check4Dirts(xval, i) || currWorld->doSomethingWithActor(xval, i, REG_PROTESTOR))			// vertical line if frackman above protestor
					return false;
			}
		}
		else
		{
			for (int i = yval - 1; i > frackY; i--)
			{
				if (!currWorld->check4Dirts(xval, i) || currWorld->doSomethingWithActor(xval, i, REG_PROTESTOR))			// vertical line if frackman below protestor
					return false;
			}
		}
	}
	else if (yval == frackY)
	{
		if (frackX - xval > 0)
		{
			for (int i = xval + 1; i < frackX; i++)
			{
				if (!currWorld->check4Dirts(i, yval) || currWorld->doSomethingWithActor(i, yval, REG_PROTESTOR))				// horizontal line if frackman to right of protestor
					return false;
			}
		}
		else
		{
			for (int i = xval - 1; i > frackX; i--)
			{
				if (!currWorld->check4Dirts(i, yval) || currWorld->doSomethingWithActor(i, yval, REG_PROTESTOR))					// horizontal line if frackman is to the left of the protestor
					return false;
			}
		}
	}
	return true;
}

bool Protestor::getRestingState()
{
	return m_restingState;						// gets resting state
}

void Protestor::setRestingState(bool b)
{
	m_restingState = b;				// sets resting state
}

bool Protestor::checkHardCore()
{
	return false;				// returns false right now, but for hardcore, actually makes it try to go in the loop to seek out the frackman
}


// ============================ HARDCORE PROTESTOR CLASS ================================

HardCoreProtestor::HardCoreProtestor(int id, StudentWorld* currWorld, int currLevel) : Protestor(id, currWorld, currLevel, 20)
{

}

void HardCoreProtestor::bribe()
{
	getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
	addPoints(50);
	int a = max(50, 100 - getCurrLevel() * 10);								// bribing increases the points, and fixates the protestor, but does NOT make him go away
	setRestingTickCount(a);
	setRestingState(true);
	
}

void HardCoreProtestor::annoy(int annoyance)
{
	reduceHealth(annoyance);
	int a = max(50, 100 - getCurrLevel() * 10);									// slightly harder to annoy (more health, and more points if annoyed too)
	setRestingTickCount(a);
	setRestingState(true);
	if (getHealth() < 0)
	{
		setLeaveStatus();
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		setSpacesToMove(0);
		addPoints(250);
		setRestingState(false);
	}
}

void HardCoreProtestor::doSomething()
{
	Protestor::doSomething();							// hardcore protestor does all of the regular protestor's do something
}

bool HardCoreProtestor::checkHardCore()
{
	StudentWorld* currWorld = getWorld();
	int moves = 16 + getCurrLevel() * 2;	
	int frackX, frackY;
	currWorld->positionOfFrack(frackX, frackY);												// calls the student worlds queue based algorithm to see if with mMoves from the frackman to chase him
	currWorld->findHardCorePath(frackX, frackY);
	return currWorld->protMovesAway(getX(), getY(), moves);
}