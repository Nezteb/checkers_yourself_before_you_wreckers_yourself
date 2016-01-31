// Noah Betzen & Lonny Strunk
// CS 405
// Checkers AI

// Compile and run with:
// clang++ --std=c++1y main.cpp -o main.o && ./main.o

#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;

#define BOARD_SIZE 32

void move(string &board, int currentPos, int newPos)
{
    bool leftLean = ( (currentPos/4) % 2 == 0 );
    bool rightLean = ( (currentPos/4) % 2 != 0 );
    bool farLeft = ( currentPos == (currentPos/4) * 4 );
    bool farRight = ( currentPos == (currentPos/4) * 4 + 3 );

    // figure out next position
    /*if(  &&  )
    {
        // can only move right
        newPos = currentPos +
    }
    else if(  &&  )
    {
        // can only move left
    }
    else
    {
        // can move either
    }

    // figure out attacks
    if(board[newPosition] == 'b')
    {
        // manage attacks
    }*/

    // do the actual move
}

int main()
{
    string board = "rrrrrrrrrrrr________bbbbbbbbbbbb";

    return 0;
}
