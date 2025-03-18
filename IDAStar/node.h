#ifndef NODE_H
#define NODE_H

#include "board.h"


typedef struct Node{
    // for data of a node
    Board board;
    int piece;
    int ini_pos;
    int end_pos;
    int step;
    int remaining;
    int piecesLeft;
    int weight;

    /***get the parent and data structure for bfs***/
    struct Node* next;
    struct Node* parent;

}Node;

Node*createNode (Board board,int piece, int ini_pos, int end_pos, int step, int remaining, int piecesLeft, int weight, Node* parent);

#endif