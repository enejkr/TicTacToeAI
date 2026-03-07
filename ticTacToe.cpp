//
// Created by enejk on 3/7/2026.
//

#include "ticTacToe.h"
bool checkWin(const BoardState &state) {
    // Check rows, columns, and diagonals for a win
    for (int i = 0; i < 3; ++i) {
        if (state.board[i * 3] != 0 && state.board[i * 3] == state.board[i * 3 + 1] && state.board[i * 3 + 1] == state.
            board[i * 3 + 2]) {
            return true;
        }
        if (state.board[i] != 0 && state.board[i] == state.board[i + 3] && state.board[i + 3] == state.board[i + 6]) {
            return true;
        }
    }
    if (state.board[0] != 0 && state.board[0] == state.board[4] && state.board[4] == state.board[8]) {
        return true;
    }
    if (state.board[2] != 0 && state.board[2] == state.board[4] && state.board[4] == state.board[6]) {
        return true;
    }
    return false;
}

void makeMove(BoardState &state, int x, int y) {
    // 0 1 2
    // 3 4 5
    // 6 7 8
    if (state.board[x * 3 + y] == 0) {
        state.board[x * 3 + y] = state.turn;
        state.turn = (state.turn == 1) ? 2 : 1; // Switch turns
    } else {
        cout << "Cell already occupied. Try again." << '\n';
    }
}
void makeMove(BoardState &state, int index) {
    if (state.board[index] == 0) {
        state.board[index] = state.turn;
        state.turn = (state.turn == 1) ? 2 : 1; // Switch turns
    } else {
        cout << "Cell already occupied. Try again." << '\n';
    }
}

int countFavorableLines(const BoardState &state, int player) {
    int opponent = (player == 1) ? 2 : 1;
    int count = 0;

    auto lineOpen = [&](int a, int b, int c) -> bool {
        return state.board[a] != opponent &&
               state.board[b] != opponent &&
               state.board[c] != opponent &&
               (state.board[a] == player || state.board[b] == player || state.board[c] == player);
    };

    for (int i = 0; i < 3; i++) {
        //row
        if (lineOpen(i * 3, i * 3 + 1, i * 3 + 2)) {
            count++;
        }
        // columns
        if (lineOpen(i, i + 3, i + 6)) {
            count++;
        }
    }
    // diagonals
    if (lineOpen(0, 4, 8)) count++;
    if (lineOpen(2, 4, 6)) count++;

    return count;
}

//basic heuristic function
int basicEvaluation(const BoardState &state) {
    int xLines = countFavorableLines(state, 1);
    int oLines = countFavorableLines(state, 2);
    return xLines - oLines;
}
// IIMPOTANT SMALLER IS BETTER FOR AI (O) AND LARGER IS BETTER FOR PLAYER (X)
// MinMax with alpha-beta pruning
// state: current board state
// depth: how many moves ahead to evaluate
// alpha: best score for maximizing player (X)
// beta: best score for minimizing player (O)
// maximizingPlayer: true if it's X's turn, false if it's O's turn
int maksMinAlfaBeta(BoardState &state, int depth, int alpha, int beta, bool maximizingPlayer) {
    //nemorem več uporabiti INT_MAX/INT_MIN (upoštevam globino) prej k zmaga boljš je
    //X
    {
        BoardState tmp = state;
        tmp.turn = 1;
        if (checkWin(tmp)) {
            return -1000 - depth;
        }
    }
    //O
    {
        BoardState tmp = state;
        tmp.turn = 2;
        if (checkWin(tmp)) {
            return 1000 + depth;
        }
    }
    //preverim draw
    bool hasEmpty = false;
    for (int i = 0; i < 9; ++i) {
        if (state.board[i] == 0) {
            hasEmpty = true;
            break;
        }
    }

    //draw ma vrednost 0
    if (!hasEmpty) {
        return 0;
    }

    if (depth == 0) {
        return basicEvaluation(state);
    }

    if (maximizingPlayer) {
        int max_eval = INT_MIN;
        for (int i = 0; i < 9; ++i) {
            if (state.board[i] == 0) {
                BoardState new_state = state;
                new_state.board[i] = 1; // X igra
                new_state.turn = 2;
                int eval = maksMinAlfaBeta(new_state, depth - 1, alpha, beta, false);
                max_eval = max(max_eval, eval);
                alpha = max(alpha, eval);
                if (beta <= alpha) break; // beta cut-off
            }
        }
        return max_eval;
    } else {
        int min_eval = INT_MAX;
        for (int i = 0; i < 9; ++i) {
            if (state.board[i] == 0) {
                BoardState new_state = state;
                new_state.board[i] = 2; // O igra
                new_state.turn = 1;
                int eval = maksMinAlfaBeta(new_state, depth - 1, alpha, beta, true);
                min_eval = min(min_eval, eval);
                beta = min(beta, eval);
                if (beta <= alpha) break; // alpha cut-off
            }
        }
        return min_eval;
    }
}
