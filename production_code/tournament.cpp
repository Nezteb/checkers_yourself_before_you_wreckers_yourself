// tournament.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile main.cpp for this project

#include "tournament.h"
#include "neuralNetwork.h"

#include <iostream>
using std::cout;
using std::endl;
#include <sstream>
using std::ostringstream;

Tournament::Tournament(vector<NeuralNetwork> &neuralNetworks) 
{
    for(auto &neuralNetwork : neuralNetworks)
    {
        _neuralNetworks.push_back(&neuralNetwork);
    }
    
}

Tournament::~Tournament() 
{
    for(auto neuralNetworkPtr : _neuralNetworks)
    {
        neuralNetworkPtr = NULL;
    }
}

void Tournament::tournamentLoop(string directory)
{
    int numGames = 20;
    
    srand (time(NULL));
    
    NeuralNetwork *pointer1;
    NeuralNetwork *pointer2;
    
    for (int i = 0; i < _neuralNetworks.size(); ++i)
    {
        for(int j = 0; j < numGames; ++j)
        {
            ostringstream stream;
            stream << "game_nn" << i << "_game" << j;
            
            int randNN = rand() % (_neuralNetworks.size() - 1); // omit last NN
            
            //cout << "NEURAL NETWORK " << i << " vs NEURAL NETWORK " << (i == randNN ? _neuralNetworks.size()-1 : randNN) << ", GAME: " << j << endl;
            
            if (i == randNN) //if the NN is the already redPlayer
            {
                pointer1 = _neuralNetworks[i];
                pointer2 = _neuralNetworks[(_neuralNetworks.size() - 1)];
            }
            else
            {
                pointer1 = _neuralNetworks[i];
                pointer2 = _neuralNetworks[randNN];
            }
            
            Game newGame = Game(pointer1, pointer2); //use last NN
            newGame.gameLoop(directory, stream.str()); //run game
            
            pointer1 = NULL;
            pointer2 = NULL;
        }
    }
}