//
// Created by enejk on 3/7/2026.
//

#ifndef TICTACTOEAI_TICTACTOE_H
#define TICTACTOEAI_TICTACTOE_H
#include <climits>
#include <iostream>
#include <algorithm>
using namespace std;

struct BoardState {
    int board[9] = {0}; // 0 = empty, 1 = player 1, 2 = player 2
    int turn = 1; // 1 or 2
};

// Free function declarations
bool checkWin(const BoardState &state);
void makeMove(BoardState &state, int x, int y);
void makeMove(BoardState &state, int index);
int countFavorableLines(const BoardState &state, int player);
int basicEvaluation(const BoardState &state);
int maksMinAlfaBeta(BoardState &state, int depth, int alpha, int beta, bool maximizingPlayer);

#endif //TICTACTOEAI_TICTACTOE_H