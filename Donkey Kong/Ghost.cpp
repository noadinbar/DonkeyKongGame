#include "Ghost.h"
#include "Board.h"
#include <cstdlib>

//checks if the ghosts next move is valid
bool Ghost::canMove()
{
    bool stage = getBoard()->isStage(getX() + getDirX(), getY() + 1);
    bool wall = getBoard()->isWall(getX() + getDirX(), getY());

    if (stage && !wall)
    {
        return true;

    }
    return false;
}

//the paramter is for us to know if to print, if were not on automatic mode, we always send false
void Ghost::move(int random_value, bool silent)
{
    if (!silent)
    {
        draw(getBoard()->getChar(getX(), getY())); //returns what was on the board before mario
    }
    if (random_value < RANDOM_CHANGE)
    {
        setDirX(getDirX() * (-1));//change dir
    }
    if (canMove())
    {
        updateXY();
    }
    else
    {
        setDirX(getDirX() * (-1));
        if (canMove())
        {
            updateXY();
        }
    }
    if (!silent)
    {
        draw(GHOST);
    }
}



