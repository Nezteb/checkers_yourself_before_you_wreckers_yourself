// main.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// LINUX
// g++ -Ofast --std=c++1y -msse2 -fopenmp -DEIGEN_NO_MALLOC -DEIGEN_NO_DEBUG main.cpp neuralNetwork.cpp tournament.cpp game.cpp -o main.o && time OMP_NUM_THREADS=8 ./main.o 32 40 10 1

// MAC (requires: brew install gcc --without-multilib)
// g++-5 -Ofast --std=c++1y -msse2 -fopenmp -DEIGEN_NO_MALLOC -DEIGEN_NO_DEBUG main.cpp neuralNetwork.cpp tournament.cpp game.cpp -o main.o && time OMP_NUM_THREADS=8 ./main.o 32 40 10 1

#include "neuralNetwork.h"
#include "tournament.h"
#include "game.h"

#include <iostream>
using std::cout;
using std::endl;
#include <algorithm>
using std::sort;

#include "../third_party/Eigen/Core"
using Eigen::initParallel;
using Eigen::setNbThreads;
using Eigen::nbThreads;

#define NNSIZE 10

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "INCORRECT USAGE\nProper Usage:\n\t"<< argv[0] << " inputLayer [otherLayers] outputLayer"
        << "\n\t(Requires at least two layer topologies as inputs)" << endl;
        exit(1);
    }
    
    initParallel();
    setNbThreads(1024);
    cout << "Using " << nbThreads() << " threads" << endl;
    cout << endl;

    srand(time(NULL));

    vector<int> inputs;

    for(int i=1; i<argc; ++i) // first input is program name
    {
        inputs.push_back(atoi(argv[i]));
    }
    
    cout << "TOPOLOGY:\n";
    for(int i=0; i<inputs.size(); ++i)
    {
        cout << "Layer " << i << ": " << inputs[i] << " nodes\n";
    }
    cout << endl;

    /*
    //ACTUAL NEURALNETWORK EVOLUTION 
    vector<NeuralNetwork> NNs();
    for (int i = 0; i < NNSIZE; ++i)
    {
        NNs.push_back( NeuralNetwork(inputs) );
    }
    
    int generationNum = 0;
    while(true)
    {
        cout << "Generation #" << generationNum << endl;
        // Start the tournament
        Tournament tourney(NNs);
        tourney.tournamentLoop();
        
        // Sort the neural networks by performance in the tournament
        sort(NNs.begin(), NNs.end(), [&](NeuralNetwork a, NeuralNetwork b)
        {a._performance > b._performance;});
        
        // Overwrite and generate new children
        int halfNNs = NNs.size() / 2;
        for(int i = 0; i < halfNNs; ++i)
        {
            NNs[halfNNs + i] = NNs[i].spawnChild();
        }
        
        // Zero out all of the new boards 
        for(auto & nn : NNs)
        {
            nn._performance = 0;
        }
        
        // Save/write best NeuralNetwork to file
        //NNs[0].writeWeightToFile(const string subdirectory, MatrixXXd weight, string weightFilename);
        //cout << NNs[0] << endl;
        
        // Increment generation
        generationNum += 1;
    }
    */
    
    
    
    
    
    
    //TESTING TOURNAMENT
    vector<NeuralNetwork> NNs;
    for (int i = 0; i < NNSIZE; ++i)
    {
        NNs.push_back(NeuralNetwork(inputs));
    }
    
    Tournament tourney(NNs);
    
    tourney.tournamentLoop();
    
    for(int i = 0; i < NNs.size(); ++i)
    {
        cout << "NN #" << i << ": "<< NNs[i]._performance << endl;
    }
    
    
    
    /*
    //TESTING GAME
    NeuralNetwork red(inputs);
    NeuralNetwork black(inputs);
    
    Game gameTest(red, black);
    
    gameTest.gameLoop();
    
    cout << "NN red  : "<< red._performance << endl;
    cout << "NN black: "<< black._performance << endl;
    */
    
    /*
    //TESTING NEURALNETWORK
    NeuralNetwork test(inputs);
    test._isRed = true;
    
    cout << "NormalBoard best move:" << endl;
    //cout << test.evaluateBoard("rrrrrrrrrrrr________bbbbbbbbbbbb") << endl;
    
    // starts getting slow at 7
    cout << test.treeSearch("rrrrrrrrrrrr________bbbbbbbbbbbb", 8) << endl;
    */
    
    
    /*
    //TESTING GENERATEMOVES
    NeuralNetwork test(inputs);
    test._isRed = true;
    string nextBoard = "rrrrrrrrrrrr________bbbbbbbbbbbb";
    for(int i = 0; i < 10; ++i)
    {
        vector<string> temp = test.generateMoves(nextBoard, test._isRed);
        
        cout << "MOVE " << i << ": " << endl;
        for(auto board : temp)
        {
            cout << board << endl;
        }
        
        test._isRed = !test._isRed;
        nextBoard = temp[0];
    }
    */
    
    /*
    //TESTING EVOLUTION
    NeuralNetwork test(inputs);
    cout << "PARENT:" << endl;
    test.print();

    NeuralNetwork baby = test.spawnChild();
    
    cout << "CHILD:" << endl;
    baby.print();
    */
    
    return 0;
}