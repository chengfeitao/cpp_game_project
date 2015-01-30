#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp



/////////////Actor////////////////////////////////////////////////////

bool Actor::meetZumi(int x, int y){
	StudentWorld* sw = getSWPtr();
	for(int i=0; i<(sw->getActorVec().size()); i++){
		if(x == sw->getActorVec()[i]->getX() &&
			y == sw->getActorVec()[i]->getY()){
			if(sw->getActorVec()[i]->getID() == IID_SIMPLE_ZUMI ||
				sw->getActorVec()[i]->getID() == IID_COMPLEX_ZUMI){
				return true;
			}
		}
	}
	return false;
}

bool Actor::meetBrick(int x, int y){
	StudentWorld* sw = getSWPtr();
	for(int i=0; i<(sw->getActorVec().size()); i++){
		if(x == sw->getActorVec()[i]->getX() &&
			y == sw->getActorVec()[i]->getY()){
			if(sw->getActorVec()[i]->getID() == IID_PERMA_BRICK ||
				sw->getActorVec()[i]->getID() == IID_DESTROYABLE_BRICK){
				return true;
			}
		}
	}
	return false;
}

bool Actor::meetDestroyableBrick(int x, int y, int& index){
	StudentWorld* sw = getSWPtr();
	for(int i=0; i<(sw->getActorVec().size()); i++){
		if(x == sw->getActorVec()[i]->getX() &&
			y == sw->getActorVec()[i]->getY()){
			if(sw->getActorVec()[i]->getID() == IID_DESTROYABLE_BRICK){
				index = i;
				return true;
			}
		}
	}
	return false;
}

bool Actor::meetPermanentBrick(int x, int y){
	StudentWorld* sw = getSWPtr();
	for(int i=0; i<(sw->getActorVec().size()); i++){
		if(x == sw->getActorVec()[i]->getX() &&
			y == sw->getActorVec()[i]->getY()){
			if(sw->getActorVec()[i]->getID() == IID_PERMA_BRICK){
				return true;
			}
		}
	}
	return false;
}

bool Actor::meetPlayerOrZumi(int x, int y, int& index){
	StudentWorld* sw = getSWPtr();
	if(x == sw->getPlayerPtr()->getX() && y == sw->getPlayerPtr()->getY()){
		index = -1;
		return true;
	}
	for(int i=0; i<(sw->getActorVec().size()); i++){
		if(x == sw->getActorVec()[i]->getX() &&
			y == sw->getActorVec()[i]->getY()){
			if(sw->getActorVec()[i]->getID() == IID_SIMPLE_ZUMI ||
				sw->getActorVec()[i]->getID() == IID_COMPLEX_ZUMI){
				index = i;
				return true;
			}
		}
	}
	return false;
}

bool Actor::meetBugSprayer(int x, int y, int& index){
	StudentWorld* sw = getSWPtr();
	for(int i=0; i<(sw->getActorVec().size()); i++){
		if(x == sw->getActorVec()[i]->getX() &&
			y == sw->getActorVec()[i]->getY()){
			if(sw->getActorVec()[i]->getID() == IID_BUGSPRAYER &&
				!sw->getActorVec()[i]->getDied()){
				index = i;
				return true;
			}
		}
	}
	return false;	
}
///////////////////////////////////////////////////////////////////////



///////////////Player//////////////////////////////////////////////////
void Player::playerMove(int x, int y){
	StudentWorld* sw = getSWPtr();
	bool canMove = true;
	for(int i=0; i<(sw->getActorVec().size()); i++){
		if(x == sw->getActorVec()[i]->getX() &&
			y == sw->getActorVec()[i]->getY()){
			if(sw->getActorVec()[i]->getID() == IID_PERMA_BRICK){
				canMove = false;
			}
			if(sw->getActorVec()[i]->getID() == IID_DESTROYABLE_BRICK && walkThroughTimeRemain == 0){
				canMove = false;
			}
		}
	}
	if(canMove){
		moveTo(x,y);
	}
}

int Player::getNumOfLiveBugSprayer(){
	int counter = 0;
	StudentWorld* sw = getSWPtr();
	for(int i=0; i<(sw->getActorVec().size()); i++){
		if(sw->getActorVec()[i]->getID() == IID_BUGSPRAYER &&
			!sw->getActorVec()[i]->getDied()){
			counter++;
		}
	}
	return counter;
}

void Player::dropBugSprayer(int x, int y){
	int tempIndex;
	if(!meetBrick(x,y) && 
		!meetBugSprayer(x,y,tempIndex) && 
		(getNumOfLiveBugSprayer()<availableBugSprayer)){
			//----add a Bug Sprayer at Player's location------------------------------
			BugSprayer* bsr = new BugSprayer(getSWPtr(), IID_BUGSPRAYER, getX(), getY());	
			getSWPtr()->addToActorVec(bsr);
	}
}

void Player::playerOrZumiDie(){
	//std::cout<<"entered playerOrZumiDie()"<<std::endl;	//test
	setDied();
	//if(getDied()){std::cout<<"player died!"<<std::endl;}	//test
	StudentWorld* sw = getSWPtr();
	sw->playSound(SOUND_PLAYER_DIE);
}

void Player::doSomething(){
	int tempIndex;
	if(getDied()){
		return;
	}
	else{
		if(meetZumi(getX(), getY())){
			playerOrZumiDie();
			return;
		}
		else if(meetDestroyableBrick(getX(), getY(), tempIndex) && walkThroughTimeRemain == 0){
			playerOrZumiDie();
			return;
		}
		else{
			while(walkThroughTimeRemain>0){
				walkThroughTimeRemain--;
			}
			while(boostedSprayerTimeRemain>0){
				boostedSprayerTimeRemain--;
			}
			int ch;
			if(getSWPtr()->getKey(ch)){
				switch(ch){
					case KEY_PRESS_LEFT:
						playerMove(getX()-1, getY());
						break;
					case KEY_PRESS_RIGHT:
						playerMove(getX()+1, getY());
						break;
					case KEY_PRESS_UP:
						playerMove(getX(), getY()+1);
						break;
					case KEY_PRESS_DOWN:
						playerMove(getX(), getY()-1);
						break;
					case KEY_PRESS_SPACE:
						dropBugSprayer(getX(), getY());
						break;
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////




////////Brick////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////



//////////Exit/////////////////////////////////////////////////////////////
void Exit::doSomething(){
	if(getSWPtr()->getPlayerPtr()->getX() == getX() &&
		getSWPtr()->getPlayerPtr()->getY() == getY() &&
		isVisible()){
		getSWPtr()->playSound(SOUND_FINISHED_LEVEL);
		getSWPtr()->setCurrentLevelComplete();
	}
}
///////////////////////////////////////////////////////////////////////////


///////////Bug Sprayers////////////////////////////////////////////////
void BugSprayer::doSomething(){
	if(getDied()){
		//std::cout<<"bug sprayer died!"<<std::endl;	//testing
		return;
	}
	else{
		setTimer(getTimer()-1);
		//std::cout<<getTimer()<<std::endl;	//testing
		if(getTimer() == 0){
			//----add a Bug Spray at Bug Sprayer's location------------------------------
			BugSpray* bs_mid = new BugSpray(getSWPtr(), IID_BUGSPRAY, getX(), getY());	
			getSWPtr()->addToActorVec(bs_mid);
			//-----add up to 2 Bug Spray in right direction-----------------------------
			for(int i=1; i<3; i++){
				if(!meetPermanentBrick(getX()+i, getY())){
					BugSpray* bs_right = new BugSpray(getSWPtr(), IID_BUGSPRAY, getX()+i, getY());
					getSWPtr()->addToActorVec(bs_right);
				}
				else{
					break;
				}
			}
			//-----add up to 2 Bug Spray in left direction---------------------------
			for(int i=1; i<3; i++){
				if(!meetPermanentBrick(getX()-i, getY())){
					BugSpray* bs_left = new BugSpray(getSWPtr(), IID_BUGSPRAY, getX()-i, getY());
					getSWPtr()->addToActorVec(bs_left);
				}
				else{
					break;
				}
			}
			//-----add up to 2 Bug Spray in up direction---------------------------
			for(int i=1; i<3; i++){
				if(!meetPermanentBrick(getX(), getY()+i)){
					BugSpray* bs_up = new BugSpray(getSWPtr(), IID_BUGSPRAY, getX(), getY()+i);
					getSWPtr()->addToActorVec(bs_up);
				}
				else{
					break;
				}
			}
			//-----add up to 2 Bug Spray in down direction---------------------------
			for(int i=1; i<3; i++){
				if(!meetPermanentBrick(getX(), getY()-i)){
					BugSpray* bs_down = new BugSpray(getSWPtr(), IID_BUGSPRAY, getX(), getY()-i);
					getSWPtr()->addToActorVec(bs_down);
				}
				else{
					break;
				}
			}
			//--------Bug Sprayer die after releasing spray----------------------
			getSWPtr()->playSound(SOUND_SPRAY);
			setDied();
			//if(getDied()){std::cout<<"bug sprayer set to die!"<<std::endl;}	//testing
		}
	}	
}
///////////////////////////////////////////////////////////////////////



/////////////Bug Spray///////////////////////////////////////////////
void BugSpray::doSomething(){
	if(getDied()){
		return;
	}
	else{
		setTimer(getTimer()-1);
		if(getTimer() == 0){
			setDied();
		}
		else{
			int index;
			if(meetDestroyableBrick(getX(), getY(), index)){
				getSWPtr()->getActorVec()[index]->setDied();
				//if(getSWPtr()->getActorVec()[index]->getDied()){std::cout<<"brick died!"<<std::endl;} //test
			}
			else if(meetPlayerOrZumi(getX(), getY(), index) && index != -1){
				getSWPtr()->getActorVec()[index]->playerOrZumiDie();	//Zumi die
				//if(getSWPtr()->getActorVec()[index]->getDied()){std::cout<<"Zumi died!"<<std::endl;} //test
			}
			else if(meetPlayerOrZumi(getX(), getY(), index) && index == -1){
				getSWPtr()->getPlayerPtr()->playerOrZumiDie();
				//if(getSWPtr()->getPlayerPtr()->getDied()){std::cout<<"Player died!"<<std::endl;} //test
			}
			else if(meetBugSprayer(getX(), getY(), index)){
				getSWPtr()->getActorVec()[index]->setTimer(1);
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////
