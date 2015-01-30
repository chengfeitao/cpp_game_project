#include "StudentWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw


GameWorld* createStudentWorld()
{
	return new StudentWorld();
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

/////////////helper functions////////////////////////////////////////////////////////
std::string StudentWorld::formatItems(int score, int level, int livesLeft, unsigned int bonus){
	std::ostringstream oss;
	oss<<"Score: ";
	oss.fill('0');
	oss<<std::setw(8)<<score<<"  "<<"Level: ";
	oss.fill('0');
	oss<<std::setw(2)<<level<<"  "<<"Lives: ";
	oss.fill('0');
	oss<<std::setw(3)<<livesLeft<<"  "<<"Bonus:";
	oss.fill(' ');
	oss<<std::setw(6)<<bonus;
	return oss.str();
}

void StudentWorld::updateDisplayText(){
	int score = getScore();
	int level = getLevel();
	int bonus = getBonusRemain();
	//std::cout<<"level bonus: "<<bonus<<std::endl;	//testing
	int livesLeft = getLives();
	// Next, create a string from your statistics, of the form:
	// "Score: 00000100 Level: 09 Lives: 003 Bonus:	345"
	std::string s = formatItems(score,level,livesLeft,bonus);
	// Finally, update the display text at the top of the screen with your
	// newly created stats
	setGameStatText(s);	// calls our provided GameWorld::setGameStatText
}

void StudentWorld::removeDeadGameObjects(){
	bool removalNotDone = true;
	while(removalNotDone){
		removalNotDone = false;
		for(int i=0; i<actorVec.size(); i++){
			if(actorVec[i]->getDied()){
				removalNotDone = true;
				Actor* toBeDeleted = actorVec[i];
				actorVec.erase(actorVec.begin()+i);
				delete toBeDeleted;
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////



///////////////init()////////////////////////////////////////////////////////////////
int StudentWorld::init(){
//	while(!actorVec.empty()){
//		actorVec.pop_back();
//	}
	std::string curLevelDat = "level0"+std::to_string(getLevel())+".dat";	
	//std::cout<<curLevelDat<<std::endl;	//for testing
	Level::LoadResult result = lev.loadLevel(curLevelDat);
	if(getLevel() == 0 && result == Level::load_fail_file_not_found){
		return GWSTATUS_NO_FIRST_LEVEL;
	}
	if(getLevel() != 0 && result == Level::load_fail_file_not_found){
		return GWSTATUS_PLAYER_WON;
	}
	if(result == Level::load_fail_bad_format){
		return GWSTATUS_LEVEL_ERROR;
	}
	//else if(result == Level::load_success){
		for(int y=0; y<VIEW_HEIGHT; y++){
			for(int x=0; x<VIEW_WIDTH; x++){
				Level::MazeEntry ge = lev.getContentsOf(x,y);
				Player* p;	//cannot initialize pointer/variable inside switch
				DestroyableBrick* db;
				PermanentBrick* pb;
				switch(ge){
					case Level::empty:
						break;
					case Level::exit:
						break;//dummy
					case Level::player:
						p = new Player(this, IID_PLAYER, x, y);
						playerPtr = p;
						break;
					case Level::simple_zumi:
						break;//dummy
					case Level::complex_zumi:
						break;//dummy
					case Level::perma_brick:
						pb = new PermanentBrick(this, IID_PERMA_BRICK, x, y);
						actorVec.push_back(pb);
						break;
					case Level::destroyable_brick:
						db = new DestroyableBrick(this, IID_DESTROYABLE_BRICK, x, y);
						actorVec.push_back(db);
						break;
				}
			}
		}
		//----------save parameters into StudentWorld-----------------
		bonusRemain = lev.getOptionValue(optionLevelBonus);
		//------------------------------------------------------------
		return GWSTATUS_CONTINUE_GAME;
	//}


//unsigned int val = lev.getOptionValue(optionProbOfGoodieOverall);
//cout << "The probability of a Zumi dropping a goodie is: ";
//cout << val << " percent. " << endl;
//}

}
//////////////////////////////////////////////////////////////////////////////////////////



/////////////move()//////////////////////////////////////////////////////////////////////

int StudentWorld::move(){
	// Update the Game Status Line
	updateDisplayText();	// update the score/lives/level text at screen top

	// Give player a chance to do something------------------------
	if(!playerPtr->getDied()){
		// ask the actor to do something (e.g. move)
		playerPtr->doSomething();
		if(playerPtr->getDied()){
			return GWSTATUS_PLAYER_DIED;
		}
		if(currentLevelComplete){
			//increaseScoreByBonus();
			//return GWSTATUS_FINISHED_LEVEL;
		}
	}
	//---------------------------------------------------------------


	//Give other actors a chance to do something---------------------
	for(int i=0; i<actorVec.size(); i++){
		/*****debug**********************************
		if(actorVec[i]->getID() == IID_BUGSPRAYER){
			std::cout<<actorVec[i]->getID()<<std::endl;
		}
		/**********************************************/
		if(!actorVec[i]->getDied()){
			// ask the actor to do something (e.g. move)
			actorVec[i]->doSomething();
			if(playerPtr->getDied()){
				return GWSTATUS_PLAYER_DIED;
			}
			if(currentLevelComplete){
				increaseScoreByBonus();
				return GWSTATUS_FINISHED_LEVEL;
			}
		}
	}
	//----------------------------------------------------------------

	// Remove newly-dead actors after each tick
	removeDeadGameObjects(); // delete dead game objects
//// Reduce the current bonus for the Level by one
//reduceLevelBonusByOne();
//// If the Player has killed all the Zumi on the level, then we
//// must expose the exit so the Player can advance to the next level
//if (playerHasKilledAllZumiOnTheLevel())
//exposeTheExitInTheMaze(); // make the exit Active
//// return the proper result
//if (playerDiedDuringThisTick())
//return GWSTATUS_PLAYER_DIED;
//if (playerCompletedCurrentLevel())
//{
//increaseScoreByBonus();
//return GWSTATUS_FINISHED_LEVEL;
//}
//// the Player hasn’t completed the current level and hasn’t died, so
//// continue playing the current level
return GWSTATUS_CONTINUE_GAME;

////--------just for compilation----------------
//        decLives();
//        return GWSTATUS_PLAYER_DIED;
////---------------------------------------------
}
//////////////////////////////////////////////////////////////////////////////////////////
