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

float INF = numeric_limits<float>::infinity();

float boardEval(string board) // dummy function for now
{
    srand (time(NULL));
    return rand();
}

struct Node
{
    string board = "rrrrrrrrrrrr________bbbbbbbbbbbb";
    float value = boardEval(board);
    vector<string> childNodes;
};

/*int negaScout(node, depth, alpha, beta, color)
{
    if()
}*/

int main()
{
    float alpha = -INF;
    float beta = INF;
    
    unordered_map<string,Node> tree;
    
    tree["rrrrrrrrrrrr________bbbbbbbbbbbb"] = Node();
    
    cout << "tree['rrrrrrrrrrrr________bbbbbbbbbbbb'] is " << tree["rrrrrrrrrrrr________bbbbbbbbbbbb"].value << endl;
    
    tree["rrrrrrrrrrrr________bbbbbbbbbbbb"].childNodes = {"r","b"};
    
    // That will eventually look like this:
    // tree["rrrrrrrrrrrr________bbbbbbbbbbbb"].childNodes = generateBoards("rrrrrrrrrrrr________bbbbbbbbbbbb");
    
    cout << "tree['rrrrrrrrrrrr________bbbbbbbbbbbb'].childNodes[0] is " << tree["rrrrrrrrrrrr________bbbbbbbbbbbb"].childNodes[0] << endl;
    cout << "tree['rrrrrrrrrrrr________bbbbbbbbbbbb'].childNodes[1] is " << tree["rrrrrrrrrrrr________bbbbbbbbbbbb"].childNodes[1] << endl;
    
    return 0;
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