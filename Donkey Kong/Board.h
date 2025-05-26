#pragma once
#include <iostream>
#include <string>
#include "Point.h"
#include "Configure.h"
#include <vector>
#include "Ghost.h"
#include <memory>
using namespace std;
using std::unique_ptr;
using std::make_unique;
using std::remove_if;
//easy way to sort and print the errors in files we get
struct typesOfErrors
{
	string reason1; //not enough
	string reason2; //too many
	int count = 0;
};

class Board
{
	char originalBoard[GAME_HEIGHT + 1][GAME_WIDTH + 1];
	char currentBoard[GAME_HEIGHT + 1][GAME_WIDTH + 1];
	Point _marioStart;
	Point _donkeyPlace;
	Point _legendPlace;
	vector<unique_ptr<Ghost>> _ghostVec;
	vector<typesOfErrors> _errors;

public:
	Board() = default;
	Board(const Board&) = delete;
	Board& operator=(const Board&) = delete;
	int getMarioX() const { return _marioStart.getX(); }
	int getMarioY()  const { return _marioStart.getY(); }
	int getDonkeyX() const { return _donkeyPlace.getX(); }
	int getDonkeyY() const { return _donkeyPlace.getY(); }
	int getlegendX() const { return _legendPlace.getX(); }
	int getlegendY() const { return _legendPlace.getY(); }
	vector<unique_ptr<Ghost>>& getGhostVec() { return _ghostVec; }
	void setDonkeyBoard(Board* board) { _donkeyPlace.setBoard(board); }
	void setErrVec();
	void assignBoard();
	void reset();
	bool isCharValid(char c);
	bool load(const string& fileToOpen);
	void print() const;
	void deleteGhost();
	void eraseHammer(int x, int y) { currentBoard[y][x] = SPACE; }
	//all this functions helps to check what is on the next move and manage each one
	bool isWall(int x, int y) const;
	bool isEqualStage(int x, int y) const { return (currentBoard[y][x] == EQUAL_STAGE); }
	bool isLeftStage(int x, int y) const { return (currentBoard[y][x] == LEFT_STAGE); }
	bool isRightStage(int x, int y) const { return (currentBoard[y][x] == RIGHT_STAGE); }
	bool isBarrel(int x, int y) const { return (currentBoard[y][x] == BARREL); }
	bool isPauline(int x, int y) const { return (currentBoard[y][x] == PAULINE); }
	bool isDonkeyKong(int x, int y) const { return (currentBoard[y][x] == DONKEY_KONG); }
	bool isLadder(int x, int y) const { return (currentBoard[y][x] == LADDER); }
	bool isEmpty(int x, int y) const { return (currentBoard[y][x] == SPACE); }
	bool isStage(int x, int y) const { return (currentBoard[y][x] == LEFT_STAGE || currentBoard[y][x] == RIGHT_STAGE || currentBoard[y][x] == EQUAL_STAGE || currentBoard[y][x] == WALL); }
	char getChar(int x, int y) const { return currentBoard[y][x]; }
};