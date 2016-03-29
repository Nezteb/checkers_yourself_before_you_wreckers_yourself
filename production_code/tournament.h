// tournament.h
// Noah Betzen & Lonny Strunk
// CS 405

// Compile main.cpp for this project

#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include "neuralNetwork.h"
#include "game.h"

// #include <array>
// using std::array;

//#define NNSIZE 100

class Tournament
{
public:
    Tournament(vector<NeuralNetwork> &neuralNetworks);
    void tournamentLoop();
    
private:
    vector<NeuralNetwork> _neuralNetworks;
    
};

#endif

