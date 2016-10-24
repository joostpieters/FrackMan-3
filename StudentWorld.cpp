#include "StudentWorld.h"
#include <string>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir)
{
	m_ticksToSpawnProt = 0;													// initialize several tick variables for the protestors
	m_maxTicks = 0;
	m_targetNumProtestors = 0;
	m_numProtestors = 0;
}

int StudentWorld::init()
{
	m_ticksToSpawnProt = 0;
	m_maxTicks = max(25, 200 -(int)(getLevel()));
	m_targetNumProtestors = min(15, 2 + (int)(getLevel()*1.5));
	m_numProtestors = 0;
	m_frack = new FrackMan(this);
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			if ((29 < j) && (j < 34) && (i > 3))					// middle mine shaft should NOT be covered with dirt
				m_dirtCounter[i][j] = nullptr;	
			else if (i == 60 || i == 61 || i == 62 || i == 63)		// top 4 rows should not be covered by the dirt
				m_dirtCounter[i][j] = nullptr;
			else
				m_dirtCounter[i][j] = new Dirt(j, i, this);			// rest of the field should be covered by the dirt
		}
	}

	int currLevel = getLevel();
	int B = min((currLevel / 2 + 2), 6);
	int G = max(5 - currLevel / 2, 2);
	int L = min(2 + currLevel, 20);

	for (int i = 0; i < B; i++)
	{																						// spawn boulders, then gold, then oils
		int x = 5;
		int y = 10;
		checkRandomizeLocation(x, y);											// calls the student world's randomize function which puts them atleast 6 away
		m_Actors.push_back(new Boulder(x, y, this));
	}

	for (int i = 0; i < G; i++)
	{
		int x = 5;
		int y = 10;
		checkRandomizeLocation(x, y);
		m_Actors.push_back(new Gold(x, y, this, false));
	}

	for (int i = 0; i < L; i++)
	{
		int x = 5;
		int y = 10;
		checkRandomizeLocation(x, y);
		m_Actors.push_back(new Oil(x, y, this));
	}
	int randT = min(90, (int)(getLevel() * 10) + 30);
	if (rand()%randT == 0)
		m_Actors.push_back(new HardCoreProtestor(IID_HARD_CORE_PROTESTER, this, (int)(getLevel())));		// puts a hardcore or regular protestor on the first tick of the round
	else
		m_Actors.push_back(new Protestor(IID_PROTESTER, this, (int)(getLevel()), 5));
	m_numProtestors++;
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::checkRandomizeLocation(int &x, int &y)
{
	if (m_Actors.empty())							// when creating the very first boulder, needs to not be in the mine shaft
	{
		int i = 0;
		while (i == 0)
		{
			int xval;
			int yval;
			createRandomLocation(xval, yval);
			if (isThereDirt(xval, yval))
			{
				if (xval > 26 && xval < 34)
				{
					continue;
				}
				x = xval;
				y = yval;
				return;
			}
		}
	}
	vector<Actor*>::iterator it;						// for creating the rest of the items
	it = m_Actors.begin();
	int xval = 5;
	int yval = 10;
	createRandomLocation(xval, yval);

	while (it != m_Actors.end())
	{
		if (isThereDirt(xval, yval))
		{
			if (xval > 26 && xval < 34)																	// keeps calling the random function if the items are tried to be placed in a bad location
			{
				it = m_Actors.begin();
				createRandomLocation(xval, yval);
			}
			else
			{
				double d = distance(xval, (*it)->getX(), yval, (*it)->getY());
				if (d > 6)
					it++;
				else
				{
					it = m_Actors.begin();
					createRandomLocation(xval, yval);
				}
			}
		}
		else 
		{
			createRandomLocation(xval, yval);
			it = m_Actors.begin();
		}
		
	}
	x = xval;
	y = yval;
}

void StudentWorld::createRandomLocation(int & xval, int & yval)
{
	xval = rand() % 61;													// making the random location
	yval = rand() % 37 + 20;
}

void StudentWorld::makeASquirt(int x, int y, GraphObject::Direction dir)
{
	m_Actors.push_back(new Squirt(x, y, this, dir));													// makes the squirt and pushes to the end of the array (the squirt will act on the same tick)
}

void StudentWorld::makeAGold(int xval, int yval)
{
	m_Actors.push_back(new Gold(xval, yval, this, true));													// makes a temp gold that acts on the same tick
}

void StudentWorld::interactWithFrack(int xval, int yval, int actorCalling)
{
	if (actorCalling == BOULDER)
	{
		if (distance(xval, m_frack->getX()+2, yval, m_frack->getY()+2) <= 3)									// various items interacting with just the frackman (boulder, then gold, etc ...)
		{
			m_frack->setDead();
			playSound(SOUND_PLAYER_GIVE_UP);		// boulder kills the frackman
		}
	}
	if (actorCalling == GOLD)
	{
		m_frack->addGold();
	}
	if (actorCalling == SONAR_KIT)
	{
		m_frack->addSonar();
	}
	if (actorCalling == WATER_POOL)
	{
		m_frack->addSquirt();
	}
	if (actorCalling == REG_PROTESTOR)
	{
		m_frack->annoy(2);
		playSound(SOUND_PROTESTER_YELL);				// protestors annoy the frackman
	}
}

bool StudentWorld::doSomethingWithActor(int xval, int yval, int actorCalling)
{
	if (actorCalling == SQUIRT)
	{
		vector<Actor*>::iterator it;
		it = m_Actors.begin();
		while (it != m_Actors.end())
		{
			if ((*it)->canBlockPath())											// if a squirt interacts with a boulder, kills itself
			{
				if (distance(xval, (*it)->getX(), yval, (*it)->getY()) <= 3)
				{
					return true;
				}
				else 
					it++;
			}
			else if ((*it)->canBeAnnoyed())
			{
				if (distance(xval, (*it)->getX(), yval, (*it)->getY()) <= 3)					// otherwise, if it acts with a protestor or hardcore, then it annoys it
				{
					if ((*it)->getLeaveStatus() != true)
					{
						(*it)->annoy(2);
						playSound(SOUND_PROTESTER_ANNOYED);
						return true;
					}
					else
						it++;
				}
				else it++;
			}
			else 
				it++;
		}
	}

	if (actorCalling == FRACKMAN_TYPE)
	{
		vector<Actor*>::iterator it;
		it = m_Actors.begin();
		while (it != m_Actors.end())
		{
			if ((*it)->canBlockPath()) 
			{		
				if (distance(xval, (*it)->getX(), yval, (*it)->getY()) <= 3)				// if frackman meets a boulder while moving, then he cannot move on it
				{
					return true;
				}
				else
					it++;
			}
			else
				it++;
		}
	}

	if (actorCalling == SONAR_KIT)
	{
		vector<Actor*>::iterator it;
		it = m_Actors.begin();
		while (it != m_Actors.end())
		{
			if ((*it)->canBePicked())		
			{
				if (distance(xval, (*it)->getX(), yval, (*it)->getY()) <= 12)							// if the sonar is activated, makes items that are <= 12 away visible
				{
					(*it)->setVisible(true);
					it++;
				}
				else
					it++;
			}
			else
				it++;
		}
	}
	if (actorCalling == GOLD)			
	{
		vector<Actor*>::iterator it;
		it = m_Actors.begin();
		while (it != m_Actors.end())
		{
			if ((*it)->canBeAnnoyed())		
			{
				if (distance(xval, (*it)->getX(), yval, (*it)->getY()) <= 3)
				{
					if ((*it)->getLeaveStatus() != true)
					{
						(*it)->bribe();
						playSound(SOUND_PROTESTER_ANNOYED);
						return true;
					}
					else
						it++; 
				}
				else
					it++;
			}
			else
				it++;
		}
	}

	if (actorCalling == BOULDER)
	{
		vector<Actor*>::iterator it;
		it = m_Actors.begin();
		while (it != m_Actors.end())
		{
			if ((*it)->canBeAnnoyed())
			{
				if (distance(xval, (*it)->getX()+2, yval, (*it)->getY()+2) <= 3)								// if actor falls on a protestor, it kills them instantly
				{
					if ((*it)->getLeaveStatus() != true)
					{
						(*it)->setLeaveStatus();
						increaseScore(500);
						return false;
					}
					else
						it++;
				}
				else
					it++;
			}
			else if ((*it)->canBlockPath())
			{
				if ((*it)->getX() == xval && ((*it)->getY()) == yval)
					it++;											// this means it is the same boulder that is found, so iterator should go up
				else if (distance(xval, (*it)->getX()+2, yval, (*it)->getY()+2) <= 3)
				{
					return true;													// otherwise if it falls on a boulder, it should kill itself
				}
				else
					it++;
			}
			else
				it++;
		}
	}
	if (actorCalling == REG_PROTESTOR)
	{
		vector<Actor*>::iterator it;
		it = m_Actors.begin();
		while (it != m_Actors.end())
		{
			if ((*it)->canBlockPath())
			{
				if (distance(xval, (*it)->getX(), yval, (*it)->getY()) <= 3)										// a protestor cannot run into boulders (both reg and hardcore will call this)
				{
					return true;
				}
				else
					it++;
			}
			else
				it++;
		}
	}
	return false;		// default
}

void StudentWorld::positionOfFrack(int & xval, int & yval)
{
	xval = m_frack->getX();												// position of frack to be able to interact with him
	yval = m_frack->getY();
}

int StudentWorld::numberOilBarrelsLeft()
{
	int counter = 0;
	vector<Actor*>::iterator it;
	it = m_Actors.begin();
	while (it != m_Actors.end())
	{
		if ((*it)->getType() == OIL_BARREL)												// calcualtes the number of oil barrels left
		{
			counter++;
		}
		it++;
	}
	return counter;
}

void StudentWorld::makePoolLocation(int & xval, int & yval)
{
	int x = -1;
	int y = -1;
	createRandomPoolLocation(x, y);
	bool goodLocation = false;
	while (goodLocation == false)
	{
		if (!check4Dirts(x, y) || doSomethingWithActor(x, y, FRACKMAN_TYPE))						// makes a location for the pool similar to the randomization of the other items
		{
			createRandomLocation(x, y);
		}
		else 
			goodLocation = true;
	}
	xval = x;
	yval = y;
}

void StudentWorld::createRandomPoolLocation(int & xval, int & yval)
{
	xval = rand() % 61;										// makes random locations just for the  pool
	yval = rand() % 61;
}

bool StudentWorld::check4Dirts(int x, int y)
{
	bool goodPos = false;
	int counter = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (!isThereDirt(x + i, y + j))																	// checks a 4 x 4 squares of the grid where the x and y val are if there are dirts anywhere
				counter++;
		}
	}
	if (counter == 16)
		goodPos = true;
	return goodPos;							// if there is no dirt there, returns true
}

void StudentWorld::setDisplayText()
{
	int score = getScore();
	int level = getLevel();
	int lives = getLives();
	int health = m_frack->getHealth() * 10;
	int squirts = m_frack->getSquirt();																// displays the text on the screen
	int gold = m_frack->getGold();
	int sonar = m_frack->getSonar();
	int barrelsLeft = numberOilBarrelsLeft();

	string s = textFormatter(score, level, lives, health, squirts, gold, sonar, barrelsLeft);
	setGameStatText(s);
}

int StudentWorld::move()
{
	setDisplayText();
	findShortestPath(60, 60);											// queue based algorithm is always updated (but just once for any protestor) every tick to find optimal path
	
	if (m_frack->isDead())
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;						// player dead, so end game
	}

	if (numberOilBarrelsLeft() == 0)
	{
		playSound(SOUND_FINISHED_LEVEL);	
		return GWSTATUS_FINISHED_LEVEL;										// all oil found so move to next level
	}

	vector<Actor*>::iterator it;
	it = m_Actors.begin();
	while (it != m_Actors.end())
	{
		(*it)->doSomething();									// then all the actors act
		it++;
	}
	m_frack->doSomething();								// the frackman acts after everything else

	m_ticksToSpawnProt++;
	if (m_ticksToSpawnProt > m_maxTicks)
	{
		if (m_numProtestors < m_targetNumProtestors)
		{
			int randT = min(90, (int)(getLevel() * 10) + 30);
			if (rand() % randT == 0)
				m_Actors.push_back(new HardCoreProtestor(IID_HARD_CORE_PROTESTER, this, (int)(getLevel())));						// protestors are randomly created at appropiate intervals
			else
				m_Actors.push_back(new Protestor(IID_PROTESTER, this, (int)(getLevel()), 5));
			m_ticksToSpawnProt = 0;
			m_numProtestors++;
		}
	}

	int spawnGoodie = getLevel() * 25 + 300;
	int randGoodie = rand() % spawnGoodie;
	if (randGoodie == 1)
	{
		int chooseGoodie = rand() % 5;
		int T = min(100, int(300 - 10 * getLevel()));
		if (chooseGoodie == 0)
		{
			m_Actors.push_back(new Sonar(0, 60, this, T));														// sonars and water pools are appropiately created at random intervals too
		}
		else
		{
			int waterX, waterY;
			makePoolLocation(waterX, waterY);
			m_Actors.push_back(new WaterPool(waterX, waterY, this, T));
		}
	}

	it = m_Actors.begin();
	while (it != m_Actors.end())
	{
		if ((*it)->isDead())
		{
			if ((*it)->canBeAnnoyed())
			{
				m_numProtestors--;
				m_ticksToSpawnProt = 0;
			}
			delete(*it);
			it = m_Actors.erase(it);												// erases any dead actors
		}
		else
			it++;
	}
	return GWSTATUS_CONTINUE_GAME;									// continues game
}

void StudentWorld::cleanUp()
{
	delete m_frack;
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			delete m_dirtCounter[i][j];		// destructing all the dirts
		}
	}
	vector<Actor*>::iterator it;
	it = m_Actors.begin();
	while (it != m_Actors.end())
	{
		delete (*it);
		it = m_Actors.erase(it);										// erase and delete all actors too
	}	
}

double StudentWorld::distance(int x1, int x2, int y1, int y2)
{
	return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));					// distance function
}	

bool StudentWorld::isThereDirt(int xval, int yval)
{
	if (m_dirtCounter[yval][xval] == nullptr)										// checks if there is dirt in that area
		return false;
	return true;
}

StudentWorld::~StudentWorld()
{
	delete m_frack;							// destructing the frackman pointer
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			delete m_dirtCounter[i][j];		// destructing all the dirts
		}
	}
	vector<Actor*>::iterator it;
	it = m_Actors.begin();
	while (it != m_Actors.end())
	{
		delete (*it);													// destructing all the actors
		it = m_Actors.erase(it);
	}
}

bool StudentWorld::isDirGood(int getX, int getY, GraphObject::Direction dir)
{
	switch (dir)
	{																				// checks if the direction to move is good or not (meant to be used by protestor). checks for boulders and the dirts 
	case GraphObject::up:
		if (doSomethingWithActor(getX, getY + 4, REG_PROTESTOR) || isThereDirt(getX, getY + 4) || isThereDirt(getX + 1, getY + 4) || isThereDirt(getX + 2, getY + 4) || isThereDirt(getX + 3, getY + 4) || getY + 1>60)
			return false;
		break;
	case GraphObject::down:
		if (doSomethingWithActor(getX, getY - 1, REG_PROTESTOR) || isThereDirt(getX, getY - 1) || isThereDirt(getX + 1, getY - 1) || isThereDirt(getX + 2, getY - 1) || isThereDirt(getX + 3, getY - 1) || getY - 1 < 0)
			return false;
		break;
	case GraphObject::left:
		if (doSomethingWithActor(getX - 1, getY, REG_PROTESTOR) || isThereDirt(getX - 1, getY) || isThereDirt(getX - 1, getY + 1) || isThereDirt(getX - 1, getY + 2) || isThereDirt(getX - 1, getY + 3) || getX - 1 < 0)
			return false;
		break;
	case GraphObject::right:
		if (doSomethingWithActor(getX + 4, getY, REG_PROTESTOR) || isThereDirt(getX + 4, getY) || isThereDirt(getX + 4, getY + 1) || isThereDirt(getX + 4, getY + 2) || isThereDirt(getX + 4, getY + 3) || getX + 1>60)
			return false;
		break;
	}
	return true;
}

std::string StudentWorld::textFormatter(int score, int lvl, int live, int hp, int squirts, int gold, int sonar, int oilLeft)
{
	std::ostringstream gameText;

	gameText.fill('0');												// formats text properly using sstream
	gameText << "Scr: " << std::setw(6) << score;
	gameText.fill(' ');
	gameText << " Lvl: " << std::setw(2) << lvl << " Lives: " << std::setw(1) << live << " Hlth: " << std::setw(2) << hp << "%" << "  Water: " << std::setw(2) << squirts << "  Gold: " << gold << "  Sonar: " << sonar << "  Oil Left: " << oilLeft;
	return gameText.str();
}

bool StudentWorld::deleteDirt(int xVal, int yVal)
{
	if (m_dirtCounter[yVal][xVal] != nullptr)				// only deletes the dirts that exist in the x and y positions passed in -> used by frackman and boulder
	{
		delete m_dirtCounter[yVal][xVal];
		m_dirtCounter[yVal][xVal] = nullptr;
		return true;
	}
	return false;
}


void StudentWorld::findShortestPath(int x, int y)
{
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			m_shortestPath[i][j] = GraphObject::Direction::none;
		}
	}
	std::queue<Coord> coordQueue;
	Coord a(x, y);
	m_shortestPath[63 - y][x] = GraphObject::Direction::none;
	coordQueue.push(a);
																						// breadth-first search using queues to find the shortest path (stored in an array of directions)
	while (!coordQueue.empty())
	{
		Coord b = coordQueue.front();
		coordQueue.pop();
		int xval = b.x(); int yval = b.y();
																							// moving up
		if (isDirGood(xval, yval, GraphObject::Direction::up) && m_shortestPath[63 - yval - 1][xval] == GraphObject::Direction::none)
		{
			coordQueue.push(Coord(xval, yval + 1));
			m_shortestPath[63 - yval - 1][xval] = GraphObject::Direction::down;
		}
																							// moving down
		if (isDirGood(xval, yval, GraphObject::Direction::down) && m_shortestPath[63 - yval + 1][xval] == GraphObject::Direction::none)
		{
			coordQueue.push(Coord(xval, yval - 1));
			m_shortestPath[63 - yval + 1][xval] = GraphObject::Direction::up;
		}
																								// moving right
		if (isDirGood(xval, yval, GraphObject::Direction::right) && m_shortestPath[63 - yval][xval + 1] == GraphObject::Direction::none)
		{
			coordQueue.push(Coord(xval + 1, yval));
			m_shortestPath[63 - yval][xval + 1] = GraphObject::Direction::left;
		}
																			// moving left
		if (isDirGood(xval, yval, GraphObject::Direction::left) && m_shortestPath[63 - yval][xval - 1] == GraphObject::Direction::none)
		{
			coordQueue.push(Coord(xval - 1, yval));
			m_shortestPath[63 - yval][xval - 1] = GraphObject::Direction::right;
		}
	}
}

void StudentWorld::findHardCorePath(int x, int y)			// int x and y are the positions of the frackman
{
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			m_HardCorePath[i][j] = GraphObject::Direction::none;
			m_shortestLength[i][j] = -1;																// for hardcore, we need both the direction AND the number of steps to that position (so 2 arrays)
		}
	}
	std::queue<Coord> coordQueue;
	Coord a(x, y);
	m_HardCorePath[63 - y][x] = GraphObject::Direction::none;
	m_shortestLength[63 - y][x] = 0;
	coordQueue.push(a);
																							// breadth-first search using queues and the same logic above to find the fastest path AND the number of steps to frackman from ANY location
	while (!coordQueue.empty())
	{
		Coord b = coordQueue.front();
		coordQueue.pop();
		int xval = b.x(); int yval = b.y();
																						// moving up
		if (isDirGood(xval, yval, GraphObject::Direction::up) && m_HardCorePath[63 - yval - 1][xval] == GraphObject::Direction::none)
		{
			coordQueue.push(Coord(xval, yval + 1));
			m_HardCorePath[63 - yval - 1][xval] = GraphObject::Direction::down;
			m_shortestLength[63 - yval - 1][xval] = m_shortestLength[63 - yval][xval] + 1;
		}
																								// moving down
		if (isDirGood(xval, yval, GraphObject::Direction::down) && m_HardCorePath[63 - yval + 1][xval] == GraphObject::Direction::none)
		{
			coordQueue.push(Coord(xval, yval - 1));
			m_HardCorePath[63 - yval + 1][xval] = GraphObject::Direction::up;
			m_shortestLength[63 - yval + 1][xval] = m_shortestLength[63 - yval][xval] + 1;
		}
																									// moving right
		if (isDirGood(xval, yval, GraphObject::Direction::right) && m_HardCorePath[63 - yval][xval + 1] == GraphObject::Direction::none)
		{
			coordQueue.push(Coord(xval + 1, yval));
			m_HardCorePath[63 - yval][xval + 1] = GraphObject::Direction::left;
			m_shortestLength[63 - yval][xval + 1] = m_shortestLength[63 - yval][xval] + 1;
		}
																											// moving left
		if (isDirGood(xval, yval, GraphObject::Direction::left) && m_HardCorePath[63 - yval][xval - 1] == GraphObject::Direction::none)
		{
			coordQueue.push(Coord(xval - 1, yval));
			m_HardCorePath[63 - yval][xval - 1] = GraphObject::Direction::right;
			m_shortestLength[63 - yval][xval - 1] = m_shortestLength[63 - yval][xval] + 1;
		}
	}
}

GraphObject::Direction StudentWorld::getProtDirToMove(int x, int y)
{
	return m_shortestPath[63 - y][x];												// returns the  direction for that position to travel the shortest path back home
}

GraphObject::Direction StudentWorld::getHardCoreProtDir(int x, int y)
{
	return m_HardCorePath[63 - y][x];													// returns the direction for that position to travel the shortest path to frackman
}

bool StudentWorld::protMovesAway(int getX, int getY, int mMoves)
{
	if (m_shortestLength[63 - getY][getX] <= mMoves)
		return true;																// returns if in that position, the hardcore protestor is within mMoves from the frackman and can "loate his cell phone"
	return false;
}
