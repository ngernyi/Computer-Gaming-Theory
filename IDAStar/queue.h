#ifndef QUEUE_H
#define QUEUE_H

#include "node.h"
#include "board.h"

typedef struct Queue
{
    Node*front;
    Node*rear;
    int size;
    
}Queue;

Queue* createQueue();
void push(Queue* queue, Board board, int piece, int ini_pos, int end_pos,int step, int remaining, int up, int down, int piecesLeft, int diceRequire, int dis, int weight, Node* parent);
Node* pop(Queue* queue);
int isEmpty(Queue* queue);
long size(Queue* queue);

#endif