#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"
#include "board.h"

Node* createNode(Board board,int piece, int ini_pos, int end_pos, int step, int remaining, int piecesLeft ,int weight, Node* parent){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode -> piece = piece;
    newNode -> ini_pos = ini_pos;
    newNode -> end_pos = end_pos;
    newNode -> board = board;
    newNode -> step = step;
    newNode -> remaining = remaining;
    newNode -> piecesLeft = piecesLeft;
    newNode -> parent = parent;
    newNode -> next = NULL;
    newNode -> weight = weight;
    return newNode;
}