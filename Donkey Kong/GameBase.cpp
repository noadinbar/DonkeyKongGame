#include <conio.h> 
#include <iostream>
#include <Windows.h> 
#include <fstream>
#include <filesystem>
#include "GameBase.h"
using namespace std;

void GameBase::createFiles()
{
	_filenamePrefix = _fileNames[_level].substr(0, _fileNames[_level].find_last_of('.'));
	_stepsFileName = _filenamePrefix + ".steps";
	_resultsFileName = _filenamePrefix + ".result";
}

void GameBase::getAllBoardFileNames(vector<string>& vec_to_fill)
{
	namespace fs = filesystem;
	for (const auto& entry : fs::directory_iterator(fs::current_path()))
	{
		auto filename = entry.path().filename();
		auto filenameStr = filename.string();
		if (filenameStr.substr(0, 5) == "dkong" && filename.extension() == ".screen") {
			vec_to_fill.push_back(filenameStr);
		}
	}
}

//checks if ghost exists and move it, checks if it hits mario.
//also if 2 ghosts is colliding, they change direction
bool GameBase::moveAllGhosts(Mario& mario, int random_value, bool silent)
{
	bool isMarioDead = false;
	for (int i = 0; i < _board.getGhostVec().size(); i++)
	{
		if (_board.getGhostVec()[i] != nullptr)
		{
			Ghost* ghostToCheck = _board.getGhostVec()[i].get();
			if (ghostToCheck->getX() == mario.getX() &&
				ghostToCheck->getY() == mario.getY())
			{
				isMarioDead = true;
			}
			ghostToCheck->move(random_value, silent);
			if (ghostToCheck->getX() == mario.getX() &&
				ghostToCheck->getY() == mario.getY())
			{
				isMarioDead = true;
			}
		}

	}

	for (int i = 0; i < _board.getGhostVec().size(); i++)
	{
		Ghost* ghostToCheck1 = _board.getGhostVec()[i].get();
		for (int j = i + 1; j < _board.getGhostVec().size(); j++)
		{
			Ghost* ghostToCheck2 = _board.getGhostVec()[j].get();
			if (ghostToCheck1 != nullptr && ghostToCheck2 != nullptr)
			{
				if (*ghostToCheck1 == *ghostToCheck2)
				{
					ghostToCheck1->setDirX(ghostToCheck1->getDirX() * (-1));
					ghostToCheck2->setDirX(ghostToCheck2->getDirX() * (-1));
				}
			}
		}
	}
	return !isMarioDead;
}

//checks if when mario uses hammer, a barrel or ghost is 1 or 2 chars away, if they are- we delete them from the board
void GameBase::destroy(array<unique_ptr<Barrel>, MAX_BARRELS>& brlsArr, Mario* pMario)
{
	int newX1 = pMario->getX() + pMario->getDirX();
	int newX2 = pMario->getX() + 2 * (pMario->getDirX());
	int newY = pMario->getY() + pMario->getDirY();

	for (int i = 0; i < MAX_BARRELS; i++)
	{
		if (brlsArr[i])
		{
			if ((newX1 == brlsArr[i]->getX() || newX2 == brlsArr[i]->getX()) && newY == brlsArr[i]->getY())
			{
				brlsArr[i]->erase();
				brlsArr[i].reset();       
			}
		}

	}


	for (int i = 0; i < _board.getGhostVec().size(); )
	{
		auto& ghostPtr = _board.getGhostVec()[i];  
		if (ghostPtr)  
		{
			Ghost* ghostToCheck = ghostPtr.get();  

			if ((ghostToCheck->getX() == newX1 || ghostToCheck->getX() == newX2) &&
				ghostToCheck->getY() == newY)
			{
				ghostToCheck->erase();
				_board.getGhostVec().erase(_board.getGhostVec().begin() + i);  
			}
			else
			{
				i++;
			}
		}
		else
		{
			i++;
		}
	}

}

void GameBase::resetBrls(array<unique_ptr<Barrel>, MAX_BARRELS>& brlsArr)
{
	for (auto& barrel : brlsArr)
	{
		barrel.reset();
	}
}

bool GameBase::moveAllBarrels(array<unique_ptr<Barrel>, MAX_BARRELS>& brlsArr, Mario& mario, bool silent)
{
	bool isMarioDead = false;
	for (int i = 0; i < MAX_BARRELS; i++)
	{
		if (brlsArr[i] != nullptr)
		{
			//this check is because we just moved mario so lets see if he was moved on a barrel
			if (brlsArr[i]->getX() == mario.getX() &&
				brlsArr[i]->getY() == mario.getY())
			{
				isMarioDead = true;
			}
			else
			{
				bool isexploded = brlsArr[i]->move(silent); //checks if barrel needs to explode
				if (isexploded)
				{
					if (isMarioAround(mario, brlsArr[i].get()))
					{
						isMarioDead = true;
					}
					brlsArr[i].reset();  
				}
				//after the barrel moved, if it meets mario
				else if (brlsArr[i]->getX() == mario.getX() &&
					brlsArr[i]->getY() == mario.getY())
				{
					isMarioDead = true;
				}
			}
		}
	}

	return !isMarioDead;   //returns true if mario is alive
}

bool GameBase::throwNewBarrel(array<unique_ptr<Barrel>, MAX_BARRELS>& brlsArr)
{
	for (int i = 0; i < MAX_BARRELS; i++)
	{
		if (!brlsArr[i])
		{
			brlsArr[i] = make_unique<Barrel>(_board.getDonkeyX(), _board.getDonkeyY());
			brlsArr[i]->setBoard(&_board);
			return true;
		}
	}
	return false;
}

bool GameBase::isMarioAround(Mario& mario, Barrel* pBrl)
{
	int distanceX, distanceY;
	distanceX = pBrl->getX() - mario.getX();
	distanceY = pBrl->getY() - mario.getY();

	if (distanceX <= DISTANCE_EXPLODE && distanceX >= -DISTANCE_EXPLODE && distanceY <= DISTANCE_EXPLODE && distanceY >= -DISTANCE_EXPLODE)
	{
		return true;
	}
	return false;
}

void GameBase::winGame() const
{
	clrscr();
	cout << "You saved Pauline, you are a hero!!!";
	Sleep(SLEEP_LONG);
}

void GameBase::printLivesAndScore() const
{
	gotoxy(_board.getlegendX(), _board.getlegendY());
	cout << "L: " << _lives << " Score: " << _score;
	if (_activeHammer)
	{
		//that is our hint so player knows he have hammer to use
		cout << " | P";
	}
}