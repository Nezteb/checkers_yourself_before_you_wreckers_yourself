// main.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// LINUX
// g++ -Ofast --std=c++1y -msse2 -fopenmp -DEIGEN_NO_MALLOC -DEIGEN_NO_DEBUG main.cpp neuralNetwork.cpp tournament.cpp game.cpp -o main.o && time OMP_NUM_THREADS=8 ./main.o 32 40 10 1

// MAC (requires: brew install gcc --without-multilib)
// g++-5 -w -Ofast --std=c++1y -msse2 -fopenmp -DEIGEN_NO_MALLOC -DEIGEN_NO_DEBUG main.cpp neuralNetwork.cpp tournament.cpp game.cpp -o main.o && time OMP_NUM_THREADS=8 ./main.o 32 40 10 1

#include "neuralNetwork.h"
#include "tournament.h"
#include "game.h"

#include <iostream>
using std::cout;
using std::endl;
#include <algorithm>
using std::sort;
#include <string>
using std::string;
#include <sstream>
using std::ostringstream;

#include "../third_party/Eigen/Core"
using Eigen::initParallel;
using Eigen::setNbThreads;
using Eigen::nbThreads;

#define NNSIZE 100

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

    
    //ACTUAL NEURALNETWORK EVOLUTION 
    vector<NeuralNetwork> NNs;
    for (int i = 0; i < NNSIZE; ++i)
    {
        NNs.push_back(NeuralNetwork(inputs));
    }
    
    int bestGeneration = 0;
    int bestPerformance = 0;
    
    int generationNum = 0;
    while(true)
    {
        cout << "GENERATION #" << generationNum << endl;
        
        ostringstream stringStream0;
        stringStream0 << "gameHistories/" << "gen" << generationNum;
        string directory0 = stringStream0.str();
        
        // Start the tournament
        Tournament tourney(NNs);
        tourney.tournamentLoop(directory0);
        
        // Sort the neural networks by performance in the tournament
        sort(NNs.begin(), NNs.end(), [](const NeuralNetwork &a, const NeuralNetwork &b)
        {
            return a._performance > b._performance;
        });
        
        cout << "TOP TEN OF GENERATION #" << generationNum << endl;
        for(int i = 0; i < 10; ++i)
        {
            cout << "NN #" << i << ": "<< NNs[i]._performance << "\t";
        }
        cout << endl;
        
        if(NNs[0]._performance >= bestPerformance)
        {
            bestPerformance = NNs[0]._performance;
            bestGeneration = generationNum;
        }
        
        cout << "Best generation and performance so far: GEN: " << bestGeneration << ", PERF: " << bestPerformance << endl;
        
        // Overwrite and generate new children
        int halfNNs = NNs.size() / 2;
        
        for(int i = 0; i < halfNNs; ++i)
        {
            NNs[halfNNs + i] = NNs[i].spawnChild();
        }
        
        // Zero out all of the new boards 
        for(auto &nn : NNs)
        {
            nn._performance = 0;
        }
        
        // Save/write NeuralNetworks to file
        ostringstream stringStream1;
        stringStream1 << "generations/" << "gen" << generationNum;
        string directory1 = stringStream1.str();
        
        for(int i = 0; i < NNs.size(); ++i)
        {
            ostringstream stringStream2;
            stringStream2 << i;
            string file = stringStream2.str();
            
            NNs[i].writeWeightsToFile(directory1, file);
        }
         
        // Increment generation
        generationNum += 1;
        cout << endl;
    }
    
    
    
    
    
    /*
    //TESTING TOURNAMENT
    vector<NeuralNetwork> NNs;
    for (int i = 0; i < NNSIZE; ++i)
    {
        NNs.push_back(NeuralNetwork(inputs));
    }
    
    Tournament tourney(NNs);
    
    tourney.tournamentLoop();
    
    sort(NNs.begin(), NNs.end(), [](const NeuralNetwork &a, const NeuralNetwork &b)
    {
        return a._performance > b._performance;
    });
    
    for(int i = 0; i < NNs.size(); ++i)
    {
        cout << "NN #" << i << ": "<< NNs[i]._performance << endl;
    }
    */
    
    
    
    
    /*
    //TESTING GAME
    //cout << "CREATING NEURAL NETWORKS" << endl;
    NeuralNetwork *red = new NeuralNetwork(inputs);
    NeuralNetwork *black = new NeuralNetwork(inputs);
    
    const string name = "generations";
    const string a = "0";
    const string b = "8";
    red->readWeightsFromFile(name, a);
    black->readWeightsFromFile(name, b);
    
    //cout << "NEURAL NETWORKS CREATED" << endl;
    
    //cout << "CREATING GAME" << endl;
    Game gameTest(red, black);
    //cout << "GAME CREATED" << endl;
    
    //cout << "STARTING GAME LOOP" << endl;
    gameTest.gameLoop("test");
    //cout << "GAME LOOP ENDED" << endl;
    
    cout << "NN red  : "<< red->_performance << endl;
    cout << "NN black: "<< black->_performance << endl;
    
    delete red;
    delete black;
    */
    
    
    
    
    /*    
    //TESTING NEURALNETWORK
    NeuralNetwork test(inputs);
    test._isRed = true;
    
    cout << "NormalBoard best move:" << endl;
    cout << test.evaluateBoard("rrrrrrrrrrrr________bbbbbbbbbbbb") << endl;
    
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