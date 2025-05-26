#pragma once
#include "Point.h"
#include "Configure.h"
#include "Board.h"

class Mario : public Point
{
private:
	bool _isClimbing = false;
	int _fallCount = 0;
	int _jumpState = 0; // is between 1-4 because each state has a different handling
	int _jumpDirX = 0; // saves the diretion mario was before jumping so it can keep moving in that direction during and after the jump
	eKeys _lastDir = eKeys::STAY_L; //because mario keeps moving in the same direction if we dont press a key

public:
	void init(int x, int y, int dirX, int dirY, Board* pBoard); //initalize mario
	eKeys getDir() const { return _lastDir; }
	void setDir(eKeys key);
	int getFallCount() const { return _fallCount; }
	void setFallCount(int newCount) { _fallCount = newCount; }
	void move(eKeys key, bool silent);
	bool canMove();
	void handleJump();

};

