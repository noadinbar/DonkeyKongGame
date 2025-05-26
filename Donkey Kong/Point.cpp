#include "Point.h"
#include <Windows.h>
#include "Configure.h"
#include <iostream>
using namespace std;

void Point::init(int x, int y, int dirX, int dirY, Board* board)
{
    _x = x;
    _y = y;
    _dirX = dirX;
    _dirY = dirY;
    pBoard = board;

}
void Point::draw(char ch) const
{
    gotoxy(_x, _y);
    cout << ch;
}

void Point::erase() const
{
    gotoxy(_x, _y);
    cout << " ";
}


