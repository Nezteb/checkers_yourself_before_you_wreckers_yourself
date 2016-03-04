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
    Node(string newBoard)
    {
        board = newBoard;
    }
    
    string board = "";
    vector<Node> childNodes;
};

double score;

double negaScout(Node &node, int depth, double alpha, double beta, int maximize)
{
    vector<string> boards = generateMovesHelper(node.board);
    for(int i = 0; i < boards.size(); ++i) // create the vector of child nodes out of generated boards
    {
        node.childNodes.push_back(Node(boards[i]));
    }
    
    if(node.childNodes.size() == 0 || depth == 0)
    {
        return maximize * boardEval(node.board);
    }
    
    for(int i = 0; i < node.childNodes.size(); ++i) // for each child node
    {
        Node child = node.childNodes[i];
        
        if(i != 0) // not first child
        {
            score = -negaScout(child, depth-1, -alpha-1, -alpha, -maximize);
            
            if(alpha < score && score < beta)
            {
                score = -negaScout(child, depth-1, -beta, -score, -maximize);
            }
        }
        else
        {
            score = -negaScout(child, depth-1, -beta, -alpha, -maximize);
        }
        
        alpha = max(alpha, score);
        if(alpha >= beta)
        {
            break;
        }
    }
    
    return alpha;
}

string negaScoutHelper(Node &root, double alpha, double beta)
{
    
    negaScout(root, 5, alpha, beta, 1);
    return bestBoard;
}

/*
(* Negascout is also termed Principal Variation Search - hence - pvs *)

function pvs(node, depth, α, β, color)
    if node is a terminal node or depth = 0
        return color × the heuristic value of node
    for each child of node
        if child is not first child
            score := -pvs(child, depth-1, -α-1, -α, -color)       (* search with a null window *)
            if α < score < β                                      (* if it failed high,
                score := -pvs(child, depth-1, -β, -score, -color)        do a full re-search *)
        else
            score := -pvs(child, depth-1, -β, -α, -color)
        α := max(α, score)
        if α ≥ β
            break                                            (* beta cut-off *)
    return α
*/

// https://en.wikipedia.org/wiki/Principal_variation_search
// https://en.wikipedia.org/wiki/Negamax#Negamax_with_alpha_beta_pruning
// http://stackoverflow.com/questions/18716465/implementing-alpha-beta-into-minimax
// http://eric-yuan.me/alpha-beta-pruning/

// Alpha beta explanation:      https://www.youtube.com/watch?v=xBXHtz4Gbdo
// PVS explanation:             https://www.youtube.com/watch?v=1YdBLgmoV_E

int main()
{
    double alpha = -INF;
    double beta = INF;
    
    string startBoard = "rrrrrrrrrrrr________bbbbbbbbbbbb";
    Node root = Node(startBoard);

    cout << "Best board: " << negaScout(root, alpha, beta);
    
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