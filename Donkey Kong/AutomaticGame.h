#pragma once
#include "GameBase.h"
#include "Steps.h"
#include "Result.h"
#include <array>       
#include <memory> 

using std::array;
using std::unique_ptr;
using std::make_unique;


class AutomaticGame : public GameBase
{
	int _stepsSize;
	int _resultsSize;
	int _stepsIteration;
	int _resultIteration;
	int _gameResult;
	Result _gameResults;
	bool _isSilent = false;
protected:
	//void handlePlayerInput(Mario& mario, Barrel* brlsArr[], bool& isGameOn) override;
	//void prepareNewGame(Mario& mario, Barrel* brlsArr[], bool& isGameOn) override;
public:
	void virtual run() override;
	bool getSilent() { return _isSilent; }
	void setSilent(bool state) { _isSilent = state; }
	void virtual startNewGame() override;
	void virtual startOver(Mario* pMario, array<unique_ptr<Barrel>, MAX_BARRELS>& arr, bool* isGameOn, bool nextLevel);

	bool loadFiles();
	bool canLoad();
	void printError(const string& error);
};

