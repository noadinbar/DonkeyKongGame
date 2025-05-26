#include "GameManager.h"
#include "ManualGame.h"
#include "AutomaticGame.h"
#include <iostream>
#include <conio.h>

using namespace std;

void GameManager::run() 
{
    clrscr();
    cout << "Welcome to Donkey Kong!" << endl;
    cout << "(1) Start regular Game" << endl;
    cout << "(2) Watch previous game" << endl;
    //cout << "(3) Watch previous results" << endl;
    cout << "(0) Exit" << endl;

    char choice = _getch();
    GameBase* game = nullptr;

    switch (choice) 
    {
    case '1':
        game = new ManualGame();
        break;
    case '2':
        game = new AutomaticGame();
        break;
    case '0':
        return;
    default:
        cout << "Invalid option!" << endl;
        return;
    }

    game->getAllBoardFileNames(game->getFileNames());
    game->run();
    delete game;
}