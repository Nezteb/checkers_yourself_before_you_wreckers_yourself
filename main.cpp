// Noah Betzen & Lonny Strunk
// CS 405
// Checkers AI

// Compile and run with:
// clang++ -Ofast --std=c++1y main.cpp -o main.o && ./main.o


// char board[8][8] = {R,r,_,b,B} // Pro: simple math moves/jumps, easy print
//                                   Cons: memory usage, convert for Net
// char board[32] =    "     "    // Pro: 1/2 memory, I/O to Net
//


#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <algorithm>
using std::swap;
#include <utility>
using std::pair;

#define BOARD_SIZE 32

enum { NE=0, SE=1, SW=2, NW=3 };

// red piece move
// from top of board
// access with: movesBoard[pieceIndex * 4 + direction]
// pieceIndex: NE, SE, SW, NW

// each pair of four numbers represents one piece and its four directional moves (based on above enum)
vector<int> movesBoard = {
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

vector<string> generateJumps(const string &currentBoard, int currentPieceIndex)
{
    vector<string> possibleJumps;

    // todo

    return possibleJumps;
}

// checks every spot on the board for valid moves
// returns a vector of board strings
vector<string> generateMoves(const string &currentBoard, vector<pair<int,char>> &redPieces)
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

                // todo

                if(currentBoard[nextSouthEast] == '_')
                {
                    tempBoard = currentBoard;
                    tempBoard[southEast] = '_';
                    swap(tempBoard[i], tempBoard[nextSouthEast]);

                    vector<string> tempJumps = generateJumps(tempBoard, nextSouthEast);
                    possibleJumps.insert(possibleJumps.end(), tempJumps.begin(), tempJumps.end());
                }
            }
            if (currentBoard[southWest] == 'b' || currentBoard[southWest] == 'B')
            {
                // todo
            }
            if (possibleJumps.empty())
            {
                if (currentBoard[southEast] == '_')
                {
                    tempBoard = currentBoard;
                    swap(tempBoard[i], tempBoard[southEast]);
                    possibleMoves.push_back(tempBoard);
                }
                if (currentBoard[southWest] == '_')
                {
                    tempBoard = currentBoard;
                    swap(tempBoard[i], tempBoard[southWest]);
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
                tempBoard = currentBoard;
                swap(tempBoard[i], tempBoard[southEast]);
                possibleMoves.push_back(tempBoard);
            }
            if (currentBoard[northWest] == 'b' || currentBoard[northWest] == 'B')
            {
                tempBoard = currentBoard;
                swap(tempBoard[i], tempBoard[southWest]);
                possibleMoves.push_back(tempBoard);
            }
            if(possibleJumps.empty())
            {
                if (currentBoard[northEast] == '_')
                {
                    tempBoard = currentBoard;
                    swap(tempBoard[i], tempBoard[northEast]);
                    possibleMoves.push_back(tempBoard);
                }
                if (currentBoard[northWest] == '_')
                {
                    tempBoard = currentBoard;
                    swap(tempBoard[i], tempBoard[northWest]);
                    possibleMoves.push_back(tempBoard);
                }
            }
        }
    }

    return possibleMoves;
}

int main()
{
    string board = "rrrrrrrrrrrr________bbbbbbbbbbbb";

    const char notKing = 'r';
    const char king = 'R';

    // each int represents an index of the board string
    vector<pair<int,char>> redPieces;

    // Populate red pieces
    for(int i=0; i < board.size(); ++i)
    {
        if(board[i] == notKing)
        {
            redPieces.push_back(pair<int,char>(i, notKing));
        }
        else if(board[i] == king)
        {
            redPieces.push_back(pair<int,char>(i, king));
        }
    }

    // Tuple debugging
    /*for(int i=0; i < redPieces.size(); ++i)
    {
        cout << "tuple " << i << ": (" << redPieces[i].first << ", " << redPieces[i].second << ")" << endl;
    }*/

    vector<string> possibleMoves = generateMoves(board, redPieces);

    for(auto move: possibleMoves)
    {
        cout << move << endl;
    }

    return 0;
}
