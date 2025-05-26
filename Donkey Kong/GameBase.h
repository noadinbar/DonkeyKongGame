#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Barrel.h"
#include "Point.h"
#include "Configure.h"
#include "Board.h"
#include "Ghost.h"
#include "Steps.h"
#include "Result.h"
#include <array>       
#include <memory> 
using std::array;
using std::unique_ptr;
using std::make_unique;

//this is base class for all modes of game
class GameBase
{
	//we don't create objects of this specific class, so all this class members are protected and not private
protected:
	int _lives = INIT_LIVES;
	int _turn = 0;
	int _score = 0;
	int _level;
	long _randomSeed;
	Steps _gameSteps;
	string _filenamePrefix;
	string _stepsFileName;
	string _resultsFileName;
	vector<string> _fileNames;
	Board _board;
	bool _activeHammer = false;
	bool isMarioAround(Mario& mario, Barrel* pBrl); //if a barrel explodes, checking if mario is around 2 chars away, because it kills him
	bool moveAllBarrels(array<unique_ptr<Barrel>, MAX_BARRELS>& brlsArr, Mario& mario, bool silent);
	bool throwNewBarrel(array<unique_ptr<Barrel>, MAX_BARRELS>& brlsArr);
	bool moveAllGhosts(Mario& mario, int random_value, bool silent);
	void virtual startNewGame()=0;
	void resetBrls(array<unique_ptr<Barrel>, MAX_BARRELS>& brlsArr);;
	void updateLives() { _lives--; }
	void winGame() const;
	void printLivesAndScore() const;
	void destroy(array<unique_ptr<Barrel>, MAX_BARRELS>& brlsArr, Mario* pMario);
	void updateTurns() { _turn++; }
	void resetTurns() { _turn = 1; }
	void resetScore() { _score = 0; }
	//virtual void prepareNewGame(Mario& mario, Barrel* brlsArr[], bool& isGameOn) = 0;
	//void updateTurnAndBarrels(Barrel* brlsArr[]);
	//bool checkPaulineReached(Mario& mario, Barrel* brlsArr[], bool& isGameOn);
	//virtual void handlePlayerInput(Mario& mario, Barrel* brlsArr[], bool& isGameOn) = 0;

public:
	vector<string>& getFileNames() { return _fileNames; }
	void getAllBoardFileNames(vector<string>& vec_to_fill);
	void virtual run()=0;
	virtual void startOver(Mario* pMario, array<unique_ptr<Barrel>, MAX_BARRELS>& arr, bool* isGameOn, bool nextLevel) = 0;
	void createFiles();
};
