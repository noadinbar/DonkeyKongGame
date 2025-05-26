#pragma once
#include "GameBase.h"
#include "Steps.h"
#include <string>
#include "Result.h"
#include <array>      
#include <memory>     
using std::array;
using std::unique_ptr;
using std::make_unique;


class ManualGame : public GameBase
{
	const char* headLine[GAME_HEIGHT] = {
		// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
		  "          ____   __   __ _  __ _  ____  _  _    __ _  __   __ _   ___           ",
		  "         (    \\ /  \\ (  ( \\(  / )(  __)( \\/ )  (  / )/  \\ (  ( \\ / __)          ",
		  "          ) D ((  O )/    / )  (  ) _)  )  /    )  ((  O )/    /( (_ \\          ",
		  "         (____/ \\__/ \\_)__)(__\\_)(____)(__/    (__\\_)\\__/ \\_)__) \\___/          ",
		  "                                                                                ", // 4
		  "                                                                                ", // 5
		  "                                                                                ", // 6
		  "                                                                                ", // 7
		  "                                                                                ", // 8
		  "                                                                                ", // 9
		  "                                                                                ", // 11
		  "                                                                                ", // 11
		  "                                                                                ", // 12
		  "                                                                                ", // 13
		  "                                                                                ", // 14
		  "                                                                                ", // 15
		  "                                                                                ", // 16
		  "                                                                                ", // 17
		  "                                                                                ", // 18
		  "                                                                                ", // 19
		  "                                                                                ", // 20
		  "                                                                                ", // 21
		  "                                                                                ", // 22
		  "                                                                                ", // 23
		  "                                                                                "  // 24
	};
	Result _resultsFile;
protected:
	//void handlePlayerInput(Mario& mario, Barrel* brlsArr[], bool& isGameOn) override;
	//void prepareNewGame(Mario& mario, Barrel* brlsArr[], bool& isGameOn) override;
public:
	char displayMenu();
	void printLevelsOptions();
	void pauseGame() const;
	char instructions() const;
	void virtual run();
	void virtual startNewGame() override;
	void virtual startOver(Mario* pMario, array<unique_ptr<Barrel>, MAX_BARRELS>& arr, bool* isGameOn, bool nextLevel);

	void saveFiles();
};