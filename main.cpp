#include<bits/stdc++.h>
using namespace std;

struct BoardState {
    vector<int> board;
    int turn; // 1 for X, 2 for O
};

string drawBoard(const BoardState &state);

void makeMove(BoardState &state, int x, int y);

bool checkWin(const BoardState &state);

int maksMinAlfaBeta(BoardState &state, int depth, int alpha, int beta, bool maximizingPlayer);

int basicEvaluation(const BoardState &state);

int main() {
    bool game_over = false;
    // initilaze board
    BoardState board;
    board.board.assign(9, 0);
    board.turn = 1;
    while (!game_over) {
        int x, y;
        cout << drawBoard(board) << endl;
        if (checkWin(board)) {
            game_over = true;
            break;
        }
        if (board.turn == 1) {
            cout << "Player X's turn. Enter row and column (0-2): ";
            cin >> x >> y;
            makeMove(board, x, y);
        } else {
            cout << "Player O's turn. Enter row and column (0-2): ";
            cin >> x >> y;
            makeMove(board, x, y);
        }
    }
    return 0;
}

string drawBoard(const BoardState &state) {
    string board_str;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int cell = state.board[i * 3 + j];
            if (cell == 0) board_str += ".";
            else if (cell == 1) board_str += "X";
            else if (cell == 2) board_str += "O";
        }
        board_str += "\n";
    }
    return board_str;
}

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
    if (state.board[x * 3 + y] == 0) {
        state.board[x * 3 + y] = state.turn;
        state.turn = (state.turn == 1) ? 2 : 1; // Switch turns
    } else {
        cout << "Cell already occupied. Try again." << endl;
    }
}

int basicEvaluation(const BoardState &state) {
    int current_player = state.turn;
    if (checkWin(state)) {
        return 1; // Current player wins
    }
    //canot be determend
    return 0; // Placeholder: return 1 if current player wins, -1 if loses, 0 for draw or ongoing game

}

int maksMinAlfaBeta(BoardState &state, int depth, int alpha, int beta, bool maximizingPlayer) {
    int max_eval;
    int min_eval;
    int eval;
    if (depth == 0) {
        return basicEvaluation(state);
    }
    // maximizing player is X
    if (maximizingPlayer) {
        max_eval = INT_MIN;
        for (int i = 0; i < state.board.size(); ++i) {
            if (state.board[i] == 0) {
                BoardState new_state = state;
                new_state.board[i] = 1;
                eval = maksMinAlfaBeta(new_state, depth - 1, alpha, beta, false);
                max_eval = max(max_eval, eval);
                alpha = max(alpha, eval);
                if (beta <= alpha) {
                    break; // Beta cut-off
                }
                return max_eval;
            }
        }
    }
    //maximaizing player is O
    else {
        min_eval = INT_MAX;
        for (int i = 0; i < state.board.size(); ++i) {
            if (state.board[i] == 0) {
                BoardState new_state = state;
                new_state.board[i] = 2;
                eval = maksMinAlfaBeta(new_state, depth - 1, alpha, beta, true);
                min_eval = min(min_eval, eval);
                beta = min(beta, eval);
                if (beta <= alpha) {
                    break; // Alpha cut-off
                }
                return min_eval;
            }
        }
    }
    return 0; // Placeholder: return the best score for the current player and best turn
}
