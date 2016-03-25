// neuralNetwork.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile main.cpp for this project

#include "neuralNetwork.h"

// Eigen matrix includes
#include "../third_party/Eigen/Core"
using Eigen::Matrix;
using Eigen::RowMajor;
using Eigen::Dynamic;
using Eigen::IOFormat;
using Eigen::initParallel;
using Eigen::setNbThreads;
using Eigen::nbThreads;

typedef Matrix<double, Dynamic, Dynamic, RowMajor> MatrixXXd;

#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;
#include <cstdlib>
using std::srand;
using std::rand;
#include <cassert>
#include <cmath>
using std::tanh;
#include <functional>
using std::ptr_fun;
#include <random>
using std::random_device;
using std::mt19937;
using std::normal_distribution;
#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <sys/stat.h> // for mkdir
#include <sys/types.h>
#include <algorithm>
using std::swap;
using std::sort;
using std::unique;
#include <utility>
using std::pair;
#include <limits>
using std::numeric_limits;
#include <algorithm>
using std::max;


double INF = numeric_limits<double>::infinity();

struct Node
{
    Node(string newBoard)
    {
        board = newBoard;
        parentNode = NULL;
    }
    
    void createChild(string newBoard)
    {
        Node temp(newBoard);
        temp.parentNode = this;
        childNodes.push_back(temp);
    }
    
    string board;
    
    Node *parentNode;
    vector<Node> childNodes;
    
    void printTree(const string &tab = "", const bool last = true)
    {
        cout << tab;
        
        if(last)
            cout << "└";
        else
            cout << "├";
            
        cout << "─";
        
        if(childNodes.size() > 0)
            cout << "┬";
        else
            cout << "─";
        
        cout << "B: " << board << /*" V: " << value << " A: " << alpha << " B: " << beta <<*/ endl;
        
        for(int i = 0; i < childNodes.size(); ++i)
        {
            string newTab;
            
            if(last)
                newTab="  ";
            else
                newTab="│ ";
            
            childNodes[i].printTree(tab + newTab, i + 1 == childNodes.size());
        }
    }
};


random_device seed;
mt19937 superRandom(seed());

// random double
double dRand(double dMin, double dMax)
{
    double d = (double)rand() / RAND_MAX;
    return dMin + d * (dMax - dMin);
}

NeuralNetwork::NeuralNetwork(vector<int> topology): _topology(topology)
{
    _performance = 0;
    _kingValue = dRand(1.0,2.0);
    
    for(int i=0; i<_topology.size(); ++i) // layers
    {
        _layers.push_back( MatrixXXd(1, _topology[i]) );
        _layers[i].setRandom();
    }

    for(int i=0; i<_topology.size()-1; ++i) // weights
    {
        _weights.push_back( MatrixXXd(_topology[i], _topology[i+1]) );
        _weights[i].setRandom();
    }
}

void NeuralNetwork::feedForward()
{
    for(int j=1; j<_layers.size(); ++j) // go through all the layers
    {
        _layers[j].noalias() = _layers[j-1] * _weights[j-1]; // next layer = previous layer * weights in between

        _layers[j].noalias() = _layers[j].unaryExpr(ptr_fun(&_sigmoid)); // apply sigmoid to layer
    }
}

NeuralNetwork NeuralNetwork::spawnChild()
{
    NeuralNetwork childNetwork(_topology);

    for(int i = 0; i < _weights.size(); ++i)
    {
        for(int row = 0; row < _weights[i].rows(); ++row)
        {
            for(int col = 0; col < _weights[i].cols(); ++col)
            {
                // create new distirubtion based on weight and sigma
                normal_distribution<double> distribution(_weights[i](row,col), dRand(0.0, 1.0));

                // apply distribution to weight
                childNetwork._weights[i](row,col) = distribution(superRandom);
            }
        }
    }
    
    // randomize king value slightly
    normal_distribution<double> distribution(_kingValue, dRand(0.0, 1.0));
    childNetwork._kingValue = distribution(superRandom);

    return childNetwork;
}

double NeuralNetwork::evaluateBoard(const string &board)
{
    // iterate through all 32 characters of board, assign values to a 32x1 matrix
    // _layers[0] is input layer
    for(int i = 0; i < board.size(); ++i)
    {
        switch(board[i])
        {
            case 'r':
                _layers[0](i, 0) = -1.0;
                break;
            case 'R':
                _layers[0](i, 0) = _kingValue;
                break;
            case 'b':
                _layers[0](i, 0) = 1.0;
                break;
            case 'B':
                _layers[0](i, 0) = -_kingValue;
                break;
            case '_':
                _layers[0](i, 0) = 0.0;
                break;
        }
    }
    
    for(int j=1; j<_layers.size(); ++j) // go through all the layers
    {
        // TODO: this is the slowest portion
        _layers[j].noalias() = _layers[j-1] * _weights[j-1]; // next layer = previous layer * weights in between

        // TODO: faster way to do unary expressions?
        _layers[j].noalias() = _layers[j].unaryExpr(ptr_fun(_sigmoid)); // apply sigmoid to layer
    }
    
    return (_layers.back())(0,0); // get first element of 1x1 matrix
}
    
void NeuralNetwork::print()
{
    int j = 0;
    for(int i = 0; i < _layers.size(); ++i)
    {
        cout << "Layer " << i << ": " << _layers[i].rows() << "x" << _layers[i].cols() << "\n\n" << _layers[i] << "\n" << endl;
        
        if(j < _weights.size())
        {
            cout << "Weight " << j << ": " << _weights[j].rows() << "x" << _weights[j].cols() << "\n\n" << _weights[j] << "\n" << endl;
            ++j;
        }
    }
}

double NeuralNetwork::_sigmoid(double x)
{
    return 1/(1+exp(-x));
}

IOFormat CSV(Eigen::StreamPrecision, Eigen::DontAlignCols, ",", ",", "", "", "", "");

MatrixXXd NeuralNetwork::readWeightFromFile(const string subdirectory, string weightFilename)
{
    struct stat info;

    if(stat(subdirectory.c_str(), &info) != 0)
    {
        cout << "Cannot access: " << subdirectory << endl;
    }
    else if(!(info.st_mode & S_IFDIR))
    {
        cout << subdirectory << " is not a directory." << endl;
    }
    
    ostringstream stringStream;
    stringStream << subdirectory << "/" << weightFilename << ".txt";
    string fileName = stringStream.str();
    
    cout << "\nAttempting to read from file: " << fileName << endl;
    
    ifstream file(fileName, ifstream::in);
    
    string rowsString;
    string colsString;
    int rows;
    int cols;
    string tempString;
    
    MatrixXXd *pointer ;
    
    if (file.is_open())
    {
        getline(file, rowsString);
        getline(file, colsString);
        rows = stoi(rowsString);
        cols = stoi(colsString);
        
        MatrixXXd matrix(rows, cols);
        pointer = &matrix;
        
        for(int j = 0; j < rows; ++j)
        {
            for(int k = 0; k < cols; ++k)
            {
                getline(file, tempString, ',');
                matrix(j, k) = stof(tempString);
            }
        }
        
        file.close();
        return matrix;
    }
    else
    {
        cout << "Could not open: " << fileName << endl;
    }
}

void NeuralNetwork::writeWeightToFile(const string subdirectory, MatrixXXd weight, string weightFilename)
{
    struct stat info;

    if(stat(subdirectory.c_str(), &info) != 0)
    {
        cout << "Cannot access: " << subdirectory << endl;
    }
    else if(!(info.st_mode & S_IFDIR))
    {
        cout << subdirectory << " is not a directory." << endl;
    }
    
    // make the directory if it doesn't already exist
    const int dir_err = mkdir(subdirectory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    ostringstream stringStream;
    stringStream << subdirectory << "/" << weightFilename << ".txt";
    string fileName = stringStream.str();
    
    cout << "\nAttempting to write to file: " << fileName << endl;
    
    ofstream file(fileName, ofstream::out | ofstream::trunc);

    if (file.is_open())
    {
        file << weight.rows() << "\n";
        file << weight.cols() << "\n";
        file << weight.format(CSV);
        
        file.close();
    }
    else
    {
        cout << "Could not open: " << fileName << endl;
    }
}

#define BOARD_SIZE 32

enum { NE=0, SE=1, SW=2, NW=3 };

// red piece move
// from top of board
// access with: movesBoard[pieceIndex * 4 + direction]
// pieceIndex: NE, SE, SW, NW

// each pair of four numbers represents one piece and its four directional moves (based on above enum)
const vector<int> movesBoard = {
            -1,  5,  4, -1,
            -1,  6,  5, -1,
            -1,  7,  6, -1,
            -1, -1,  7, -1,
             0,  8, -1, -1,
             1,  9,  8,  0,
             2, 10,  9,  1,
             3, 11, 10,  2,
             5, 13, 12,  4,
             6, 14, 13,  5,
             7, 15, 14,  6,
            -1, -1, 15,  7,
             8, 16, -1, -1,
             9, 17, 16,  8,
            10, 18, 17,  9,
            11, 19, 18, 10,
            13, 21, 20, 12,
            14, 22, 21, 13,
            15, 23, 22, 14,
            -1, -1, 23, 15,
            16, 24, -1, -1,
            17, 25, 24, 16,
            18, 26, 25, 17,
            19, 27, 26, 18,
            21, 29, 28, 20,
            22, 30, 29, 21,
            23, 31, 30, 22,
            -1, -1, 31, 23,
            24, -1, -1, -1,
            25, -1, -1, 24,
            26, -1, -1, 25,
            27, -1, -1, 26 };

vector<string> NeuralNetwork::generateJumps(const string &currentBoard, int currentPieceIndex)
{
    vector<string> possibleJumps;
    string tempBoard;

    int southEast;
    int southWest;
    int northEast;
    int northWest;

    int nextSouthEast;
    int nextSouthWest;
    int nextNorthEast;
    int nextNorthWest;

    if(currentBoard[currentPieceIndex] == 'r' || currentBoard[currentPieceIndex] == 'R')
    {
        southEast = movesBoard[currentPieceIndex * 4 + SE];
        southWest = movesBoard[currentPieceIndex * 4 + SW];

        if (currentBoard[southEast] == 'b' || currentBoard[southEast] == 'B')
        {
            nextSouthEast = movesBoard[southEast * 4 + SE];

            if(currentBoard[nextSouthEast] == '_')
            {
                tempBoard = currentBoard;
                tempBoard[southEast] = '_';
                swap(tempBoard[currentPieceIndex], tempBoard[nextSouthEast]);
                vector<string> tempJumps;

                if(nextSouthEast >= 28 && tempBoard[nextSouthEast] == 'r')
                {
                    tempBoard[nextSouthEast] = 'R';
                    tempJumps.push_back(tempBoard);
                } else
                {
                    tempJumps = generateJumps(tempBoard, nextSouthEast);
                }

                possibleJumps.insert(possibleJumps.end(), tempJumps.begin(), tempJumps.end());
            }
        }
        if (currentBoard[southWest] == 'b' || currentBoard[southWest] == 'B')
        {
            nextSouthWest = movesBoard[southWest * 4 + SW];

            if(currentBoard[nextSouthWest] == '_')
            {
                tempBoard = currentBoard;
                tempBoard[southWest] = '_';
                swap(tempBoard[currentPieceIndex], tempBoard[nextSouthWest]);
                vector<string> tempJumps;

                if(nextSouthWest >= 28 && tempBoard[nextSouthWest] == 'r')
                {
                    tempBoard[nextSouthWest] = 'R';
                    tempJumps.push_back(tempBoard);
                } else
                {
                    tempJumps = generateJumps(tempBoard, nextSouthWest);
                }

                possibleJumps.insert(possibleJumps.end(), tempJumps.begin(), tempJumps.end());
            }
        }
    }
    if(currentBoard[currentPieceIndex] == 'R')
    {
        northEast = movesBoard[currentPieceIndex * 4 + NE];
        northWest = movesBoard[currentPieceIndex * 4 + NW];

        if (currentBoard[northEast] == 'b' || currentBoard[northEast] == 'B')
        {
            nextNorthEast = movesBoard[northEast * 4 + NE];

            if(currentBoard[nextNorthEast] == '_')
            {
                tempBoard = currentBoard;
                tempBoard[northEast] = '_';
                swap(tempBoard[currentPieceIndex], tempBoard[nextNorthEast]);

                vector<string> tempJumps = generateJumps(tempBoard, nextNorthEast);
                possibleJumps.insert(possibleJumps.end(), tempJumps.begin(), tempJumps.end());
            }
        }
        if (currentBoard[northWest] == 'b' || currentBoard[northWest] == 'B')
        {
            nextNorthWest = movesBoard[northWest * 4 + NW];

            if(currentBoard[nextNorthWest] == '_')
            {
                tempBoard = currentBoard;
                tempBoard[northWest] = '_';
                swap(tempBoard[currentPieceIndex], tempBoard[nextNorthWest]);

                vector<string> tempJumps = generateJumps(tempBoard, nextNorthWest);
                possibleJumps.insert(possibleJumps.end(), tempJumps.begin(), tempJumps.end());
            }
        }
    }

    if(possibleJumps.empty())
    {
        return vector<string>{currentBoard};
    }
    return possibleJumps;
}

// checks every spot on the board for valid moves
// returns a vector of board strings
vector<string> NeuralNetwork::generateMovesHelper(const string &currentBoard, vector<pair<int,char>> &redPieces)
{
    vector<string> possibleMoves;
    vector<string> possibleJumps;
    string tempBoard;

    int southEast;
    int southWest;
    int northEast;
    int northWest;

    int nextSouthEast;
    int nextSouthWest;
    int nextNorthEast;
    int nextNorthWest;

    for (int i = 0; i < redPieces.size(); ++i)
    {
        if (redPieces[i].second == 'r' || redPieces[i].second == 'R')
        {
            southEast = movesBoard[redPieces[i].first * 4 + SE];
            southWest = movesBoard[redPieces[i].first * 4 + SW];

            if (currentBoard[southEast] == 'b' || currentBoard[southEast] == 'B')
            {
                nextSouthEast = movesBoard[southEast * 4 + SE];

                if(currentBoard[nextSouthEast] == '_')
                {
                    tempBoard = currentBoard;
                    tempBoard[southEast] = '_';
                    swap(tempBoard[redPieces[i].first], tempBoard[nextSouthEast]);
                    vector<string> tempJumps;

                    if(nextSouthEast >= 28 && redPieces[i].second == 'r')
                    {
                        tempBoard[nextSouthEast] = 'R';
                        tempJumps.push_back(tempBoard);
                    } else
                    {
                        tempJumps = generateJumps(tempBoard, nextSouthEast);
                    }

                    possibleJumps.insert(possibleJumps.end(), tempJumps.begin(), tempJumps.end());
                }
            }
            if (currentBoard[southWest] == 'b' || currentBoard[southWest] == 'B')
            {
                nextSouthWest = movesBoard[southWest * 4 + SW];

                if(currentBoard[nextSouthWest] == '_')
                {
                    tempBoard = currentBoard;
                    tempBoard[southWest] = '_';
                    swap(tempBoard[redPieces[i].first], tempBoard[nextSouthWest]);
                    vector<string> tempJumps;

                    if(nextSouthWest >= 28 && redPieces[i].second == 'r')
                    {
                        tempBoard[nextSouthWest] = 'R';
                        tempJumps.push_back(tempBoard);
                    } else
                    {
                        tempJumps = generateJumps(tempBoard, nextSouthWest);
                    }

                    possibleJumps.insert(possibleJumps.end(), tempJumps.begin(), tempJumps.end());
                }
            }
            if (possibleJumps.empty())
            {
                if (currentBoard[southEast] == '_')
                {
                    tempBoard = currentBoard;
                    swap(tempBoard[redPieces[i].first], tempBoard[southEast]);

                    if(southEast >= 28 && redPieces[i].second == 'r')
                    {
                        tempBoard[southEast] = 'R';
                    }

                    possibleMoves.push_back(tempBoard);
                }
                if (currentBoard[southWest] == '_')
                {
                    tempBoard = currentBoard;
                    swap(tempBoard[redPieces[i].first], tempBoard[southWest]);

                    if(southWest >= 28 && redPieces[i].second == 'r')
                    {
                        tempBoard[southWest] = 'R';
                    }

                    possibleMoves.push_back(tempBoard);
                }
            }
        }
        if (redPieces[i].second == 'R')
        {
            northEast = movesBoard[redPieces[i].first * 4 + NE];
            northWest = movesBoard[redPieces[i].first * 4 + NW];

            if (currentBoard[northEast] == 'b' || currentBoard[northEast] == 'B')
            {
                nextNorthEast = movesBoard[northEast * 4 + NE];

                if(currentBoard[nextNorthEast] == '_')
                {
                    tempBoard = currentBoard;
                    tempBoard[northEast] = '_';
                    swap(tempBoard[redPieces[i].first], tempBoard[nextNorthEast]);

                    vector<string> tempJumps = generateJumps(tempBoard, nextNorthEast);
                    possibleJumps.insert(possibleJumps.end(), tempJumps.begin(), tempJumps.end());
                }
            }
            if (currentBoard[northWest] == 'b' || currentBoard[northWest] == 'B')
            {
                nextNorthWest = movesBoard[northWest * 4 + NW];

                if(currentBoard[nextNorthWest] == '_')
                {
                    tempBoard = currentBoard;
                    tempBoard[northWest] = '_';
                    swap(tempBoard[redPieces[i].first], tempBoard[nextNorthWest]);

                    vector<string> tempJumps = generateJumps(tempBoard, nextNorthWest);
                    possibleJumps.insert(possibleJumps.end(), tempJumps.begin(), tempJumps.end());
                }
            }
            if(possibleJumps.empty())
            {
                if (currentBoard[northEast] == '_')
                {
                    tempBoard = currentBoard;
                    swap(tempBoard[redPieces[i].first], tempBoard[northEast]);
                    possibleMoves.push_back(tempBoard);
                }
                if (currentBoard[northWest] == '_')
                {
                    tempBoard = currentBoard;
                    swap(tempBoard[redPieces[i].first], tempBoard[northWest]);
                    possibleMoves.push_back(tempBoard);
                }
            }
        }
    }

    // prioritize jumps
    if(!possibleJumps.empty())
    {
        sort(possibleJumps.begin(), possibleJumps.end());
        possibleJumps.erase(unique(possibleJumps.begin(), possibleJumps.end()), possibleJumps.end());
        return possibleJumps;
    }

    return possibleMoves;
}

vector<string> NeuralNetwork::generateMoves(string board)
{

    //string board = "rrrrrrrrrrrr________bbbbbbbbbbbb";     // regular board
    //string board = "______bBr_R_bbbB_____bB_________";     // to check jumps
    //string board = "______________r_b_b__Rr_bbb_____";     // red to king
    //string board = "_________R___bb______bb_________";     // king cycle (duplicate board?) //TODO

    //const char notKing = 'r';
    //const char king = 'R';

    // each int represents an index of the board string
    vector<pair<int,char>> redPieces;

    // Populate red pieces
    for(int i=0; i < board.size(); ++i)
    {
        if(board[i] == 'r')
        {
            redPieces.push_back(pair<int,char>(i, 'r'));
        }
        else if(board[i] == 'R')
        {
            redPieces.push_back(pair<int,char>(i, 'R'));
        }
    }

    // Tuple debugging
    //for(int i=0; i < redPieces.size(); ++i)
    //{
    //    cout << "tuple " << i << ": (" << redPieces[i].first << ", " << redPieces[i].second << ")" << endl;
    //}
    
    //for(auto move: possibleMoves)
    //{
    //    cout << move << endl;
    //}

    return generateMovesHelper(board, redPieces);
}

double NeuralNetwork::negaScout(Node &currentNode, int depth, double alpha, double beta)
{
    Node *child;
    double temp;
    
    vector<string> boards = generateMoves(currentNode.board);
    for(int i = 0; i < boards.size(); ++i) // create the vector of child nodes out of generated boards
    {
        currentNode.createChild(boards[i]);
    }
    
    // if we are a leaf or have reached our max depth
    if(currentNode.childNodes.size() == 0 || depth == 0)
    {
        return evaluateBoard(currentNode.board);
    }

    for(int i = 0; i < currentNode.childNodes.size(); ++i) // for each child node
    {
        child = &currentNode.childNodes[i];
        
        if(i != 0) // not first child
        {
            // -alpha-1.0 == -(alpha + 1.0) == -beta 
            // scout window
            temp = -negaScout(*child, depth-1, -alpha-1.0, -alpha);
            
            // if we're still between alpha and beta
            if(alpha < temp && temp < beta)
            {
                // do a research
                temp = -negaScout(*child, depth-1, -beta, -temp);
            }
        }
        else // is first child
        {
            temp = -negaScout(*child, depth-1, -beta, -alpha);
        }
        
        alpha = max(alpha, temp);
        
        if(alpha >= beta)
        {
            break; // prune
        }
    }

    return alpha;
}

string NeuralNetwork::treeSearch(string rootBoard)
{
    double alpha = -INF;
    double beta = INF;
    int depth = 2;
    string bestBoard = "";
    double temp;
    Node *child;

    Node root = Node(rootBoard);

    vector<string> boards = generateMoves(root.board);
    for(int i = 0; i < boards.size(); ++i) // create the vector of child nodes out of generated boards
    {
        root.createChild(boards[i]);
    }

    // if we are a leaf or have reached our max depth
    if(root.childNodes.size() == 0 || depth == 0)
    {
        return root.board;
        //return evaluateBoard(root.board);
    }

    for(int i = 0; i < root.childNodes.size(); ++i) // for each child node
    {
        child = &root.childNodes[i];
        
        if(i != 0) // not first child
        {
            // -alpha-1.0 == -(alpha + 1.0) == -beta 
            // scout window
            temp = -negaScout(*child, depth-1, -alpha-1.0, -alpha);
            
            // if we're still between alpha and beta
            if(alpha < temp && temp < beta)
            {
                // do a research
                temp = -negaScout(*child, depth-1, -beta, -temp);
            }
        }
        else // is first child
        {
            temp = -negaScout(*child, depth-1, -beta, -alpha);
        }
        
        //alpha = max(alpha, temp);
        if(temp > alpha)
        {
            alpha = temp;
            bestBoard = child->board; //save best board
        }
        
        if(alpha >= beta)
        {
            break; // prune
        }
    }

    return bestBoard;
}