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
        for (int el : board.board) if (el == 0) { anyEmpty = true; break; }
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
    //
    //
    //
    if (state.board[x * 3 + y] == 0) {
        state.board[x * 3 + y] = state.turn;
        state.turn = (state.turn == 1) ? 2 : 1; // Switch turns
    } else {
        cout << "Cell already occupied. Try again." << '\n';
    }
}

int countFavorableLines(const BoardState &state) {
    int count = 0;

    // check row and column lines for the current player
    for (int i = 0; i < 3; i++) {
        // Check rows
        if ((state.board[i * 3 + 0] == state.turn && state.board[i * 3 + 1] == state.turn && state.board[i * 3 + 2] == 0) ||
            (state.board[i * 3 + 0] == state.turn && state.board[i * 3 + 1] == 0 && state.board[i * 3 + 2] == state.turn) ||
            (state.board[i * 3 + 0] == 0 && state.board[i * 3 + 1] == state.turn && state.board[i * 3 + 2] == state.turn)) {
            count++;
        }
        // Check columns
        if ((state.board[0 * 3 + i] == state.turn && state.board[1 * 3 + i] == state.turn && state.board[2 * 3 + i] == 0) ||
            (state.board[0 * 3 + i] == state.turn && state.board[1 * 3 + i] == 0 && state.board[2 * 3 + i] == state.turn) ||
            (state.board[0 * 3 + i] == 0 && state.board[1 * 3 + i] == state.turn && state.board[2 * 3 + i] == state.turn)) {
            count++;
        }
    }
    // Check diagonals
    if (state.board[0] == state.turn && state.board[4] == state.turn && state.board[8] == 0 ||
        state.board[0] == state.turn && state.board[4] == 0 && state.board[8] == state.turn ||
        state.board[0] == 0 && state.board[4] == state.turn && state.board[8] == state.turn) {
        count++;
    }
    if (state.board[2] == state.turn && state.board[4] == state.turn && state.board[6] == 0 ||
        state.board[2] == state.turn && state.board[4] == 0 && state.board[6] == state.turn ||
        state.board[2] == 0 && state.board[4] == state.turn && state.board[6] == state.turn) {
        count++;
    }
    return count;
}
int basicEvaluation(const BoardState &state) {
    if (checkWin(state)) {
        // state.turn has already been flipped after the last move
        // state.turn == 1 means O just moved and won -> -10
        // state.turn == 2 means X just moved and won -> +10
        return (state.turn == 1) ? -11 : 11;
    }
    int count =  countFavorableLines(state);
    cout << drawBoard(state) << "Favorable lines for player " << state.turn << ": " << count << '\n';
    return (state.turn == 1) ? -count : count;


}

int maksMinAlfaBeta(BoardState &state, int depth, int alpha, int beta, bool maximizingPlayer) {
    if (checkWin(state) || depth == 0) {
        return basicEvaluation(state);
    }
    // Check for draw (no empty cells)
    bool hasEmpty = false;
    for (int i = 0; i < 9; ++i) {
        if (state.board[i] == 0) { hasEmpty = true; break; }
    }
    if (!hasEmpty || depth == 0) {
        return 0; // draw or depth limit with no win
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

