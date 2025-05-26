#pragma once
//#include "Board.h"
#include "Configure.h"
class Board;
class Point
{
private:
	int _x = 0;
	int _y = 0;
	int _dirX = 0;
	int _dirY = 0;
	Board* pBoard = nullptr;
public:
	void init(int x, int y, int dirX = 0, int dirY = 0, Board* board = nullptr);
	void draw(char ch) const;
	void erase() const;
	int getX() const { return _x; }
	int getY() const { return _y; }
	int getDirX() const { return _dirX; }
	int getDirY() const { return _dirY; }
	Board* getBoard() { return pBoard; }
	void setX(int x) { _x = x; }
	void setY(int y) { _y = y; }
	void setDirX(int dirX) { _dirX = dirX; }
	void setDirY(int dirY) { _dirY = dirY; }
	void setBoard(Board* board) { pBoard = board; }
	void updateXY() { _x += _dirX; _y += _dirY; }

};
