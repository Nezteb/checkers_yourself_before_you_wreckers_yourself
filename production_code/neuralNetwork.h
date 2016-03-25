// neuralNetwork.h
// Noah Betzen & Lonny Strunk
// CS 405

// Compile and run with:
// clang++ -Ofast --std=c++1y neuralNetwork.cpp -o neuralNetwork.o && ./neuralNetwork.o

#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <utility>
using std::pair;

// Eigen matrix includes
#include "../third_party/Eigen/Core"
using Eigen::Matrix;
using Eigen::RowMajor;
using Eigen::Dynamic;

typedef Matrix<double, Dynamic, Dynamic, RowMajor> MatrixXXd;

class NeuralNetwork
{
public:
    NeuralNetwork(vector<int> topology);
    void feedForward();
    NeuralNetwork spawnChild();
    void print();
    MatrixXXd readWeightFromFile(const string subdirectory, string weightFilename);
    void writeWeightToFile(const string subdirectory, MatrixXXd weight, string weightFilename);
    vector<string> generateMoves(string board);
    
private:
    static double _sigmoid(double x);
    vector<string> generateJumps(const string &currentBoard, int currentPieceIndex);
    vector<string> generateMovesHelper(const string &currentBoard, vector<pair<int,char>> &redPieces);

    double _kingValue;
    vector<int> _topology;
    vector<MatrixXXd> _layers;
    vector<MatrixXXd> _weights;
    int _performance;
};

#endif

