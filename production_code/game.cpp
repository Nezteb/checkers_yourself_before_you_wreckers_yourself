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
#include <sstream>
using std::ostringstream;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <sys/stat.h> // for mkdir
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include "skynet/checkers.hpp"
#include "skynet/checkers_client.hpp"
#include <stdexcept>
#include <string>
#include <unistd.h>

Game::Game(NeuralNetwork *redPlayer, NeuralNetwork *blackPlayer)
{
    _redPlayerPtr = redPlayer;
    _blackPlayerPtr = blackPlayer;
    
    _redPlayerPtr->_isRed = true;
    _blackPlayerPtr->_isRed = false;
}

Game::Game(NeuralNetwork *us, bool weAreRed)
{
    _networkUs = us;
    _networkUs->_isRed = weAreRed;
}

Game::~Game()
{
    _redPlayerPtr = NULL;
    _blackPlayerPtr = NULL;
}

void Game::writeGameHistoryToFile(string subdirectory, string weightFilename)
{
    struct stat info;

    if(stat(subdirectory.c_str(), &info) != 0)
    {
        //cout << "Cannot access: " << subdirectory << " (it probably already exists)" << endl;
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
    
    //cout << "\nAttempting to write to file: " << fileName << endl;
    
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

void Game::gameLoop(string directory, string filename)
{
    //cout << "STARTING GAME" << endl;
    
    string win = "";
    int turns = 0;
    bool redPlayerTurn = true; // red goes first
    _currentBoard = "rrrrrrrrrrrr________bbbbbbbbbbbb"; // starting board
    
    while (win == "")
    {
        if(turns >= 100)
        {
            break;
        }
        
        //cout << "TURN " << turns << endl;
        if(redPlayerTurn)
        {
            //cout << "RED MOVE" << endl;
            _currentBoard = _redPlayerPtr->treeSearch(_currentBoard, 8);
            
            if(_currentBoard == "")
            {
                win = "black";
            }
            //cout << "END RED MOVE" << endl;
        }
        else
        {
            //cout << "BLACK MOVE" << endl;
            _currentBoard = _blackPlayerPtr->treeSearch(_currentBoard, 8);
            
            if(_currentBoard == "")
            {
                win = "red";
            }
            //cout << "END BLACK MOVE" << endl;
        }
        
        _gameHistory.push_back(_currentBoard);
        redPlayerTurn = !redPlayerTurn;
        
        //cout << "END TURN " << turns << endl;
        turns += 1;
    }
    
    if(win == "red")
    {
        //cout << "WIN RED" << endl;
        _redPlayerPtr->_performance += 1;
        _blackPlayerPtr->_performance -= 2;
    }
    else if(win == "black")
    {
        //cout << "WIN BLACK" << endl;
        _redPlayerPtr->_performance -= 2;
        _blackPlayerPtr->_performance += 1;
    }
    else
    {
        //cout << "DRAW" << endl;
        _redPlayerPtr->_performance -= 1;
        _blackPlayerPtr->_performance -= 1;
    }
    
    int status = system("rm -rf ./gameHistories/gen*");
    
    writeGameHistoryToFile(directory, filename);
}

bool isItOurTurn(bool weAreRed, string server, string gameName)
{
    skynet::checkers::game_info_t thisGame = skynet::checkers::info_game(server, gameName);
    
    bool redPlayerTurn;
    
    //cout << "GAME STATUS: " << thisGame.status << endl;
    
    if(thisGame.status == 0) // red
    {
        redPlayerTurn = true;
    }
    else if (thisGame.status == 1) // black
    {
        redPlayerTurn = false;
    }
    
    if(weAreRed && redPlayerTurn)
    {
        return true;
    }
    else if(!weAreRed && !redPlayerTurn)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Game::networkGameLoop()
{
    //cout << "STARTING GAME" << endl;
    
    // take in best weights we have
    
    string server = "skynet.cs.uaf.edu";
    string gameName = "testgame2";
    
    //bool redPlayerTurn = true; // red goes first
    
    bool gameover = false;
    
    skynet::checkers::game_info_t thisGame = skynet::checkers::info_game(server, gameName);
    _currentBoard = thisGame.boards.back();
    
    bool ourTurn;
    
    cout << "GAME START" << endl;
    ourTurn = isItOurTurn(_networkUs->_isRed, server, gameName);
    
    while (!gameover)
    {
        if(ourTurn)
        {
            cout << "OUR TURN" << endl;
            _currentBoard = _networkUs->treeSearch(_currentBoard, 16);
            
            skynet::checkers::board_t networkBoard(_currentBoard);
            skynet::checkers::play_game(server, gameName, networkBoard);
            cout << "WE MOVED: " << _currentBoard << endl;
            
            //ourTurn = isItOurTurn(_networkUs->_isRed, server, gameName);
            ourTurn = !ourTurn;
            
            if(_currentBoard == "")
            {
                gameover = true;
            }
        }
        else
        {
            cout << "THEIR TURN" << endl;
            while(true)
            {
                cout << ".";
                ourTurn = isItOurTurn(_networkUs->_isRed, server, gameName);
                
                if(ourTurn)
                {
                    skynet::checkers::game_info_t thisGame = skynet::checkers::info_game(server, gameName);
                    _currentBoard = thisGame.boards.back();
                    cout << "THEY MOVED: " << _currentBoard << endl;
                    break;
                }
            }
            cout << endl;
        }
        
        //redPlayerTurn = !redPlayerTurn;
    }
}