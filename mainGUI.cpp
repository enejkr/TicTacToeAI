#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QWidget>
#include <QMessageBox>
#include <vector>
#include <climits>

#include "ticTacToe.h"

void updateButtons(const BoardState &state, std::vector<QPushButton*> &buttons) {
    for (int i = 0; i < 9; ++i) {
        if (state.board[i] == 1)
            buttons[i]->setText("X");
        else if (state.board[i] == 2)
            buttons[i]->setText("O");
        else
            buttons[i]->setText("");
    }
}

bool isBoardFull(const BoardState &state) {
    for (int i = 0; i < 9; ++i)
        if (state.board[i] == 0) return false;
    return true;
}

void disableAllButtons(std::vector<QPushButton*> &buttons) {
    for (auto *btn : buttons)
        btn->setEnabled(false);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Tic Tac Toe AI");
    window.setFixedSize(780, 900);
    vector<int> resoult_counter(3, 0); // 0 = draw, 1 = X wins, 2 = O wins
    BoardState *gameState = new BoardState();
    bool *gameOver = new bool(false);
    //Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(&window);

    //Status label
    QLabel *statusLabel = new QLabel("Set depth and press Play!", &window);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("font-size: 50px; font-weight: bold; margin: 10px;");
    // settings layout dropdown for depth selection in sepret layout for future expansion
    QVBoxLayout *settingsLayout = new QVBoxLayout();
    QHBoxLayout *depthLayout = new QHBoxLayout();
    QSpinBox *depthSpinBox = new QSpinBox(&window);
    depthSpinBox->setRange(1, 9);
    depthSpinBox->setValue(1);
    depthSpinBox->setFixedSize(75, 50);
    QLabel *depthLabel = new QLabel("Select Depth: ", &window);
    depthLabel->setStyleSheet("font-size: 16px;");
    depthLayout->addWidget(depthLabel);
    depthLayout->addWidget(depthSpinBox);
    settingsLayout->addLayout(depthLayout);
    settingsLayout->setContentsMargins(0, 0, 0, 0);
    settingsLayout->setSpacing(0);
    settingsLayout->setAlignment(Qt::AlignCenter);

    //Leaderboard
    QVBoxLayout *leaderboardLayout = new QVBoxLayout();
    leaderboardLayout->setSpacing(4);
    leaderboardLayout->setAlignment(Qt::AlignCenter);

    QLabel *leaderboardTitle = new QLabel("Leaderboard", &window);
    leaderboardTitle->setAlignment(Qt::AlignCenter);
    leaderboardTitle->setStyleSheet("font-size: 20px; font-weight: bold; margin-top: 8px;");

    QLabel *xWinsLabel    = new QLabel("X (You)  wins: 0", &window);
    QLabel *oWinsLabel    = new QLabel("O (AI)   wins: 0", &window);
    QLabel *drawsLabel    = new QLabel("Draws:        0", &window);

    QString leaderboardStyle = "font-size: 17px; font-family: monospace; padding: 2px 12px;";
    xWinsLabel->setAlignment(Qt::AlignCenter);
    oWinsLabel->setAlignment(Qt::AlignCenter);
    drawsLabel->setAlignment(Qt::AlignCenter);
    xWinsLabel->setStyleSheet(leaderboardStyle + " color: green;");
    oWinsLabel->setStyleSheet(leaderboardStyle + " color: red;");
    drawsLabel->setStyleSheet(leaderboardStyle);

    leaderboardLayout->addWidget(leaderboardTitle);
    leaderboardLayout->addWidget(xWinsLabel);
    leaderboardLayout->addWidget(oWinsLabel);
    leaderboardLayout->addWidget(drawsLabel);

    // Helper lambda to refresh leaderboard
    auto updateLeaderboard = [&resoult_counter, xWinsLabel, oWinsLabel, drawsLabel]() mutable {
        xWinsLabel->setText(QString("X (You)  wins: %1").arg(static_cast<int>(resoult_counter[1])));
        oWinsLabel->setText(QString("O (AI)   wins: %1").arg(static_cast<int>(resoult_counter[2])));
        drawsLabel->setText(QString("Draws:        %1").arg(static_cast<int>(resoult_counter[0])));
    };

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(5);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    std::vector<QPushButton*> buttons(9);

    for (int i = 0; i < 9; ++i) {
        buttons[i] = new QPushButton("", &window);
        buttons[i]->setFixedSize(100, 100);
        buttons[i]->setStyleSheet("font-size: 36px; font-weight: bold;");
        buttons[i]->setEnabled(false);
        gridLayout->addWidget(buttons[i], i / 3, i % 3);
    }

    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->addStretch();
    centerLayout->addLayout(gridLayout);
    centerLayout->addStretch();

    QPushButton *playButton = new QPushButton("Play", &window);
    playButton->setStyleSheet("font-size: 18px; padding: 10px;");

    QPushButton *newDepthButton = new QPushButton("Set New Depth", &window);
    newDepthButton->setStyleSheet("font-size: 18px; padding: 10px;");
    newDepthButton->setEnabled(false);

    for (int i = 0; i < 9; ++i) {
        QObject::connect(buttons[i], &QPushButton::clicked,
            [i, gameState, gameOver, &buttons, &resoult_counter, depthSpinBox, statusLabel, playButton, updateLeaderboard]() mutable {
                if (*gameOver) return;
                if (gameState->turn != 1) return;
                if (gameState->board[i] != 0) return;

                makeMove(*gameState, i);
                updateButtons(*gameState, buttons);

                // Check if player won
                if (checkWin(*gameState) != 0) {
                    statusLabel->setText("X wins!");
                    resoult_counter[1]++;
                    updateLeaderboard();
                    statusLabel->setStyleSheet("font-size: 50px; font-weight: bold; margin: 10px; color: green;");
                    *gameOver = true;
                    disableAllButtons(buttons);
                    playButton->setEnabled(true);
                    return;
                }
                // Check draw
                if (isBoardFull(*gameState)) {
                    resoult_counter[0]++;
                    updateLeaderboard();
                    statusLabel->setText("It's a draw!");
                    statusLabel->setStyleSheet("font-size: 50px; font-weight: bold; margin: 10px;");
                    *gameOver = true;
                    disableAllButtons(buttons);
                    playButton->setEnabled(true);
                    return;
                }

                // AI turn (O = 2, minimizing player)
                statusLabel->setText("AI is thinking...");
                int depth = depthSpinBox->value();
                int best_score = INT_MAX;
                bool is_best_empty = true;
                int best_move = -1;
                for (int j = 0; j < 9; ++j) {
                    if (gameState->board[j] == 0) {
                        BoardState new_state = *gameState;
                        new_state.board[j] = 2;
                        new_state.turn = 1;
                        int score = maksMinAlfaBeta(new_state, depth - 1, INT_MIN, INT_MAX, true);
                        if (score < best_score || is_best_empty) {
                            best_score = score;
                            best_move = j;
                            is_best_empty = false;
                        }
                    }
                }
                if (best_move != -1) {
                    makeMove(*gameState, best_move);
                    updateButtons(*gameState, buttons);
                }

                // Check if AI won
                if (checkWin(*gameState) != 0) {
                    statusLabel->setText("O wins!");
                    resoult_counter[2]++;
                    updateLeaderboard();
                    statusLabel->setStyleSheet("font-size: 50px; font-weight: bold; margin: 10px; color: red;");
                    *gameOver = true;
                    disableAllButtons(buttons);
                    playButton->setEnabled(true);
                    return;
                }
                // Check draw after AI move
                if (isBoardFull(*gameState)) {
                    statusLabel->setText("It's a draw!");
                    resoult_counter[0]++;
                    updateLeaderboard();
                    statusLabel->setStyleSheet("font-size: 50px; font-weight: bold; margin: 10px;");
                    *gameOver = true;
                    disableAllButtons(buttons);
                    playButton->setEnabled(true);
                    return;
                }

                statusLabel->setText("Your turn (X)");
                statusLabel->setStyleSheet("font-size: 50px; font-weight: bold; margin: 10px;");
            });
    }

    // Play / Reset button
    QObject::connect(playButton, &QPushButton::clicked,
        [gameState, gameOver, &buttons, depthSpinBox, depthLabel, playButton, statusLabel, newDepthButton]() {
            for (int i = 0; i < 9; ++i)
                gameState->board[i] = 0;
            gameState->turn = 1;
            *gameOver = false;

            updateButtons(*gameState, buttons);
            for (auto *btn : buttons)
                btn->setEnabled(true);

            depthSpinBox->setEnabled(false);
            depthLabel->setEnabled(false);
            playButton->setText("Restart");
            newDepthButton->setEnabled(true);
            statusLabel->setText("Your turn (X)");
            statusLabel->setStyleSheet("font-size: 50px; font-weight: bold; margin: 10px;");
        });

    // Set New Depth button - resets everything back to initial state (also resets leaderboard)
    QObject::connect(newDepthButton, &QPushButton::clicked,
        [gameState, gameOver, &buttons, &resoult_counter, depthSpinBox, depthLabel, playButton, newDepthButton, statusLabel,
         updateLeaderboard]() mutable {
            for (int i = 0; i < 9; ++i)
                gameState->board[i] = 0;
            gameState->turn = 1;
            *gameOver = false;

            // Reset leaderboard counters
            resoult_counter[0] = resoult_counter[1] = resoult_counter[2] = 0;
            updateLeaderboard();

            updateButtons(*gameState, buttons);
            disableAllButtons(buttons);

            depthSpinBox->setEnabled(true);
            depthLabel->setEnabled(true);
            playButton->setText("Play");
            playButton->setEnabled(true);
            newDepthButton->setEnabled(false);
            statusLabel->setText("Set depth and press Play!");
            statusLabel->setStyleSheet("font-size: 50px; font-weight: bold; margin: 10px;");
        });

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(playButton);
    buttonLayout->addWidget(newDepthButton);

    //final composition of layouts
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(settingsLayout);
    mainLayout->addLayout(leaderboardLayout);
    mainLayout->addLayout(centerLayout);
    mainLayout->addLayout(buttonLayout);

    window.show();
    return app.exec();
}

