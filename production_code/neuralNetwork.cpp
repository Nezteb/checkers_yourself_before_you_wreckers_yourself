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

#include <array>
using std::array;
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
        //parentNode = NULL;
    }
    
    ~Node()
    {
        //parentNode = NULL;
        for(auto child : childNodes)
        {
            delete child; // kill the child
        }
    }
    
    void createChild(string newBoard)
    {
        Node *temp = new Node(newBoard);
        //temp->parentNode = this;
        childNodes.push_back(temp);
    }
    
    string board;
    
    //Node *parentNode;
    vector<Node*> childNodes;
    
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
            
            childNodes[i]->printTree(tab + newTab, i + 1 == childNodes.size());
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
                _layers[0](0, i) = -1.0;
                break;
            case 'R':
                _layers[0](0, i) = _kingValue;
                break;
            case 'b':
                _layers[0](0, i) = 1.0;
                break;
            case 'B':
                _layers[0](0, i) = -_kingValue;
                break;
            case '_':
                _layers[0](0, i) = 0.0;
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
        return MatrixXXd();
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
// access with: movesBoard[pieceIndex * 4 + direction]
// pieceIndex: NE, SE, SW, NW
// each group of four numbers represents one piece and its four directional moves (based on above enum)
const array<int, 128> movesBoard = {
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
            27, -1, -1, 26
};
const array<int, 128> jumpsBoard = {
            -1,  9, -1, -1,
            -1, 10,  8, -1,
            -1, 11,  9, -1,
            -1, -1, 10, -1,
            -1, 13, -1, -1,
            -1, 14, 12, -1,
            -1, 15, 13, -1,
            -1, -1, 14, -1,
             1, 17, -1, -1,
             2, 18, 16,  0,
             3, 19, 17,  1,
            -1, -1, 18,  2,
             5, 21, -1, -1,
             6, 22, 20,  4,
             7, 23, 21,  5,
            -1, -1, 22,  6,
             9, 25, -1, -1,
            10, 26, 24,  8,
            11, 27, 25,  9,
            -1, -1, 26, 10,
            13, 29, -1, -1,
            14, 30, 28, 12,
            15, 31, 29, 13,
            -1, -1, 30, 14,
            17, -1, -1, -1,
            18, -1, -1, 16,
            19, -1, -1, 17,
            -1, -1, -1, 18,
            21, -1, -1, -1,
            22, -1, -1, 20,
            23, -1, -1, 21,
            -1, -1, -1, 22
};

bool NeuralNetwork::pieceMove(vector<string> &jumps, vector<string> &moves, const string currentBoard, char piece, int index, bool isRed, bool &canJump)
{
    string temp = currentBoard;
    bool didJump = false;
    
    int northEast = movesBoard[index * 4 + NE];
    int southEast = movesBoard[index * 4 + SE];
    int southWest = movesBoard[index * 4 + SW];
    int northWest = movesBoard[index * 4 + NW];

    int jumpNorthEast = jumpsBoard[index * 4 + NE];
    int jumpSouthEast = jumpsBoard[index * 4 + SE];
    int jumpSouthWest = jumpsBoard[index * 4 + SW];
    int jumpNorthWest = jumpsBoard[index * 4 + NW];
    
    // ######################################## JUMPS ########################################
    
    // ######################################## RED JUMP ########################################
    if(isRed)
    {
        char pawn = 'r'; char king = 'R';
        char enemyPawn = 'b'; char enemyKing = 'B';
        
        // king northeast jump
        if( (northEast != -1) && (jumpNorthEast != -1) &&
            (temp[index] == king) &&
            (temp[northEast] == enemyPawn || temp[northEast] == enemyKing) &&
            (temp[jumpNorthEast] == '_')
        )
        {
            canJump = true;
            temp[northEast] = '_';
            swap(temp[index], temp[jumpNorthEast]);
            
            didJump = true;
            if (!pieceMove(jumps, moves, temp, piece, jumpNorthEast, isRed, canJump)) //if did not jump
            {
                jumps.push_back(temp); //record jump
            }
            
            temp = currentBoard;
        }
        // king northwest jump
        if( (northWest != -1) && (jumpNorthWest != -1) &&
            (temp[index] == king) &&
            (temp[northWest] == enemyPawn || temp[northWest] == enemyKing) &&
            (temp[jumpNorthWest] == '_')
        )
        {
            canJump = true;
            temp[northWest] = '_';
            swap(temp[index], temp[jumpNorthWest]);
            
            didJump = true;
            if (!pieceMove(jumps, moves, temp, piece, jumpNorthWest, isRed, canJump)) //if did not jump
            {
                jumps.push_back(temp); //record jump
            }
            
            temp = currentBoard;
        }
        // king/pawn southeast jump
        if( (southEast != -1) && (jumpSouthEast != -1) &&
            (temp[index] == king || temp[index] == pawn) &&
            (temp[southEast] == enemyPawn || temp[southEast] == enemyKing) &&
            (temp[jumpSouthEast] == '_')
        )
        {
            canJump = true;
            temp[southEast] = '_';
            swap(temp[index], temp[jumpSouthEast]);
            
            // promote pawn if in last row
            if( (temp[jumpSouthEast] == pawn) && (jumpSouthEast >= 28) )
            {
                temp[jumpSouthEast] = king;
                jumps.push_back(temp);
            }
            else
            {
                didJump = true;
                if (!pieceMove(jumps, moves, temp, piece, jumpSouthEast, isRed, canJump)) //if did not jump
                {
                    jumps.push_back(temp); //record jump
                }
            }
            
            temp = currentBoard;
        }
        // king/pawn southwest jump
        if( (southWest != -1) && (jumpSouthWest != -1) &&
            (temp[index] == king || temp[index] == pawn) &&
            (temp[southWest] == enemyPawn || temp[southWest] == enemyKing) &&
            (temp[jumpSouthWest] == '_')
        )
        {
            canJump = true;
            temp[southWest] = '_';
            swap(temp[index], temp[jumpSouthWest]);
            
            // promote pawn if in last row
            if( (temp[jumpSouthWest] == pawn) && (jumpSouthWest >= 28) )
            {
                temp[jumpSouthWest] = king;
                jumps.push_back(temp);
            }
            else
            {
                didJump = true;
                if (!pieceMove(jumps, moves, temp, piece, jumpSouthWest, isRed, canJump)) //if did not jump
                {
                    jumps.push_back(temp); //record jump
                }
            }
            
            temp = currentBoard;
        }
    }
    // ######################################## BLACK JUMP ########################################
    else
    {
        char pawn = 'b'; char king = 'B';
        char enemyPawn = 'r'; char enemyKing = 'R';
        
        // king southeast jump
        if( (southEast != -1) && (jumpSouthEast != -1) &&
            (temp[index] == king) &&
            (temp[southEast] == enemyPawn || temp[southEast] == enemyKing) &&
            (temp[jumpSouthEast] == '_')
        )
        {
            canJump = true;
            temp[southEast] = '_';
            swap(temp[index], temp[jumpSouthEast]);
            
            didJump = true;
            if (!pieceMove(jumps, moves, temp, piece, jumpSouthEast, isRed, canJump)) //if did not jump
            {
                jumps.push_back(temp); //record jump
            }
            
            temp = currentBoard;
        }
        // king southwest jump
        if( (southEast != -1) && (jumpSouthWest != -1) &&
            (temp[index] == king) &&
            (temp[southWest] == enemyPawn || temp[southWest] == enemyKing) &&
            (temp[jumpSouthWest] == '_')
        )
        {
            canJump = true;
            temp[southWest] = '_';
            swap(temp[index], temp[jumpSouthWest]);
            
            didJump = true;
            if (!pieceMove(jumps, moves, temp, piece, jumpSouthWest, isRed, canJump)) //if did not jump
            {
                jumps.push_back(temp); //record jump
            }
            
            temp = currentBoard;
        }
        // king/pawn northeast jump
        if( (northEast != -1) && (jumpNorthEast != -1) &&
            (temp[index] == king || temp[index] == pawn) &&
            (temp[northEast] == enemyPawn || temp[northEast] == enemyKing) &&
            (temp[jumpNorthEast] == '_')
        )
        {
            canJump = true;
            temp[northEast] = '_';
            swap(temp[index], temp[jumpNorthEast]);
            
            // promote pawn if in last row
            if( (temp[jumpNorthEast] == pawn) && (jumpNorthEast <= 3) )
            {
                temp[jumpNorthEast] = king;
                jumps.push_back(temp);
            }
            else
            {
                didJump = true;
                if (!pieceMove(jumps, moves, temp, piece, jumpNorthEast, isRed, canJump)) //if did not jump
                {
                    jumps.push_back(temp); //record jump
                }
            }
            
            temp = currentBoard;
        }
        // king/pawn northwest jump
        if( (northWest != -1) && (jumpNorthWest != -1) &&
            (temp[index] == king || temp[index] == pawn) &&
            (temp[northWest] == enemyPawn || temp[northWest] == enemyKing) &&
            (temp[jumpNorthWest] == '_')
        )
        {
            canJump = true;
            temp[northWest] = '_';
            swap(temp[index], temp[jumpNorthWest]);
            
            // promote pawn if in last row
            if( (temp[jumpNorthWest] == pawn) && (jumpNorthWest <= 3) )
            {
                temp[jumpNorthWest] = king;
                jumps.push_back(temp);
            }
            else
            {
                didJump = true;
                if (!pieceMove(jumps, moves, temp, piece, jumpNorthWest, isRed, canJump)) //if did not jump
                {
                    jumps.push_back(temp); //record jump
                }
            }
            
            temp = currentBoard;
        }
    }
    
    // ######################################## MOVES ########################################
    if(!canJump)
    {
        // ######################################## RED MOVES ########################################
        if(isRed)
        {
            char pawn = 'r'; char king = 'R';
            char enemyPawn = 'b'; char enemyKing = 'B';
            
            // king northeast move
            if( (northEast !=  -1)  &&
                (temp[index] == king) &&
                (temp[northEast] == '_')
            )
            {
                swap(temp[index], temp[northEast]);
                moves.push_back(temp);
                temp = currentBoard;
            }
            // king northwest move
            if( (northWest != -1) &&
                (temp[index] == king) &&
                (temp[northWest] == '_')
            )
            {
                swap(temp[index], temp[northWest]);
                moves.push_back(temp);
                temp = currentBoard;
            }
            // king/pawn southeast move
            if( (southEast != -1) &&
                (temp[index] == king || temp[index] == pawn) &&
                (temp[southEast] == '_')
            )
            {
                swap(temp[index], temp[southEast]);
                
                // promote pawn if in last row
                if( (temp[southEast] == pawn) && (southEast >= 28) )
                {
                    temp[southEast] = king;
                }
                
                moves.push_back(temp);
                temp = currentBoard;
            }
            // king/pawn southwest move
            if( (southWest != -1) &&
                (temp[index] == king || temp[index] == pawn) &&
                (temp[southWest] == '_')
            )
            {
                swap(temp[index], temp[southWest]);
                
                // promote pawn if in last row
                if( (temp[southWest] == pawn) && (southWest >= 28) )
                {
                    temp[southWest] = king;
                }
                
                moves.push_back(temp);
                temp = currentBoard;
            }
        }
        // ######################################## BLACK MOVES ########################################
        else
        {
            char pawn = 'b'; char king = 'B';
            char enemyPawn = 'r'; char enemyKing = 'R';
            
            // king southeast move
            if( (southEast != -1) &&
                (temp[index] == king) &&
                (temp[southEast] == '_')
            )
            {
                swap(temp[index], temp[southEast]);
                moves.push_back(temp);
                temp = currentBoard;
            }
            // king southwest move
            if( (southWest != -1) &&
                (temp[index] == king) &&
                (temp[southWest] == '_')
            )
            {
                swap(temp[index], temp[southWest]);
                moves.push_back(temp);
                temp = currentBoard;
            }
            // king/pawn northeast move
            if( (northEast != -1) &&
                (temp[index] == king || temp[index] == pawn) &&
                (temp[northEast] == '_')
            )
            {
                swap(temp[index], temp[northEast]);
                
                // promote pawn if in last row
                if( (temp[northEast] == pawn) && (northEast <= 3) )
                {
                    temp[northEast] = king;
                }
                
                moves.push_back(temp);
                temp = currentBoard;
            }
            // king/pawn northwest move
            if( (northWest != -1) &&
                (temp[index] == king || temp[index] == pawn) &&
                (temp[northWest] == '_')
            )
            {
                swap(temp[index], temp[northWest]);
                
                // promote pawn if in last row
                if( (temp[northWest] == pawn) && (northWest <= 3) )
                {
                    temp[northWest] = king;
                }
                
                moves.push_back(temp);
                temp = currentBoard;
            }
        }
    }
    
    return didJump;
}

// checks every spot on the board for valid moves
// returns a vector of board strings
vector<string> NeuralNetwork::generateMovesHelper(const string currentBoard, vector<pair<int,char>> &pieces, bool isRed)
{
    vector<string> jumps;
    vector<string> moves;
    
    bool canJump = false;
    
    for (int i = 0; i < pieces.size(); ++i)
    {
        pair<int,char> piece = pieces[i];
        
        pieceMove(jumps, moves, currentBoard, piece.second, piece.first, isRed, canJump);
    }
    
    if(jumps.size() == 0)
    {
        return moves;
    }
    else
    {
        // jumps can be duplicated (jumping in a circle), so remove them
        sort(jumps.begin(), jumps.end());
        jumps.erase(unique(jumps.begin(), jumps.end()), jumps.end());
        return jumps;
    }
}

vector<string> NeuralNetwork::generateMoves(string board, bool isRed)
{
    // each int represents an index of the board string
    vector<pair<int,char>> pieces;
    
    // Populate pieces
    for(int i = 0; i < board.size(); ++i)
    {
        switch(board[i])
        {
            case 'r':
            case 'R':
                if(isRed) pieces.push_back(pair<int,char>(i, board[i]));
                break;
            case 'b':
            case 'B':
                if(!isRed) pieces.push_back(pair<int,char>(i, board[i]));
                
        }
    }
    
    return generateMovesHelper(board, pieces, isRed);
}

double NeuralNetwork::negaScout(Node *currentNode, int depth, double alpha, double beta, bool isRed)
{
    Node *child = NULL;
    double temp;
    
    vector<string> boards = generateMoves(currentNode->board, isRed);
    
    for(int i = 0; i < boards.size(); ++i) // create the vector of child nodes out of generated boards
    {
        // SORT BOARDS/NODES HERE BEFORE ADDING THEM TO THE TREE
        currentNode->createChild(boards[i]);
    }
    
    // if we are a leaf or have reached our max depth
    if(currentNode->childNodes.size() == 0 || depth == 0)
    {
        string temp = currentNode->board;
        return evaluateBoard(temp);
    }

    for(int i = 0; i < currentNode->childNodes.size(); ++i) // for each child node
    {
        child = currentNode->childNodes[i];
        
        if(i != 0) // not first child
        {
            // -alpha-1.0 == -(alpha + 1.0) == -beta 
            // scout window
            temp = -negaScout(child, depth-1, -alpha-1.0, -alpha, !isRed);
            
            // if we're still between alpha and beta
            if(alpha < temp && temp < beta)
            {
                // do a research
                temp = -negaScout(child, depth-1, -beta, -temp, !isRed);
            }
        }
        else // is first child
        {
            temp = -negaScout(child, depth-1, -beta, -alpha, !isRed);
        }
        
        alpha = max(alpha, temp);
        
        if(alpha >= beta)
        {
            break; // prune
        }
    }
    
    child = NULL;
    
    //for(auto child : currentNode->childNodes)
    //{
    //    delete child; // kill the child
    //}
    
    //delete currentNode;

    return alpha;
}

string NeuralNetwork::treeSearch(string rootBoard, int depth) // called once
{
    double alpha = -INF;
    double beta = INF;
    string bestBoard = "";
    double temp;
    Node *child;
    bool isRed = _isRed;

    Node *root = new Node(rootBoard);

    vector<string> boards = generateMoves(root->board, isRed);
    
    for(int i = 0; i < boards.size(); ++i) // create the vector of child nodes out of generated boards
    {
        root->createChild(boards[i]);
    }

    // if we are a leaf or have reached our max depth
    if(root->childNodes.size() == 0 || depth == 0)
    {
        return "";
        //return root->board;
        //return evaluateBoard(root.board);
    }

    for(int i = 0; i < root->childNodes.size(); ++i) // for each child node
    {
        child = root->childNodes[i];
        
        if(i != 0) // not first child
        {
            // -alpha-1.0 == -(alpha + 1.0) == -beta 
            // scout window
            temp = -negaScout(child, depth-1, -alpha-1.0, -alpha, !isRed);
            
            // if we're still between alpha and beta
            if(alpha < temp && temp < beta)
            {
                // do a research
                temp = -negaScout(child, depth-1, -beta, -temp, !isRed);
            }
        }
        else // is first child
        {
            temp = -negaScout(child, depth-1, -beta, -alpha, !isRed);
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
    
    child = NULL;

    return bestBoard;
}