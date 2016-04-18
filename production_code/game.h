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
    Game(NeuralNetwork *redPlayer, NeuralNetwork *blackPlayer);
    Game(NeuralNetwork *us, bool weAreRed);
    void gameLoop(string directory, string filename);
    void networkGameLoop();
    
    ~Game();

private:
    void writeGameHistoryToFile(string subdirectory, string weightFilename);
    string invertBoard(string board);

    NeuralNetwork *_redPlayerPtr;
    NeuralNetwork *_blackPlayerPtr;
    
    NeuralNetwork *_networkUs;
    
    string _currentBoard;
    vector<string> _gameHistory;
};

#endif

