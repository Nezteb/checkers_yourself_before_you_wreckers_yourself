// main.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile and run with:
// g++ -Ofast --std=c++1y -msse2 -fopenmp -DEIGEN_NO_MALLOC -DEIGEN_NO_DEBUG main.cpp neuralNetwork.cpp tournament.cpp game.cpp -o main.o && time OMP_NUM_THREADS=8 ./main.o 32 40 10 1

#include "neuralNetwork.h"
#include "tournament.h"
#include "game.h"

#include <iostream>
using std::cout;
using std::endl;

#include "../third_party/Eigen/Core"
using Eigen::initParallel;
using Eigen::setNbThreads;
using Eigen::nbThreads;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "INCORRECT USAGE\nProper Usage:\n\t"<< argv[0] << " inputLayer [otherLayers] outputLayer"
        << "\n\t(Requires at least two layer topologies as inputs)" << endl;
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

    NeuralNetwork test(inputs);
    test._isRed = true;
    
    cout << "NormalBoard best move:" << endl;
    //cout << test.evaluateBoard("rrrrrrrrrrrr________bbbbbbbbbbbb") << endl;
    
    // starts segfaulting at 7
    cout << test.treeSearch("rrrrrrrrrrrr________bbbbbbbbbbbb", 2) << endl;
    
    // cout << "PARENT:" << endl;
    // test.print();

    // NeuralNetwork baby = test.spawnChild();
    
    // cout << "CHILD:" << endl;
    // baby.print();
    
    return 0;
}