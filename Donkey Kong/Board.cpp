#include "Board.h"
#include "Configure.h"
#include "SpecialGhost.h"
#include <iostream>
#include <fstream>
#include <algorithm>   
using namespace std;





//from class with Amir
void Board::reset()
{
    for (int i = 0; i <= GAME_HEIGHT; i++)
    {
        memcpy(currentBoard[i], originalBoard[i], GAME_WIDTH + 1);
    }

}

void Board::setErrVec()
{
    _errors.push_back({ "no mario in file" ,"more than one mario",0 });
    _errors.push_back({ "no donkey kong in file","more than one donkey kong" ,0 });
    _errors.push_back({ "no pauline in file" ,"more than one pauline",0 });
    _errors.push_back({ "no legend in file" ," ",0 });
}


bool Board::load(const string& fileToOpen)
{
    ifstream file(fileToOpen);
    setErrVec();
    if (!file)
    {
        cout << "Error: Could not open file " << fileToOpen << endl;
    }
    string line;
    int row = 0;
    _ghostVec.reserve(GAME_SIZE); //we reserve the max places for ghosts on board
    _legendPlace.init(0, 0);

    //we copy the board from file to our array
    //when we get special characters, we treat them as needed
    //also, we check if there is all the characters we need or too many
    while (getline(file, line) && row < GAME_HEIGHT)
    {
        for (int col = 0; col < GAME_WIDTH; ++col)
        {
            if (col < line.length())
            {
                originalBoard[row][col] = line[col];
                if (originalBoard[row][col] == MARIO)
                {
                    if (_errors[0].count == 0)
                    {
                        _marioStart.init(col, row, 0, 0, nullptr);
                    }
                    _errors[0].count++;
                    originalBoard[row][col] = SPACE;
                }
                else if (originalBoard[row][col] == DONKEY_KONG)
                {
                    if (_errors[1].count == 0)
                    {
                        _donkeyPlace.init(col, row, 0, 0, nullptr);
                    }
                    _errors[1].count++;
                }
                else if (originalBoard[row][col] == GHOST)
                {
                    originalBoard[row][col] = SPACE;
                    _ghostVec.push_back(make_unique<Ghost>(col, row));
                }
                else if (originalBoard[row][col] == SPECIAL_GHOST)
                {
                    originalBoard[row][col] = SPACE;
                    _ghostVec.push_back(make_unique<SpecialGhost>(col, row));
                }
                else if (originalBoard[row][col] == PAULINE)
                {
                    _errors[2].count++;
                }
                else if (originalBoard[row][col] == LEGEND)
                {
                    if (_errors[3].count == 0)
                    {
                        originalBoard[row][col] = SPACE;
                        _legendPlace.init(col, row);
                    }
                    _errors[3].count++;
                }
                //if its not a valid char, we erase it so it wont bother the game
                else if (!isCharValid(originalBoard[row][col]))
                {
                    originalBoard[row][col] = SPACE;
                }
            }
            else
            {
                originalBoard[row][col] = SPACE; // fill in spaces where needed
            }
        }
        ++row;
    }

    // fill in lines if the file has less lines than expected
    for (int r = row; r < GAME_HEIGHT; ++r)
    {
        for (int c = 0; c < GAME_WIDTH; ++c)
        {
            originalBoard[r][c] = SPACE;
        }
    }
    // we check if the ghosts is in valid position, if not- we erase it so it wont be an error
    // remove invalid-position ghosts
    _ghostVec.erase(
        remove_if(
            _ghostVec.begin(),
            _ghostVec.end(),
            [&](const unique_ptr<Ghost>& g) {
                if (originalBoard[g->getY() + 1][g->getX()] == SPACE) {
                    originalBoard[g->getX()][g->getY()] = SPACE;
                    return true; 
                }
                return false; 
            }
        ),
        _ghostVec.end()
    );


    //we add one more line of stage below the file, so if the file doesnt have stage, all the characters will move
    for (int i = 0; i < GAME_WIDTH; i++)
    {
        originalBoard[GAME_HEIGHT][i] = EQUAL_STAGE;
    }

    bool printed = false;
    int countErrors = 0;
    //print the errors in file loaded- if there are any
    for (typesOfErrors& i : _errors)
    {
        if (i.count == 0)
        {
            if (!printed)
            {
                cout << '"' << fileToOpen << '"' << " couldn't load -" << endl;
                printed = true;
            }
            countErrors++;
            cout << i.reason1 << endl;
        }
        else if (i.count > 1)
        {
            if (!printed)
            {
                cout << '"' << fileToOpen << '"' << " couldn't load -" << endl;
                printed = true;
            }
            countErrors++;
            cout << i.reason2 << endl;
        }


    }

    file.close();
    _errors.clear(); //clearing the vector for the next file loaded

    if (countErrors > 0)
    {
        cout << "To sum up, " << countErrors << " error/s was found" << endl;
        Sleep(SLEEP_LONG);
        clrscr();
        return false;
    }

    return true;
}

void Board::print() const
{
    for (int row = 0; row < GAME_HEIGHT; ++row)
    {
        for (int col = 0; col < GAME_WIDTH; ++col)
        {
            cout << originalBoard[row][col];
        }
        if (row < GAME_HEIGHT - 1)
        {
            cout << endl;
        }

    }
}

void Board::deleteGhost()
{
    _ghostVec.clear();
}

void Board::assignBoard()
{
    for (const auto& g : getGhostVec())
    {
        if (g) 
        {
            g->setBoard(this);
        }
    }
}

bool Board::isWall(int x, int y) const
{
    if (x == GAME_WIDTH || x == NOT_IN_BOARD || y == NOT_IN_BOARD || y == GAME_HEIGHT || (currentBoard[y][x] == WALL))
    {
        return true;
    }
    return false;
}

//checks if the char we got is one of the expected characters
bool Board::isCharValid(char c)
{
    return (c == MARIO || c == DONKEY_KONG || c == PAULINE || c == GHOST ||
        c == SPACE || c == EQUAL_STAGE || c == LEFT_STAGE || c == RIGHT_STAGE || c == WALL
        || c == HAMMER || c == LEGEND || c == LADDER);
}
