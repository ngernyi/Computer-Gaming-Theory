#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"
#include "board.h"



Queue*createQueue(){
    Queue*queue = (Queue*)malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    queue->size = 0;
    return queue;
}

// void push(Queue* queue, Board board, int piece, int ini_pos, int end_pos, int step, int remaining, int up, int down, int piecesLeft, int diceRequire, int distance, int p[], int min, int weight, Node* parent){
//     Node* newNode = createNode(board ,piece, ini_pos, end_pos, step, remaining,up, down, piecesLeft, diceRequire, distance,p,min,  weight, parent);

//     if(queue->rear == NULL){
//         queue->front = queue->rear = newNode;
        
//     }
//     else{
//         queue->rear->next = newNode;
//         queue->rear = newNode;
//     }
//     queue->size++;
// }

Node* pop(Queue* queue){
    if (queue->front == NULL)
    {
        return NULL;
        /* code */
    }

    Node* temp = queue->front;
    queue-> front = queue->front->next;

    if (queue->front == NULL)
    {
        queue -> rear = NULL;
        /* code */
    }
    queue->size--;
    
    return temp;
    
    
}

int isEmpty (Queue* queue){
    if (queue -> front == NULL)
    {
        return 1;
        /* code */
    }
    return 0;
    
}

long size(Queue* queue){
    return queue->size;
}
