#include <iostream>
#include "Mario.h"
#include <conio.h>
#include "Configure.h"
#include "GameBase.h"
using namespace std;

void Mario::init(int x, int y, int dirX, int dirY, Board* pBoard)
{
	Point::init(x, y, dirX, dirY, pBoard);
	_fallCount = 0;
	_jumpState = 0;
	_lastDir = eKeys::STAY_L; //mario stands in place when starting, waiting for user to set the direction
	_jumpDirX = 0;

}

//sets the direction of mario according to the key from user, also saving the direction
void Mario::setDir(eKeys key)
{
	if (key == eKeys::LEFT_L || key == eKeys::LEFT_U)
	{
		setDirX(-1);
		setDirY(0);
		_lastDir = eKeys::LEFT_L;
	}
	else if (key == eKeys::RIGHT_L || key == eKeys::RIGHT_U)
	{
		setDirX(1);
		setDirY(0);
		_lastDir = eKeys::RIGHT_L;
	}
	else if (key == eKeys::STAY_L || key == eKeys::STAY_U)
	{
		setDirX(0);
		setDirY(0);
		_lastDir = eKeys::STAY_L;
	}
	else if (key == eKeys::UP_L || key == eKeys::UP_U)
	{
		setDirX(0);
		setDirY(-1);
		_lastDir = eKeys::UP_L;
	}
	else if (key == eKeys::DOWN_L || key == eKeys::DOWN_U)
	{
		setDirX(0);
		setDirY(1);
		_lastDir = eKeys::DOWN_L;
	}
}

//returns if mario can move to the next location
bool Mario::canMove()
{
	int newX = getX() + getDirX();
	int newY = getY() + getDirY();

	//if mario is on ladder and climbing up, allow him to climb up
	if (getBoard()->isStage(getX(), getY()) && _isClimbing && getDirY() < 0)
	{
		return true;
	}

	if (getBoard()->isLadder(getX(), getY()))
	{
		//if marios next move is up to be on stage, return true, and change _isClimbing to true so we will know to stop him when he finishes climbing
		if (getDirY() < 0 && getBoard()->isStage(newX, newY))
		{
			_isClimbing = true;
			return true;
		}

		//if mario tries to move left or right while he's on ladder, we dont let him
		if (getDirX() != 0 && !getBoard()->isStage(newX, newY + 1))
			return false;

		//if mario wants to go down in ladder and his next move is on stage, we dont let him
		if (getDirY() == 1 && getBoard()->isStage(newX, newY))
			return false;


		return true;
	}

	// if marios next move is one of the below, we dont let him
	if (getBoard()->isWall(newX, newY) || getBoard()->isDonkeyKong(newX, newY) || getBoard()->isStage(newX, newY))
		return false;


	if (_jumpState > 0)
	{

		//if mario is during jump and in the 2 moves up will hit a wall or stage, we dont let him
		if (_jumpState <= 2 && (getBoard()->isWall(getX() + _jumpDirX, getY() - 1) || getBoard()->isStage(getX() + _jumpDirX, getY() - 1)))
		{

			return false;
		}

		else if (_jumpState <= 4 && _jumpState > 2 && (getBoard()->isWall(getX() + _jumpDirX, getY() + 1) || getBoard()->isStage(getX() + _jumpDirX, getY() + 1)))
		{

			return false;
		}


	}

	// if we got here it means mario can move :) 
	return true;
}

//the paramter is for us to know if to print, if were not on automatic mode, we always send false
void Mario::move(eKeys key, bool silent)
{
	//if mario isnt jumping
	if (_jumpState == 0)
	{
		//when mario was climbing and now reached to be above a stage we want him to stop moving
		if (_isClimbing && getBoard()->isStage(getX(), getY() + 1))
		{
			_isClimbing = false;
			setDirX(0);
			setDirY(0);
			key = eKeys::STAY_L;

		}
		//if mario is not climbing and the user pressed up
		else if ((key == eKeys::UP_L || key == eKeys::UP_U) && !_isClimbing)
		{

			_jumpDirX = getDirX(); //we save the direction of the jump

			//if hes not on ladder it means he can jump
			if (!getBoard()->isLadder(getX(), getY()))
			{
				_jumpState = 1; // start jumping
			}

		}
		setDir(key); //set the direction by the key 
	}

	//if marios next position is valid
	if (canMove())
	{
		if (!silent)
		{
			draw(getBoard()->getChar(getX(), getY())); //returns what was on the board before mario
		}


		// if mario is jumping
		if (_jumpState > 0)
		{
			handleJump();
		}
		else
		{
			updateXY();
		}
		if (!silent)
		{
			draw(MARIO);
		}
	}
	else
	{
		if (_jumpState > 0)
		{
			// if mario cant move during jump, hes not jumping anymore so we reset the _jumpState
			_jumpState = 0;
		}

		// if mario is falling, we count how many chars he falls
		else if (_jumpState == 0 && getBoard()->isEmpty(getX(), getY() + 1))
		{
			_fallCount++;
		}
		else
		{
			setDirX(0);
			setDirY(0);
			_lastDir = eKeys::STAY_L;
		}

	}
}


void Mario::handleJump()
{
	//if mario is in the first two moves of jump, means going up
	if (_jumpState == 1 || _jumpState == 2)
	{
		setDirX(_jumpDirX);
		setDirY(-1);
		//_pos.updateXY(_jumpDirX, -1); // update his location according to the directions
		updateXY();
	}
	//if mario is in the last two moves of jump, means going down
	else if (_jumpState == 3 || _jumpState == 4)
	{
		setDirX(_jumpDirX);
		setDirY(1);
		//_pos.updateXY(_jumpDirX, 1); // update his location according to the directions
		updateXY();
		_fallCount = 0;
	}

	_jumpState++;

	//it means we finished the jump and need to reset the moves
	if (_jumpState > 4)
	{
		_jumpState = 0;

		//we want mario to keep going in the direction he started the jump, so we change the last direction instead of up, to the previous one according to _jumpDirX
		if (_jumpDirX == -1)
		{
			_lastDir = eKeys::LEFT_L;
		}
		else if (_jumpDirX == 1)
		{
			_lastDir = eKeys::RIGHT_L;
		}
		else
		{
			_lastDir = eKeys::STAY_L;
		}
	}
}
