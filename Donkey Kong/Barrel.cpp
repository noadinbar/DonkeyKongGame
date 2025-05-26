#include "Barrel.h"
#include <iostream>
#include "Mario.h"
#include "Board.h"
#include "Point.h"
#include "Configure.h"

bool Barrel::checkFallToExplode() //when barrel meets the ground, we check if it needs to explode
{
	if (_countFall >= MAX_FALL_BARREL)
	{
		return true; //the barrel is exploding
	}
	else
	{
		_countFall = 0; // if it doesnt need to explode, we reset the count
		_isFalling = false; // its on the ground so its not falling anymore
	}
	return false;
}

//the paramter is for us to know if to print, if were not on automatic mode, we always send false
bool Barrel::move(bool silent)
{
	if (!silent)
	{
		draw(getBoard()->getChar(getX(), getY())); //returns what was on the board before barrel
	}
	bool exploded = false; //boolean to check if the barrel needs to explode

	//on every type of stage, check if barrel needs to explode, and set its direction to keep moving
	if (getBoard()->isEqualStage(getX(), getY() + 1))
	{
		exploded = checkFallToExplode();
		if (getLastDir() == RIGHT_STAGE)
		{
			setDirX(1);
			setDirY(0);
		}
		else
		{
			setDirX(-1);
			setDirY(0);
		}
	}
	else if (getBoard()->isRightStage(getX(), getY() + 1))
	{
		exploded = checkFallToExplode();
		setDirX(1);
		setDirY(0);
	}
	else if (getBoard()->isLeftStage(getX(), getY() + 1))
	{
		exploded = checkFallToExplode();
		setDirX(-1);
		setDirY(0);
	}

	//if the barrel is above an empty space, it means its falling
	else if (getBoard()->isEmpty(getX(), getY() + 1))
	{
		setDirX(0);
		setDirY(1);
		_countFall++;
		_isFalling = true;
	}
	//when the barrel meets a wall it needs to explode
	if (getBoard()->isWall(getX() + getDirX(), getY() + getDirY()))
	{
		setDirX(0);
		setDirY(0);
		exploded = true;
	}

	//if the barrel doesnt need to explode, we can update its location
	if (!exploded)
	{
		updateXY();//_dirX, _dirY
		if (!silent)
		{
			draw(BARREL);
		}

	}

	return exploded;
}


