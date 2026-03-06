#include <QApplication>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Qt Test - TicTacToeAI");
    window.setFixedSize(300, 150);

    QVBoxLayout *layout = new QVBoxLayout(&window);

    QLabel *label = new QLabel("Qt6 deluje! ✔", &window);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-size: 18px; color: green;");

    QPushButton *button = new QPushButton("Klikni me!", &window);
    QObject::connect(button, &QPushButton::clicked, [&]() {
        QMessageBox::information(&window, "Sporočilo", "Qt6 je pravilno nastavljen!");
    });

    layout->addWidget(label);
    layout->addWidget(button);

    window.show();
    return app.exec();
}

