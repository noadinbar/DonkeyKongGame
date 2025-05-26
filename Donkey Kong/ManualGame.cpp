#include "ManualGame.h"
#include <conio.h> 
#include <iostream>
#include <Windows.h> 
#include <list>
#include <filesystem>
#include <fstream>

char ManualGame::displayMenu()
{
	clrscr();
	ShowConsoleCursor(false);

	for (int i = 0; i < GAME_HEIGHT - 1; i++)
	{
		cout << headLine[i] << '\n';
	}
	cout << headLine[GAME_HEIGHT - 1];

	gotoxy(GAME_WIDTH / 2 - 2, GAME_HEIGHT / 2 - 3);
	cout << "Menu" << endl;
	gotoxy(GAME_WIDTH / 3, GAME_HEIGHT / 2 - 2);
	cout << "(1) Start a new game" << endl;
	gotoxy(GAME_WIDTH / 3, GAME_HEIGHT / 2 - 1);
	cout << "(8) Present instructions and keys" << endl;
	gotoxy(GAME_WIDTH / 3, GAME_HEIGHT / 2);
	cout << "(9) EXIT" << endl;
	char choice = _getch();
	return choice;

}

char ManualGame::instructions() const
{
	clrscr();
	gotoxy(GAME_WIDTH / 2, 0);
	cout << "Instructions" << endl;
	cout << "Your goal is to navigate the levels," << endl;
	cout << "avoid hazards like barrels and fireballs, " << endl;
	cout << "and finally, reach the top to rescue Pauline." << endl;
	cout << "Use precise timing to jump over obstacles," << endl;
	cout << "and climb strategically to avoid danger." << endl;
	cout << "You do that by pressing the keys:" << endl;
	cout << "a/A for going left" << endl;
	cout << "d/D for going right" << endl;
	cout << "s/S to stay in place" << endl;
	cout << "w/W to jump or going up the ladders" << endl;
	cout << "x/X for going down" << endl;
	cout << "p/P for killing with hammer ghosts and barrels" << endl;
	cout << "Press ESC to pause the game" << endl;
	cout << "Good luck!" << endl;
	cout << "" << endl;
	cout << "Press 0 to return to menu" << endl;
	char userKey = _getch();
	while (userKey != RETURN_MENU)
	{
		userKey = _getch();
	}
	return userKey;
}

void ManualGame::printLevelsOptions()
{
	clrscr();
	char choice;

	cout << "Choose the screen:" << endl;
	for (int j = 0; j < _fileNames.size(); j++)
	{
		cout << (j + 1) << ". " << _fileNames[j] << endl;
	}
	choice = _getch();
	//we block the player from typing any key that is not compatible for the choice
	while (true)
	{
		if (choice - '0' <= _fileNames.size())
		{
			_level = choice - '0' - 1;
			break;
		}
		choice = _getch();
	}
}

void ManualGame::run()
{
	char choice = displayMenu();
	while (choice != EXIT_GAME)
	{
		if (choice == START_GAME)
		{
			if (_fileNames.size() == 0)
			{
				clrscr();
				cout << "There are no files to load. Try again :(" << endl;
				Sleep(SLEEP_LONG);
				break;
			}
			startNewGame();
		}
		else if (choice == INSTRUCTIONS)
		{
			choice = instructions();
		}
		choice = displayMenu();
	}
	clrscr();
}

void ManualGame::pauseGame() const
{
	gotoxy(_board.getlegendX(), _board.getlegendY());
	cout << "    Game paused    ";
	char pressed = 0;

	//ignore all keys pressed if its not ESC
	while (true)
	{
		if (_kbhit())
		{
			pressed = _getch();
		}
		if ((eKeys)pressed == eKeys::ESC)
		{
			return;
		}
	}

}

void ManualGame::startNewGame()
{
	printLevelsOptions();
	char pressedkey = 0;
	bool isGameOn = true;
	array<unique_ptr<Barrel>, MAX_BARRELS> brlsArr;
	Mario mario;
	resetScore();
	startOver(&mario, brlsArr, &isGameOn, true);

	while (isGameOn == true)
	{
		updateTurns();
		if (_turn % BARRELS_PACE == 0) //sets the pace when a new barrel gets in
		{
			throwNewBarrel(brlsArr);
		}

		if (_kbhit()) //if the user pressed on keyboard
		{
			pressedkey = _getch();

			//if pressedkey is valid char
			if ((eKeys)pressedkey == eKeys::STAY_U || (eKeys)pressedkey == eKeys::STAY_L ||
				(eKeys)pressedkey == eKeys::UP_U || (eKeys)pressedkey == eKeys::UP_L ||
				(eKeys)pressedkey == eKeys::DOWN_U || (eKeys)pressedkey == eKeys::DOWN_L ||
				(eKeys)pressedkey == eKeys::LEFT_U || (eKeys)pressedkey == eKeys::LEFT_L ||
				(eKeys)pressedkey == eKeys::RIGHT_U || (eKeys)pressedkey == eKeys::RIGHT_L)
			{
				_gameSteps.addStep(_turn, pressedkey); //we add keypressed and iteration to steps list

			}
			if ((eKeys)pressedkey == eKeys::ESC)
			{
				pauseGame();
				gotoxy(mario.getX(), mario.getY());
			}
			else if (mario.getFallCount() > 0)
			{
				mario.move(eKeys::DOWN_L, false);
			}
			//if we press p to use hammer, we check if mario changes direction before continuing with the game
			else if (_activeHammer && mario.getDir() != eKeys::STAY_L && mario.getDir() != eKeys::STAY_U &&
				((eKeys)pressedkey == eKeys::HAMMER_L || (eKeys)pressedkey == eKeys::HAMMER_U))
			{
				_gameSteps.addStep(_turn, pressedkey); //we add keypressed and iteration to steps list
				destroy(brlsArr, &mario);
				if (_kbhit())
				{
					if ((eKeys)pressedkey == eKeys::STAY_U || (eKeys)pressedkey == eKeys::STAY_L ||
						(eKeys)pressedkey == eKeys::UP_U || (eKeys)pressedkey == eKeys::UP_L ||
						(eKeys)pressedkey == eKeys::DOWN_U || (eKeys)pressedkey == eKeys::DOWN_L ||
						(eKeys)pressedkey == eKeys::LEFT_U || (eKeys)pressedkey == eKeys::LEFT_L ||
						(eKeys)pressedkey == eKeys::RIGHT_U || (eKeys)pressedkey == eKeys::RIGHT_L)
					{
						_gameSteps.addStep(_turn, pressedkey); //we add keypressed and iteration to steps list

					}

					pressedkey = _getch();
					if ((eKeys)pressedkey == eKeys::ESC)
					{
						pauseGame();
						gotoxy(mario.getX(), mario.getY());
					}
					else
					{
						mario.move((eKeys)pressedkey, false);
					}
				}
				else
				{
					mario.move(mario.getDir(), false);
				}
			}
			else
			{
				mario.move((eKeys)pressedkey, false);
			}
		}
		else
		{
			mario.move(mario.getDir(), false); //keep moving in the same direction
		}

		if (_board.isPauline(mario.getX(), mario.getY()))
		{
			_resultsFile.addResult(_turn, Result::finished); //we add game result and iteration to results list
			saveFiles();
			//if we are in the last level- we finished the game
			if (_level == _fileNames.size() - 1)
			{
				winGame();
				isGameOn = false; //game ends
			}
			else
			{
				_level++;
				startOver(&mario, brlsArr, &isGameOn, true);
				continue;
			}
		}

		int random_value = rand() % DIVIDED;
		bool isMarioAliveFromBrl = moveAllBarrels(brlsArr, mario, false);
		bool isMarioAliveFromGst = moveAllGhosts(mario, random_value, false);

		if (!isMarioAliveFromBrl || !isMarioAliveFromGst)
		{
			_resultsFile.addResult(_turn, Result::died); //we add game result and iteration to results list
			updateLives();
			clrscr();
			if (_lives > 0)
			{
				resetScore();
				startOver(&mario, brlsArr, &isGameOn, false);
			}
			else
			{
				isGameOn = false;
				clrscr();
				cout << "You lost the game";
				Sleep(SLEEP_LONG);
			}
		}

		if (isGameOn == true)
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
					_resultsFile.addResult(_turn, Result::died); //we add game result and iteration to results list
					updateLives();
					if (_lives > 0)
					{
						resetScore();
						startOver(&mario, brlsArr, &isGameOn, false);
					}
					else
					{
						isGameOn = false;
						clrscr();
						cout << "You lost the game";
						Sleep(SLEEP_LONG);
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
			Sleep(SLEEP_SHORT);

			if (_turn % SCORE_PACE == 0)
			{
				_score++;
			}
			printLivesAndScore();
		}
	}

	saveFiles();
	//prepare for next game
	_lives = INIT_LIVES;
	_board.deleteGhost();
}

void ManualGame::startOver(Mario* pMario, array<unique_ptr<Barrel>, MAX_BARRELS>& arr, bool* isGameOn, bool nextLevel)
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
		_resultsFile.getResults().clear(); // Clear previous results
		createFiles();
		_randomSeed = static_cast<long>(chrono::system_clock::now().time_since_epoch().count());
		_gameSteps.setRandomSeed(_randomSeed);
		srand(_randomSeed);
	}
	_board.assignBoard();
	_board.reset();
	_board.print();
	pMario->init(_board.getMarioX(), _board.getMarioY(), 0, 0, &_board);
	pMario->draw(MARIO);
	pMario->setDir(eKeys::STAY_L);
}

void ManualGame::saveFiles()
{
	_gameSteps.saveSteps(_stepsFileName);
	_resultsFile.setScore(_score);
	_resultsFile.saveResults(_resultsFileName);
}
