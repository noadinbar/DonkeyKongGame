#include "AutomaticGame.h"
#include <conio.h> 
#include <iostream>
#include <Windows.h> 
#include <list>
#include <filesystem>
#include <fstream>

using namespace std;

void AutomaticGame::run()
{
	startNewGame();
}

void AutomaticGame::startNewGame()
{
	bool isGameOn = true;
	
	std::array<std::unique_ptr<Barrel>, MAX_BARRELS> brlsArr;

	Mario mario;
	resetScore();
	startOver(&mario, brlsArr, &isGameOn, true);
	char pressedkey;

	while (isGameOn == true)
	{
		updateTurns();
		if (_turn % BARRELS_PACE == 0) //sets the pace when a new barrel gets in
		{
			throwNewBarrel(brlsArr);
		}

		if (_turn == _stepsIteration) //if we reached a turn that in the steps file the user pressed a key
		{
			if (_stepsSize > 0)
			{
				pressedkey = _gameSteps.getSteps().front().second; //we get the key that needs to be in this iteration
				_gameSteps.getSteps().pop_front(); //we dont need the information about this iteration anymore
				_stepsSize--;
				if (_stepsSize > 0) //only if there are more steps in file, we take the next iteration
					_stepsIteration = static_cast<int>(_gameSteps.getSteps().front().first);

			}

			if (mario.getFallCount() > 0)
			{
				mario.move(eKeys::DOWN_L, _isSilent);
			}

			//if p was pressed, we check if mario changes direction before continuing with the game
			else if (_activeHammer && mario.getDir() != eKeys::STAY_L && mario.getDir() != eKeys::STAY_U &&
				((eKeys)pressedkey == eKeys::HAMMER_L || (eKeys)pressedkey == eKeys::HAMMER_U))
			{
				destroy(brlsArr, &mario);

				//if we use hammer and move on the same turn
				if (_stepsIteration == _turn)
				{
					if (_stepsSize > 0)
					{
						pressedkey = _gameSteps.getSteps().front().second; //we get the key that needs to be in this iteration
						_gameSteps.getSteps().pop_front(); //we dont need the information about this iteration anymore
						_stepsSize--;
						if (_stepsSize > 0) //only if there are more steps in file, we take the next iteration
							_stepsIteration = static_cast<int>(_gameSteps.getSteps().front().first);
					}
					mario.move((eKeys)pressedkey, _isSilent);
				}
				else
				{
					mario.move(mario.getDir(), _isSilent);
				}
			}
			else
			{
				mario.move((eKeys)pressedkey, _isSilent);
			}
		}

		//from here we keep running with the step iteration and keys we take from steps file
		else
		{
			mario.move(mario.getDir(), _isSilent); //keep moving in the same direction
		}

		//mario reached Pauline, we check if it was supposed to happen
		if (_board.isPauline(mario.getX(), mario.getY()) && _turn == _resultIteration && _gameResult == Result::ResultValue::finished)
		{
			_gameResults.getResults().pop_front();
			_resultsSize--;
			if (_resultsSize != 0)
			{
				printError("There are more results in file then needed");
				break;
			}

			//if we are in the last level- we finished the game
			if (_level == _fileNames.size() - 1)
			{
				winGame();
				isGameOn = false; //game ends
				break;
			}
			else
			{
				_level++;
				startOver(&mario, brlsArr, &isGameOn, true);
				continue;
			}
		}
		else
		{
			//if mario didn't reach Pauline but according to steps and results file he was supposed to
			if (!_board.isPauline(mario.getX(), mario.getY()) && _turn == _resultIteration && _gameResult == Result::ResultValue::finished)
			{
				printError("Mario was supposed to reach Pauline but he didn't, game stops. ");
				break;
			}
			else if (_board.isPauline(mario.getX(), mario.getY()))
			{
				if (_gameResult != Result::ResultValue::finished)
				{
					printError("Mario was supposed to die but he finished the level, game stops.");
					break;
				}
				else if (_turn != _resultIteration)
				{
					printError("Mario reached Pauline in the wrong time, game stops.");
					break;
				}
			}
		}
		int random_value = rand() % DIVIDED;
		bool isMarioAliveFromBrl = moveAllBarrels(brlsArr, mario, _isSilent);
		bool isMarioAliveFromGst = moveAllGhosts(mario, random_value, _isSilent);

		if (!isMarioAliveFromBrl || !isMarioAliveFromGst)
		{
			if (_turn == _resultIteration && _gameResult == Result::ResultValue::died)
			{
				_gameResults.getResults().pop_front();
				_resultsSize--;
				if (_resultsSize > 0) //only if there are more results in file, we take the next iteration+result
				{
					_resultIteration = static_cast<int>(_gameResults.getResults().front().first);
					_gameResult = _gameResults.getResults().front().second;
				}

				updateLives();
				clrscr();

				if (_lives > 0)
				{
					if (_resultsSize == 0)
					{
						printError("There are less results in file then needed");
						break;
					}
					resetScore();
					startOver(&mario, brlsArr, &isGameOn, false);
				}
				else
				{
					isGameOn = false;
					clrscr();
					if (_resultsSize != 0)
					{
						printError("There are more results in file then needed");
						break;
					}
					cout << "Mario lost the game";
					Sleep(SLEEP_LONG);
				}
			}
			else
			{
				if (_gameResult != Result::ResultValue::died)
				{
					printError("Mario was supposed to die but you didnt, game stops.");
					break;
				}
				else if (_turn != _resultIteration)
				{
					printError("Mario died in the wrong time, game stops.");
					break;
				}
			}
		}

		if (isGameOn)
		{
			//mario is falling
			if (_board.isEmpty(mario.getX(), mario.getY() + 1))
			{
				mario.setFallCount(mario.getFallCount() + 1);
				mario.setDir(eKeys::DOWN_L);
			}
			else
			{
				if (mario.getFallCount() >= MAX_FALL_MARIO)
				{
					if (_turn == _resultIteration && _gameResult == Result::ResultValue::died)
					{
						_gameResults.getResults().pop_front();
						_resultsSize--;
						if (_resultsSize > 0) //only if there are more results in file, we take the next iteration+result
						{
							_resultIteration = static_cast<int>(_gameResults.getResults().front().first);
							_gameResult = _gameResults.getResults().front().second;
						}

						updateLives();

						if (_lives > 0)
						{
							if (_resultsSize == 0)
							{
								printError("There are less results in file then needed");
								break;
							}
							resetScore();
							startOver(&mario, brlsArr, &isGameOn, false);
						}
						else
						{
							isGameOn = false;
							clrscr();
							if (_resultsSize != 0)
							{
								printError("There are more results in file then needed");
								break;
							}
							cout << "Mario lost the game";
							Sleep(SLEEP_LONG);
						}
					}
					else
					{
						if (_gameResult != Result::ResultValue::died)
						{
							printError("Mario was supposed to die but you didnt, game stops.");
							break;
						}
						else if (_turn != _resultIteration)
						{
							printError("Mario died in the wrong time, game stops.");
							break;
						}
					}
				}
				else
				{
					//if mario is done falling and didnt die, we reset the fall count
					mario.setFallCount(0);
				}
			}
			//if mario collects the hammer, we activate it so he can use it
			if (_board.getChar(mario.getX(), mario.getY()) == HAMMER && !_activeHammer)
			{
				_activeHammer = true;
				_board.eraseHammer(mario.getX(), mario.getY());
			}

			if (_turn % SCORE_PACE == 0)
			{
				_score++;
			}
			//if we load the game without -silent, we activate sleep and print legend
			if (!_isSilent)
			{
				printLivesAndScore();
				Sleep(SLEEP_SUPER_SHORT);
			}
		}
	}

	//prepare for next game
	_lives = INIT_LIVES;
	_board.deleteGhost();

}

void AutomaticGame::startOver(Mario* pMario, array<unique_ptr<Barrel>, MAX_BARRELS>& arr, bool* isGameOn, bool nextLevel)
{
	resetBrls(arr);
	_board.deleteGhost();
	_activeHammer = false;
	pMario->setFallCount(0);
	clrscr();
	ShowConsoleCursor(false);
	//until we went through all levels, we try to load a level
	while (_level <= _fileNames.size())
	{
		if (_board.load(_fileNames[_level]))
		{
			break;
		}
		_level++;
	}

	//if there are no more levels- we stop the game
	if (_level == _fileNames.size())
	{
		*isGameOn = false;
		return;
	}

	if (nextLevel)
	{
		resetTurns();
		_gameSteps.getSteps().clear(); // Clear previous steps
		_gameResults.getResults().clear(); // Clear previous results

		if (!loadFiles())
		{
			*isGameOn = false;
			printError("Results\\steps file missing, can't load game.");
			return;
		}
		//only if there are results, we load the first iteration+result
		if (_resultsSize > 0 && _stepsSize > 0)
		{
			_stepsIteration = static_cast<int>(_gameSteps.getSteps().front().first);
			_resultIteration = static_cast<int>(_gameResults.getResults().front().first);
			_gameResult = _gameResults.getResults().front().second;
		}
		srand(_gameSteps.getRandomSeed()); //we set the random from the steps file we got so it will run the same as loaded

	}

	if (_gameSteps.getSteps().empty() || _gameResults.getResults().empty())
	{
		printError("The game can't run because one of the files is empty.");
		*isGameOn = false;
		return;
	}

	_board.assignBoard();
	_board.reset();
	pMario->init(_board.getMarioX(), _board.getMarioY(), 0, 0, &_board);

	//if we load the game without -silent, we print the board and mario
	if (!_isSilent)
	{
		_board.print();
		pMario->draw(MARIO);
	}
	pMario->setDir(eKeys::STAY_L);
}

bool AutomaticGame::loadFiles()
{
	createFiles();
	if (canLoad())
	{
		_gameSteps.loadSteps(_stepsFileName);
		_stepsSize = static_cast<int>(_gameSteps.getSteps().size());
		_gameResults.loadResults(_resultsFileName);
		_resultsSize = static_cast<int>(_gameResults.getResults().size());
		return true;
	}
	else
	{
		return false;
	}
}

//we check if the specific files can be loaded
bool AutomaticGame::canLoad()
{
	bool stepsFound = false, resultsFound = false;
	namespace fs = filesystem;
	for (const auto& entry : fs::directory_iterator(fs::current_path()))
	{
		auto filename = entry.path().filename();
		auto filenameStr = filename.string();
		if (filenameStr == _stepsFileName)
		{
			stepsFound = true;
		}
		if (filenameStr == _resultsFileName)
		{
			resultsFound = true;
		}
	}
	return resultsFound && stepsFound;
}

void AutomaticGame::printError(const string& error)
{
	clrscr();
	cout << error << endl << endl;
}
