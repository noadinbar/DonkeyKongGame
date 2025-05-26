#include "SpecialGhost.h"
#include "Board.h"

//the paramter is for us to know if to print, if were not on automatic mode, we always send false
void SpecialGhost::move(int random_value, bool silent)
{
    if (!silent)
    {
        draw(getBoard()->getChar(getX(), getY())); //returns what was on the board before ghost
    }
    if (!_isClimbing)
    {
        if (random_value < RANDOM_CHANGE)
        {
            setDirX(getDirX() * (-1));//change dir
        }
        if (canMove(random_value))
        {
            updateXY();
        }
        else
        {
            setDirX(getDirX() * (-1));
            if (canMove(random_value))
            {
                updateXY();
            }
        }
    }
    else
    {
        if (getBoard()->isStage(getX() + getDirX(), getY()))
        {
            setDirY(-1);
            setDirX(0);
            _doneClimbing = true;
        }
        else if (_doneClimbing)
        {
            _isClimbing = false;
            _doneClimbing = false;
            setDirY(0);
            setDirX(1); //we decided that special ghost will always move right after finishing climbing
        }
        else
        {
            setDirY(-1);
            setDirX(0);
        }
        updateXY();
    }
    if (!silent)
    {
        draw(SPECIAL_GHOST);
    }
}

bool SpecialGhost::canMove(int random)
{
    bool stage = getBoard()->isStage(getX() + getDirX(), getY() + 1);
    bool wall = getBoard()->isWall(getX() + getDirX(), getY());
    bool ladder = getBoard()->isLadder(getX() + getDirX(), getY());

    if (ladder && random < RANDOM_LADDER)
    {
        _isClimbing = true;
        return true;
    }

    else if (stage && !wall)
    {
        return true;
    }
    return false;
}


