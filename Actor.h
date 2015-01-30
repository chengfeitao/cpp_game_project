#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

/////////possible design//////////////////////////////////////////
//GraphObject
//    Actor
//        Player
//        Brick
//            PermanentBrick
//            DestroyableBrick
//        Exit
//        TimedLifetimeActor
//            BugSprayer
//            BugSpray
//            Goodie
//                ExtraLifeGoodie
//                WalkThroughWallsGoodie
//                IncreaseSimultaneousSprayersGoodie
//        Zumi
//            SimpleZumi
//            ComplexZumi
////////////////////////////////////////////////////////////////

class StudentWorld;


///////////Actor/////////////////////////////////////////////////////////////////////
class Actor: public GraphObject{
	public:
		Actor(StudentWorld* SWPtr, int imageID, int startX, int startY):GraphObject(imageID, startX, startY){
			studentWorldPtrInActor = SWPtr;
			died = false;
			GraphObject::setVisible(true);	//set all actors visible, expect Exit, who will be set invisible
		}
		virtual ~Actor(){}

		virtual void doSomething() = 0;	//must have doSomething in the base class for dynamic dispatching
										//pure virtual function has to be defined in all derived class

		virtual void playerOrZumiDie(){}	//not pure virtual, can skip definition in derived class
		virtual void setTimer(int newTime){}

		bool meetZumi(int x, int y);
		bool meetBrick(int x, int y);
		bool meetDestroyableBrick(int x, int y, int& index);
		bool meetPermanentBrick(int x, int y);
		bool meetPlayerOrZumi(int x, int y, int& index);
		bool meetBugSprayer(int x, int y, int& index);

		StudentWorld* getSWPtr(){	//derived class cannot access private of base class, need public accessor
			return studentWorldPtrInActor;
		}
		bool getDied(){
			return died;
		}
		void setDied(){
			died = true;
		}

	private:
		StudentWorld* studentWorldPtrInActor;
		bool died;
};
/////////////////////////////////////////////////////////////////////////////////////



//////////Player//////////////////////////////////////////////////////////////////////
class Player: public Actor{
	public:
		Player(StudentWorld* SWPtr, int imageID, int startX, int startY):Actor(SWPtr, imageID, startX, startY){
			availableBugSprayer = 2;
			walkThroughTimeRemain = 0;
			boostedSprayerTimeRemain = 0;
		}
		virtual ~Player(){}
		virtual void doSomething();
		virtual void playerOrZumiDie();

		void playerMove(int x, int y);	//arrow key press
		void dropBugSprayer(int x, int y);	//space key press

		int getNumOfLiveBugSprayer();		

	private:
		int availableBugSprayer;
		int walkThroughTimeRemain;
		int boostedSprayerTimeRemain;
};
/////////////////////////////////////////////////////////////////////////////////////



/////////two kinds of Brick//////////////////////////////////////////////////////////
class Brick: public Actor{
	public:
		Brick(StudentWorld* SWPtr, int imageID, int startX, int startY)
			:Actor(SWPtr, imageID, startX, startY){}
		virtual ~Brick(){}
		virtual void doSomething() = 0;
};
class PermanentBrick: public Brick{
	public:
		PermanentBrick(StudentWorld* SWPtr, int imageID, int startX, int startY)
			:Brick(SWPtr, imageID, startX, startY){}
		virtual ~PermanentBrick(){}
		virtual void doSomething(){}
};
class DestroyableBrick: public Brick{
	public:
		DestroyableBrick(StudentWorld* SWPtr, int imageID, int startX, int startY)
			:Brick(SWPtr, imageID, startX, startY){}
		virtual ~DestroyableBrick(){}
		virtual void doSomething(){}
};
//////////////////////////////////////////////////////////////////////////////////////



///////////////Exit/////////////////////////////////////////////////////
class Exit: public Actor{
	public:
		Exit(StudentWorld* SWPtr, int imageID, int startX, int startY)
			:Actor(SWPtr, imageID, startX, startY){
			GraphObject::setVisible(false);
		}
		virtual ~Exit(){}
		virtual void doSomething();
};
////////////////////////////////////////////////////////////////////////



//////////Bug Sprayers/////////////////////////////////////////////////
class TimedLifetimeActor: public Actor{
	public:
		TimedLifetimeActor(StudentWorld* SWPtr, int imageID, int startX, int startY)
			:Actor(SWPtr, imageID, startX, startY){}
		virtual ~TimedLifetimeActor(){}
		virtual void doSomething() = 0;

		int getTimer(){
			return timer;
		}
		virtual void setTimer(int newTime){
			timer = newTime;
		}

	private:
		int timer;
};

class BugSprayer: public TimedLifetimeActor{
	public:
		BugSprayer(StudentWorld* SWPtr, int imageID, int startX, int startY)
			:TimedLifetimeActor(SWPtr, imageID, startX, startY){
			setTimer(40);
		}
		virtual ~BugSprayer(){}
		virtual void doSomething();
};
/////////////////////////////////////////////////////////////////////////



///////////////Bug Spray/////////////////////////////////////////////
class BugSpray: public TimedLifetimeActor{
	public:
		BugSpray(StudentWorld* SWPtr, int imageID, int startX, int startY)
			:TimedLifetimeActor(SWPtr, imageID, startX, startY){
			setTimer(3);
		}
		virtual ~BugSpray(){}
		virtual void doSomething();
};
/////////////////////////////////////////////////////////////////////
	
#endif // ACTOR_H_
