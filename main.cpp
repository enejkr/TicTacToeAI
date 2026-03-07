#include "ticTacToe.h"
#include <string>

string drawBoard(const BoardState &state);

// main game loop
int main() {
    // initialize board
    BoardState board;
    board.turn = 1;
    int depth = 0;
    while (true) {
        // v prvi iteraciji doliči globina AI
        if (depth == 0) {
            cout << "Enter search depth for AI: 1 - 9 " << '\n';
            cin >> depth;
        }
        int x, y;
        // izrišemo trenutno stanje plošče
        cout << drawBoard(board) << '\n';
        // preverimo win con
        if (checkWin(board)) {
            // board.turn was already flipped after the winning move
            cout << ((board.turn == 1) ? "O wins!" : "X wins!") << '\n';
            break;
        }
        // Check draw
        bool anyEmpty = false;
        // preverimo draw con
        for (int i = 0; i < 9; ++i)
            if (board.board[i] == 0) {
                anyEmpty = true;
                break;
            }
        if (!anyEmpty) {
            cout << "It's a draw!\n";
            break;
        }
        // player trun
        if (board.turn == 1) {
            cout << "Player X's turn. Enter row and column (0-2): ";
            cin >> x >> y;
            makeMove(board, x, y);
        }
        // MaxMin turn
        else {
            // AI plays as O (minimizing player)
            int best_score = INT_MAX;
            bool is_best_empty = true;
            int best_move = -1;
            for (int i = 0; i < 9; ++i) {
                if (board.board[i] == 0) {
                    BoardState new_state = board;
                    new_state.board[i] = 2;
                    new_state.turn = 1;
                    int score = maksMinAlfaBeta(new_state, depth - 1, INT_MIN, INT_MAX, true);
                    if (score < best_score || is_best_empty) {
                        best_score = score;
                        best_move = i;
                        is_best_empty = false;
                    }
                }
            }
            if (best_move != -1) {
                board.board[best_move] = 2;
                board.turn = 1;
                cout << "AI plays at row " << best_move / 3 << " col " << best_move % 3 << '\n';
            }
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

