// game.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile main.cpp for this project

#include "game.h"
#include "neuralNetwork.h"

#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <algorithm>
using std::reverse;
using std::replace;
using std::ostringstream;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <sys/stat.h> // for mkdir
#include <sys/types.h>

Game::Game(NeuralNetwork &redPlayer, NeuralNetwork &blackPlayer)
{
    _redPlayerPtr = &redPlayer;
    _blackPlayerPtr = &blackPlayer;
    
    _redPlayerPtr->_isRed = true;
    _blackPlayerPtr->_isRed = false;
}

void Game::writeGameHistoryToFile(const string subdirectory, string weightFilename)
{
    struct stat info;

    if(stat(subdirectory.c_str(), &info) != 0)
    {
        cout << "Cannot access: " << subdirectory << endl;
    }
    else if(!(info.st_mode & S_IFDIR))
    {
        cout << subdirectory << " is not a directory." << endl;
    }
    
    // make the directory if it doesn't already exist
    const int dir_err = mkdir(subdirectory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    ostringstream stringStream;
    stringStream << subdirectory << "/" << weightFilename << ".txt";
    string fileName = stringStream.str();
    
    cout << "\nAttempting to write to file: " << fileName << endl;
    
    ofstream file(fileName, ofstream::out | ofstream::trunc);

    if (file.is_open())
    {
        for(auto i : _gameHistory) 
        {
            file << i << "\n";
        }
        file.close();
    }
    else
    {
        cout << "Could not open: " << fileName << endl;
    }
}

void Game::gameLoop()
{
    string win = "";
    int turns = 0;
    bool redPlayerTurn = true; // red goes first
    _currentBoard = "rrrrrrrrrrrr________bbbbbbbbbbbb"; // starting board
    
    while (win == "" || turns <= 100)
    {
        if(redPlayerTurn)
        {
            _currentBoard = _redPlayerPtr->treeSearch(_currentBoard, 4);
            if(_currentBoard == "" || turns >= 100)
            {
                win = "black";
            }
        }
        else
        {
            _currentBoard = _blackPlayerPtr->treeSearch(_currentBoard, 4);
            if(_currentBoard == "" || turns >= 100)
            {
                win = "red";
            }
        }
        
        _gameHistory.push_back(_currentBoard);
        redPlayerTurn = !redPlayerTurn;
        ++turns;
    }
    
    if(win == "red")
    {
        _redPlayerPtr->_performance += 1;
        _blackPlayerPtr->_performance -= 2;
    }
    else if(win == "black")
    {
        _redPlayerPtr->_performance -= 2;
        _blackPlayerPtr->_performance += 1;
    }
    else
    {
        _redPlayerPtr->_performance -= 1;
        _blackPlayerPtr->_performance -= 1;
    }
    
    writeGameHistoryToFile("gameHistories", "game");
}