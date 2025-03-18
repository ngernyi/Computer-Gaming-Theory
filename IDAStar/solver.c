#include "solver.h"
#include "step_table.h"
#include "board.h"
#include "timer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

int dice_seq[_dice_sequence_len_];
int goal_piece;
int path[_dice_sequence_len_][2];


#define TIME_LIMIT 10

// Function to check whether the target reaches (0,0)
int reachTarget(Board chessboard, int targetPiece) {
    return (chessboard.piece_position[targetPiece] == 0);
}

int mmax(int a, int b) {
    return (a > b) ? a : b;
}

int mmin(int a, int b) {
    return (a < b) ? a : b;
}

int getX(int position) {
    return position / 10;
}

int getY(int position) {
    return position % 10;
}

int getDistance(int a, int b){

    if (a==-1 || b==-1)
    {
        return 100;
        /* code */
    }
    
    int ax = getX(a);
    int ay = getY(a);

    int bx = getX(b);
    int by = getY(b);

    int deltaX = abs(ax - bx);
    int deltaY = abs(ay - by);

    // The base distance is the larger of the X or Y difference
    int distance = mmax(deltaX, deltaY);

    return distance;
    

}

// Function to detect if a piece is eaten
int eaten(Board board, int moves) {
    for (size_t i = 0; i < 6; i++) {
        if (board.piece_position[i] == moves) {
            return 1;
        }
    }
    return 0;
}

// number of steps remaining to reach the target
int getRemaining(int position) {
    int x = getX(position);
    int y = getY(position);
    if (x == y && x > 3) {
        return (x + 1);
    } else {
        return (x > y) ? x : y;
    }
}



// number of impossible dice moves for the target piece
int impossibleDice(prob_layout problem_layout, Board chessboard, int step, int t) {
    int d[6] = {0};
    int counter = problem_layout.goal_piece;

    // Forward pass
    while (counter < 6) {
        d[counter] += (counter > 0) ? d[counter - 1] : 0;
        if (chessboard.piece_position[counter] != -1) {
            d[counter]++;
        }
        counter++;
    }

    // Backward pass
    counter = problem_layout.goal_piece - 2;
    while (counter >= 0) {
        d[counter] += (counter < 5) ? d[counter + 1] : 0;
        if (chessboard.piece_position[counter] != -1) {
            d[counter]++;
        }
        counter--;
    }

    // Count impossible moves
    int ans = 0;
    for (int i = 0; i < t; i++) {
        if (d[problem_layout.dice_sequence[i + step] - 1] > i) {
            ans++;
        }
    }

    return ans;
}

// Perform recursive depth-limited search for IDA*
int depthLimitedSearch(prob_layout problem_layout, Board chessboard, int step, int limit, int target, int t, struct timespec start) {
    // Timer to check for time limit
    struct timespec end;
    clock_gettime(CLOCK_REALTIME, &end);
    float elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1.0e9;

    // Time limit check
    if (elapsed_time > 9.8) {
        printf("%.6f\n", elapsed_time);
        printf("0");
        exit(0);  // Exit if time limit is exceeded
    }

    // Heuristic values
    int remainingSteps = getRemaining(chessboard.piece_position[target]);
    int piecesLeft = impossibleDice(problem_layout, chessboard, step ,t);
    // int piecesLeft = impossibleDice22(problem_layout,chessboard,step,remainingSteps);
    int f = step + remainingSteps + piecesLeft;

    if (f > limit) {
        return f;  // Return next threshold value
    }

    if (reachTarget(chessboard, target)) {
        // Print the steps and timing when the goal is reached
        printf("%.6f\n", elapsed_time);
        printf("%d\n", step);
        for (int i = 0; i < step; i++) {
            printf("%d%d %d%d ", getX(path[i][0]), getY(path[i][0]), getX(path[i][1]), getY(path[i][1]));
        }
        printf("\n");
        return -1;  // Goal reached
    }

    int minThreshold = INT_MAX;
    int moves[16][2];
    int moveCount = gen_moves(&chessboard, dice_seq[step], moves);

    for (int i = 0; i < moveCount; i++) {
    Board newBoard;
    memcpy(&newBoard, &chessboard, sizeof(Board));
    move(&newBoard, moves[i]);

    if (newBoard.piece_position[target] == -1) {
        continue;  // Skip if target piece is eaten
    }

    // Record the current move (ini_pos -> end_pos) temporarily in path
    path[step][0] = chessboard.piece_position[moves[i][0]] ;
    path[step][1] = moves[i][1];
   

    // Recursive call for the next depth level
    int result = depthLimitedSearch(problem_layout, newBoard, step + 1, limit, target, t, start);

    if (result == -1) {
        // If a valid solution is found, return and keep the path
        return -1;  // Goal found
    }

  
    minThreshold = mmin(minThreshold, result);
       
    }
    return minThreshold;
}

// IDA* search function
void idaStar(prob_layout problem_layout, Board chessboard, int target) {
    // Initial heuristic calculation
    int t = mmax((target + 1), (6 - target + 1));
    int threshold = getRemaining(chessboard.piece_position[target]) + impossibleDice(problem_layout, chessboard, 0,t);

    struct timespec start;
    clock_gettime(CLOCK_REALTIME, &start);  // Start timing

    // Iterative deepening
    while (1) {
        int result = depthLimitedSearch(problem_layout, chessboard, 0, threshold, target, t, start);

        if (result == -1) {
            return;  // Solution found
        }

        if (result == 30) {
            return;
        }

        threshold = result;  // Increase threshold for the next iteration
    }
}

void solve(prob_layout problem_layout) {
    // Initialize the chessboard
    Board chessboard;
    chessboard.piece_bits = 0x3F;  // Assuming this is your initial setup
    memcpy(chessboard.piece_position, problem_layout.piece_position, sizeof(problem_layout.piece_position));
    memcpy(dice_seq, problem_layout.dice_sequence, sizeof(dice_seq));

    // Call IDA* search
    int target = problem_layout.goal_piece - 1;
    idaStar(problem_layout, chessboard, target);
}
