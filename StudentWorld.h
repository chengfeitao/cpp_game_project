#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Level.h"	//variable rather than pointer in StudentWorld private member

#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;	//pointer in StudentWorld private member
class Player;

class StudentWorld : public GameWorld
{
public:

	StudentWorld():GameWorld(){
		playerPtr = NULL;
		currentLevelComplete = false;
	}

	virtual ~StudentWorld(){
		//dummy
	}

	//-----------init-----------------------------
	virtual int init();
	//--------------------------------------------


	//-----------move---------------------------------------------------------------------
	virtual int move();

	std::string formatItems(int score, int level, int livesLeft, unsigned int bonus);	//helper func for move()
	void updateDisplayText();	//helper func for move()

	int getBonusRemain(){
		return bonusRemain;
	}

	void increaseScoreByBonus(){
		increaseScore(bonusRemain);
	}

	void removeDeadGameObjects();
	//-------------------------------------------------------------------------------------

	//----------cleanup-----------------------------------------------------------------
	virtual void cleanUp()
    {
    }
	//----------------------------------------------------------------------------------

	std::vector<Actor*> getActorVec(){
		return actorVec;
	}

	void addToActorVec(Actor* a){	//need public function in base class to modify private member
		actorVec.push_back(a);
	}

	Player* getPlayerPtr(){
		return playerPtr;
	}

	void setCurrentLevelComplete(){	//Exit will call this function to signal StudentWorld
		currentLevelComplete = true;
	}

private:
	Player* playerPtr;
	std::vector<Actor*> actorVec;	//single container required in spec	
	Level lev;
	int bonusRemain;
	bool currentLevelComplete;	//Exit use this to signal StudentWorld that Player has completed current level
};

#endif // STUDENTWORLD_H_
