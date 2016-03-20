// treeSearch.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile and run with:
// clang++ -Ofast --std=c++1y treeSearch.cpp -o treeSearch.o && time ./treeSearch.o

#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <unordered_map>
using std::unordered_map;
#include <limits>
using std::numeric_limits;
#include <utility>
using std::make_pair;
#include <algorithm>
using std::max;

#include "boardGeneration.cpp"

double INF = numeric_limits<double>::infinity();

double boardEval(string board) // dummy function for now
{
    srand (time(NULL));
    return rand();
}

struct Node
{
    Node(string newBoard, int newMaximize)
    {
        board = newBoard;
        maximize = newMaximize;
        value = boardEval(newBoard);
        parentNode = NULL;
    }
    
    void createChild(string newBoard)
    {
        Node temp(newBoard, -maximize);
        temp.parentNode = this;
        childNodes.push_back(temp);
    }
    
    string board;
    
    double value;
    
    double alpha;
    double beta;
    int maximize;
    
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
        
        cout << "B: " << board << " V: " << value << " A: " << alpha << " B: " << beta << endl;
        
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

double negaScout(Node &currentNode, int depth, double alpha, double beta, int maximize)
{
    Node *child;
    double temp;
    
    vector<string> boards = generateMovesHelper(currentNode.board);
    for(int i = 0; i < boards.size(); ++i) // create the vector of child nodes out of generated boards
    {
        currentNode.createChild(boards[i]);
    }
    
    // if we are a leaf or have reached our max depth
    if(currentNode.childNodes.size() == 0 || depth == 0)
    {
        return (double)maximize * currentNode.value;
    }
    
    for(int i = 0; i < currentNode.childNodes.size(); ++i) // for each child node
    {
        child = &currentNode.childNodes[i];
        
        if(i != 0) // not first child
        {
            // -alpha-1.0 == -(alpha + 1.0) == -beta 
            // scout window
            temp = -negaScout(*child, depth-1, -alpha-1.0, -alpha, -maximize);
            
            // if we're still between alpha and beta
            if(alpha < temp && temp < beta)
            {
                // do a research
                temp = -negaScout(*child, depth-1, -beta, -temp, -maximize);
            }
        }
        else // is first child
        {
            temp = -negaScout(*child, depth-1, -beta, -alpha, -maximize);
        }
        
        alpha = max(alpha, temp);
        
        if(alpha >= beta)
        {
            break; // prune
        }
    }
    
    if(currentNode.parentNode == NULL)
    {
        return alpha;
    }
    if(maximize) // we are max node and parent is min node
    {
        // update parent beta
        currentNode.parentNode->beta = alpha;
    }
    else
    {
        // update parent alpha
        currentNode.parentNode->alpha = alpha;
    }
    
    return alpha;
}

double negaScoutHelper(Node &root)
{
    double alpha = -INF;
    double beta = INF;
    
    int depth = 2;
    return negaScout(root, depth, alpha, beta, 1);
}

// https://en.wikipedia.org/wiki/Principal_variation_search
// https://en.wikipedia.org/wiki/Negamax#Negamax_with_alpha_beta_pruning
// http://stackoverflow.com/questions/18716465/implementing-alpha-beta-into-minimax
// http://eric-yuan.me/alpha-beta-pruning/
// https://chessprogramming.wikispaces.com/NegaScout
// http://www.ics.uci.edu/~rickl/courses/cs-171/2014-fq-cs171/2014-wq-cs171-lecture-slides/cs-171-07-Games.pdf
// http://arxiv.org/pdf/1404.1511.pdf

// Alpha beta explanation:      https://www.youtube.com/watch?v=xBXHtz4Gbdo
// PVS explanation:             https://www.youtube.com/watch?v=1YdBLgmoV_E

// http://will.thimbleby.net/algorithms/doku.php?id=minimax_search_with_alpha-beta_pruning

int main()
{
    string startBoard = "rrrrrrrrrrrr________bbbbbbbbbbbb";
    Node root = Node(startBoard, 1);

    cout << "Best board: " << negaScoutHelper(root) << "\n" << endl;
    
    root.printTree();
    
    /*
    unordered_map<string,Node> tree;
    tree["rrrrrrrrrrrr________bbbbbbbbbbbb"] = Node();
    cout << "tree['rrrrrrrrrrrr________bbbbbbbbbbbb'] is " << tree["rrrrrrrrrrrr________bbbbbbbbbbbb"].value << endl;
    tree["rrrrrrrrrrrr________bbbbbbbbbbbb"].childNodes = {"r","b"};
    // That will eventually look like this:
    // tree["rrrrrrrrrrrr________bbbbbbbbbbbb"].childNodes = generateBoards("rrrrrrrrrrrr________bbbbbbbbbbbb");
    cout << "tree['rrrrrrrrrrrr________bbbbbbbbbbbb'].childNodes[0] is " << tree["rrrrrrrrrrrr________bbbbbbbbbbbb"].childNodes[0] << endl;
    cout << "tree['rrrrrrrrrrrr________bbbbbbbbbbbb'].childNodes[1] is " << tree["rrrrrrrrrrrr________bbbbbbbbbbbb"].childNodes[1] << endl;
    */
    
    return 0;
}