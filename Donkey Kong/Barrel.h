#pragma once
#include "Point.h"
#include "Configure.h"
#include "Mario.h"

class Barrel :public Point
{
private:
	char _lastDir = LEFT_STAGE; //saving the last direction of the barrel for handling moving on equal stage
	char _under = EQUAL_STAGE; //so we know what the direction of the barrel needs to be
	int _countFall = 0;
	bool _isFalling = false;
public:
	Barrel(int startX, int startY) { setX(startX); setY(startY); } //constructor for creating new barrel
	char getLastDir() const { return _lastDir; }
	void setDir(char direction) { _lastDir = direction; }
	char whoIsUnder() const { return _under; } //get the symbol of the stage to know the direction of the barrel
	void setUnder(char newUnder) { _under = newUnder; }
	bool move(bool silent);
	bool checkFallToExplode(); //checks if the fall of the barrel is 8 chars or more

};

