// neuralNetwork.h
// Noah Betzen & Lonny Strunk
// CS 405

// Compile main.cpp for this project

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

struct Node; // forward declaration

class NeuralNetwork
{
public:
    NeuralNetwork(vector<int> topology);
    void feedForward();
    NeuralNetwork spawnChild();
    void print();
    double evaluateBoard(const string &board);
    MatrixXXd readWeightFromFile(const string subdirectory, string weightFilename);
    void writeWeightToFile(const string subdirectory, MatrixXXd weight, string weightFilename);
    vector<string> generateMoves(string board);
    string treeSearch(string rootBoard, int depth);
    
    int _performance;
    bool _isRed;
private:
    static double _sigmoid(double x);
    vector<string> generateJumps(const string &currentBoard, int currentPieceIndex);
    vector<string> generateMovesHelper(const string &currentBoard, vector<pair<int,char>> &redPieces);
    double negaScout(Node *currentNode, int depth, double alpha, double beta, bool isRed);
    string invertBoard(string board);

    double _kingValue;
    vector<int> _topology;
    vector<MatrixXXd> _layers;
    vector<MatrixXXd> _weights;
};

#endif

