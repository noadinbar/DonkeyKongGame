#pragma once
#include "Windows.h"

static constexpr int GAME_WIDTH = 80;
static constexpr int GAME_HEIGHT = 25;
static constexpr int GAME_SIZE = 2000;
static constexpr int NOT_IN_BOARD = -1;
static constexpr char WALL = 'Q';
static constexpr char EQUAL_STAGE = '=';
static constexpr char LEFT_STAGE = '<';
static constexpr char RIGHT_STAGE = '>';
static constexpr char LADDER = 'H';
static constexpr char BARREL = 'O';
static constexpr char PAULINE = '$';
static constexpr char DONKEY_KONG = '&';
static constexpr char MARIO = '@';
static constexpr char GHOST = 'x';
static constexpr char SPECIAL_GHOST = 'X';
static constexpr char HAMMER = 'p';
static constexpr char SPACE = ' ';
static constexpr char LEGEND = 'L';
static constexpr int DISTANCE_EXPLODE = 2;
static constexpr int MAX_FALL_MARIO = 5;
static constexpr int MAX_FALL_BARREL = 8;
static constexpr int MAX_BARRELS = 10;
static constexpr char EXIT_GAME = '9';
static constexpr char INSTRUCTIONS = '8';
static constexpr char START_GAME = '1';
static constexpr char RETURN_MENU = '0';
static constexpr int INIT_LIVES = 3;
static constexpr int BARRELS_PACE = 25;
static constexpr int SLEEP_LONG = 4000;
static constexpr int SLEEP_SHORT = 100;
static constexpr int SLEEP_SUPER_SHORT = 50;
static constexpr int DIVIDED = 100;
static constexpr int SCORE_PACE = 10;
static constexpr int RANDOM_CHANGE = 5;
static constexpr int RANDOM_LADDER = 60;

enum class eKeys
{
	LEFT_L = 'a', LEFT_U = 'A',
	RIGHT_L = 'd', RIGHT_U = 'D',
	UP_L = 'w', UP_U = 'W',
	DOWN_L = 'x', DOWN_U = 'X',
	STAY_L = 's', STAY_U = 'S',
	HAMMER_L = 'p', HAMMER_U = 'P',
	ESC = 27
};

void gotoxy(int x, int y);
void ShowConsoleCursor(bool showFlag);
void clrscr();