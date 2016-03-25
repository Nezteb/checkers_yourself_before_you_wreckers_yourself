// game.h
// Noah Betzen & Lonny Strunk
// CS 405

// Compile main.cpp for this project

#ifndef GAME_H
#define GAME_H

#include "neuralNetwork.h"

#include <string>
using std::string;

class Game
{
public:
    void gameLoop();

private:
    void writeGameHistoryToFile(const string subdirectory, string weightFilename);
    string invertBoard(string board);

    NeuralNetwork _redPlayer;
    NeuralNetwork _blackPlayer;
    string _currentBoard;
    vector<string> _gameHistory;
};

#endif

