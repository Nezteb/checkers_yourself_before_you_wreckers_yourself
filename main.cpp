// Noah Betzen & Lonny Strunk
// CS 405
// Checkers AI

// Compile and run with:
// clang++ --std=c++1y main.cpp -o main.o && ./main.o


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

#define BOARD_SIZE 32

enum {NE=0,SE=1,SW=2,NW=3};


// Red piece move
// from top of board
// access with: move_board[piece_index * 4 + direction]
// index: NE, SE, SW, NW
vector<int> load_move_validator()
{
    vector<int> move_board_temp = {
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
    return move_board_temp;
}


// checks every spot on the board for valid moves 
// returns a vector of board strings
// ??can be modified to only check spots with a red piece??
vector<string> move_generator(const string & curr_board_string)
{
    vector<char> curr_board(curr_board_string.begin(), curr_board_string.end());
    vector<int> move_board = load_move_validator();
    vector<string> possible_moves;


    for (int i = 0; i < curr_board.size(); ++i) 
    {

        if (curr_board[i] == 'r')
        {
            vector<int>::const_iterator first = move_board.begin() + i*4+1;
            vector<int>::const_iterator last = move_board.begin() + i*4+3;
            vector<int> move_piece(first, last);

            for(auto move: move_piece)
            {
                if (curr_board[ move ] == '_')
                {
                    string temp_board = curr_board_string;
                    swap(temp_board[i], temp_board[move]);
                    possible_moves.push_back(temp_board);
                } 
            }
        } 

        else if (curr_board[i] == 'R')
        {
            vector<int>::const_iterator first = move_board.begin() + i*4;
            vector<int>::const_iterator last = move_board.begin() + i*4+4;
            vector<int> move_piece(first, last);

            for(auto move: move_piece)
            {
                if (curr_board[ move ] == '_')
                {
                    string temp_board = curr_board_string;
                    swap(temp_board[i], temp_board[move]);
                    possible_moves.push_back(temp_board);
                } 
            }
        }
    }
    return possible_moves;
}

vector<string> jump_generator(const string & curr_board_string)
{
    vector<char> curr_board(curr_board_string.begin(), curr_board_string.end());
    vector<int> move_board = load_move_validator();
    vector<string> possible_moves;

    //To Do!!!!!!

    return possible_moves;
}


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

    vector<string> board_moves = move_generator(board);

    for(auto move: board_moves)
    {
        cout << move << endl;
    }

    return 0;
}







