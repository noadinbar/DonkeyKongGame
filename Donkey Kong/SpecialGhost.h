#pragma once
#include "Ghost.h"
#include "Point.h"

class SpecialGhost : public Ghost
{
private:
	bool _isClimbing = false;
	bool _doneClimbing = false;
public:
	SpecialGhost(int startX, int startY) :Ghost(startX, startY) { }
	virtual void move(int random_value, bool silent) override;
	bool canMove(int random);
};

