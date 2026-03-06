#include<bits/stdc++.h>
using namespace std;

struct BoardState {
    vector<int> board;
    int turn = 1; // 1 for X, 2 for O
};

string drawBoard(const BoardState &state);

void makeMove(BoardState &state, int x, int y);

bool checkWin(const BoardState &state);

int maksMinAlfaBeta(BoardState &state, int depth, int alpha, int beta, bool maximizingPlayer);

int basicEvaluation(const BoardState &state);

int countFavorableLines(const BoardState &state);

// main game loop
int main() {
    // initialize board
    BoardState board;
    board.board.assign(9, 0);
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
        for (int el: board.board)
            if (el == 0) {
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
            int best_move = -1;
            for (int i = 0; i < 9; ++i) {
                if (board.board[i] == 0) {
                    BoardState new_state = board;
                    new_state.board[i] = 2;
                    new_state.turn = 1;
                    int score = maksMinAlfaBeta(new_state, depth - 1, INT_MIN, INT_MAX, true);
                    if (score < best_score) {
                        best_score = score;
                        best_move = i;
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

// check win con
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

int countFavorableLines(const BoardState &state, int player) {
    int opponent = (player == 1) ? 2 : 1;
    int count = 0;

    //helper lamda check if line is open for playe
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
    if (checkWin(state)) {
        return (state.turn == 1) ? -100 : 100;
    }
    // account for open lines
    int xLines = countFavorableLines(state, 1);
    int oLines = countFavorableLines(state, 2);
    return xLines - oLines;
}
// MinMax with alpha-beta pruning
// state: current board state
// depth: how many moves ahead to evaluate
// alpha: best score for maximizing player
// beta: best score for minimizing player
// maximizingPlayer: true if it's the maximizing player's turn (X), false for minimizing player (O)
int maksMinAlfaBeta(BoardState &state, int depth, int alpha, int beta, bool maximizingPlayer) {
    if (checkWin(state) || depth == 0) {
        return basicEvaluation(state);
    }
    // Check for draw (no empty cells)
    bool hasEmpty = false;
    for (int i = 0; i < 9; ++i) {
        if (state.board[i] == 0) {
            hasEmpty = true;
            break;
        }
    }
    // draw or depth limit with no win
    if (!hasEmpty || depth == 0) {
        return 0;
    }
    if (maximizingPlayer) {
        int max_eval = INT_MIN;
        for (int i = 0; i < 9; ++i) {
            if (state.board[i] == 0) {
                BoardState new_state = state;
                new_state.board[i] = 1;
                new_state.turn = 2;
                int eval = maksMinAlfaBeta(new_state, depth - 1, alpha, beta, false);
                max_eval = max(max_eval, eval);
                alpha = max(alpha, eval);

                if (beta <= alpha) break;
            }
        }
        return max_eval;
    } else {
        int min_eval = INT_MAX;
        for (int i = 0; i < 9; ++i) {
            if (state.board[i] == 0) {
                BoardState new_state = state;
                new_state.board[i] = 2;
                new_state.turn = 1;
                int eval = maksMinAlfaBeta(new_state, depth - 1, alpha, beta, true);
                min_eval = min(min_eval, eval);
                beta = min(beta, eval);

                if (beta <= alpha) break;
            }
        }
        return min_eval;
    }
}
