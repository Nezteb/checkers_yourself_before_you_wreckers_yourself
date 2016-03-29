// tournament.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile main.cpp for this project

#include "tournament.h"
#include "neuralNetwork.h"

Tournament::Tournament(vector<NeuralNetwork> &neuralNetworks) 
{
    _neuralNetworks = neuralNetworks;
}

void Tournament::tournamentLoop()
{
    int numGames = 5;
    
    srand (time(NULL));
    
    for (int i = 0; i < _neuralNetworks.size(); ++i)
    {
        for(int j = 0; j < numGames; ++j)
        {  
            int randNN = rand() % (_neuralNetworks.size() - 1); // omit last NN
            
            if (i == randNN) //if the NN is the already redPlayer
            {                                  
                 Game newGame = Game(_neuralNetworks[i], _neuralNetworks[(_neuralNetworks.size() - 1)]); //use last NN
                 newGame.gameLoop(); //run game
            }
            else
            {
                Game newGame = Game(_neuralNetworks[i], _neuralNetworks[randNN]);
                newGame.gameLoop(); //run game
            }
        }
    }
}