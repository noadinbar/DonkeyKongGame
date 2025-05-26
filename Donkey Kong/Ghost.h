#pragma once
#include "Point.h"
class Ghost :public Point
{
	char _under = EQUAL_STAGE;
public:
	Ghost(int startX, int startY) { setX(startX); setY(startY); setDirX(1); }
	char whoIsUnder() const { return _under; } //get the symbol of the stage to know the direction of the barrel
	void setUnder(char newUnder) { _under = newUnder; }
	virtual void move(int random_value, bool silent);
	bool canMove();
	bool operator==(const Ghost& other) const { return (getX() == other.getX() && getY() == other.getY()); }
};

