# Qt6 GUI – Cheat Sheet za TicTacToe projekt

---

## 1. OSNOVNA STRUKTURA VSAKE QT APLIKACIJE

```cpp
#include <QApplication>   // obvezno – upravlja z event loop

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);  // inicializira Qt

    // ... ustvari okna, widgete ...

    return app.exec();  // zažene event loop (čaka na klike, tipke, ...)
}
```

**Brez `app.exec()` se okno takoj zapre.**

---

## 2. OKNA (QWidget / QMainWindow)

| Razred         | Kdaj uporabiti                                      |
|----------------|-----------------------------------------------------|
| `QWidget`      | Preprosto okno / osnova za vse widgete              |
| `QMainWindow`  | Okno z menu barom, toolbar-om, status barom         |
| `QDialog`      | Pojavno okno (nastavitve, sporočila, vnosna forma)  |

```cpp
QWidget *window = new QWidget();
window->setWindowTitle("Tic Tac Toe");
window->setFixedSize(400, 400);   // fiksna velikost
window->show();
```

---

## 3. LAYOUTS – RAZPOREJANJE WIDGETOV

Layout avtomatično razporedi widgete v oknu. **Ne postavljaj widgetov ročno z x,y koordinatami.**

| Layout           | Opis                                         |
|------------------|----------------------------------------------|
| `QVBoxLayout`    | Widgeti eden pod drugim (vertikalno)         |
| `QHBoxLayout`    | Widgeti eden poleg drugega (horizontalno)    |
| `QGridLayout`    | Mreža (vrstice × stolpci) – idealno za 3×3  |
| `QStackedLayout` | Samo en widget viden naenkrat (strani)       |

```cpp
QVBoxLayout *mainLayout = new QVBoxLayout(window);  // postavi layout na window

QHBoxLayout *row = new QHBoxLayout();   // vrstica znotraj

mainLayout->addLayout(row);             // dodaj vrstico v glavni layout
mainLayout->addWidget(someButton);      // dodaj widget direktno
mainLayout->addStretch();               // prazen raztegljiv prostor
mainLayout->setSpacing(10);             // razmak med elementi
mainLayout->setContentsMargins(10,10,10,10); // robovi (L,T,R,B)
```

### QGridLayout – za 3×3 mrežo

```cpp
QGridLayout *grid = new QGridLayout(window);
grid->addWidget(button, vrstica, stolpec);         // postavi widget
grid->addWidget(button, vrstica, stolpec, 1, 2);   // razpni čez 1 vrstico, 2 stolpca
grid->setSpacing(5);
```

---

## 4. OSNOVNI WIDGETI (KOMPONENTE)

### QPushButton – gumb
```cpp
QPushButton *btn = new QPushButton("Klikni", parent);
btn->setFixedSize(80, 80);
btn->setText("X");               // nastavi besedilo
btn->setEnabled(false);          // onemogoči klik
btn->setStyleSheet("font-size: 24px; background-color: lightblue;");
```

### QLabel – prikaz besedila / slike
```cpp
QLabel *label = new QLabel("Na vrsti je: X", parent);
label->setAlignment(Qt::AlignCenter);
label->setText("Nova vrednost");   // posodobi besedilo
label->setStyleSheet("font-size: 16px; color: red;");
```

### QLineEdit – eno-vrstično vnosno polje
```cpp
QLineEdit *input = new QLineEdit(parent);
input->setPlaceholderText("Vpiši ime...");
input->setText("Privzeto");
QString text = input->text();    // preberi vsebino
input->setMaxLength(20);
input->setValidator(new QIntValidator(1, 9, parent));  // samo cela števila 1-9
```

### QComboBox – spustni seznam (dropdown)
```cpp
QComboBox *combo = new QComboBox(parent);
combo->addItem("Globina 1");
combo->addItem("Globina 2");
combo->addItems({"Lahka", "Srednja", "Težka"});  // dodaj več naenkrat

int idx    = combo->currentIndex();   // trenutno izbran indeks (0-based)
QString val = combo->currentText();   // trenutno izbrano besedilo
combo->setCurrentIndex(2);            // nastavi izbor programsko
```

### QSpinBox – številčni vnosnik z +/- gumboma
```cpp
QSpinBox *spin = new QSpinBox(parent);
spin->setRange(1, 9);
spin->setValue(5);
int val = spin->value();   // preberi vrednost
```

### QTextEdit – večvrstično besedilno polje (za log / prikaz)
```cpp
QTextEdit *log = new QTextEdit(parent);
log->setReadOnly(true);                  // samo za prikaz
log->append("AI je odigral potezo!");    // dodaj vrstico
log->clear();                            // počisti
```

### QMessageBox – pojavno sporočilno okno
```cpp
// Informativno sporočilo
QMessageBox::information(parent, "Naslov", "X je zmagal!");

// Vprašanje z gumboma Da/Ne
int odgovor = QMessageBox::question(parent, "Nova igra?", "Želiš začeti znova?",
                                     QMessageBox::Yes | QMessageBox::No);
if (odgovor == QMessageBox::Yes) { /* ... */ }

// Opozorilo
QMessageBox::warning(parent, "Napaka", "To polje je zasedeno!");
```

---

## 5. SIGNALS & SLOTS – KAKO WIDGETI KOMUNICIRAJO

Osnova Qt-ja. **Signal** sproži widget ob dogodku, **slot** je funkcija ki se izvede.

```cpp
// Sintaksa:
QObject::connect(vir, &VirRazred::signal, prejemnik, &PrejemnikRazred::slot);

// Gumb -> klic funkcije
connect(btn, &QPushButton::clicked, this, &MojRazred::onButtonClick);

// Lambda (brez ločene funkcije) – najpogosteje za enostavne stvari
connect(btn, &QPushButton::clicked, [=]() {
    label->setText("Kliknjen!");
});

// ComboBox – ob spremembi izbire
connect(combo, &QComboBox::currentIndexChanged, [=](int index) {
    qDebug() << "Izbran indeks:" << index;
});

// QSpinBox – ob spremembi vrednosti
connect(spin, &QSpinBox::valueChanged, [=](int val) {
    depth = val;
});

// LineEdit – ob pritisku Enter
connect(input, &QLineEdit::returnPressed, [=]() {
    QString text = input->text();
});
```

### Najpogostejši signali

| Widget        | Signal                    | Kdaj se sproži                  |
|---------------|---------------------------|---------------------------------|
| QPushButton   | `clicked()`               | ob kliku                        |
| QPushButton   | `pressed()` / `released()`| ob pritisku / spustu            |
| QComboBox     | `currentIndexChanged(int)`| ob spremembi izbire             |
| QComboBox     | `currentTextChanged(str)` | ob spremembi besedila izbire    |
| QSpinBox      | `valueChanged(int)`       | ob spremembi vrednosti          |
| QLineEdit     | `textChanged(str)`        | ob vsaki spremembi teksta       |
| QLineEdit     | `returnPressed()`         | ob pritisku Enter               |

---

## 6. LASTNI RAZRED Z WIDGETI (pravi Qt način)

Za kompleksnejši GUI narediš lasten razred ki deduje od `QWidget` ali `QMainWindow`.
`Q_OBJECT` makro je obvezen za signals & slots.

```cpp
// gamewindow.h
#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

class GameWindow : public QWidget {
    Q_OBJECT   // OBVEZNO za signals/slots

public:
    explicit GameWindow(QWidget *parent = nullptr);

private slots:
    void onCellClicked(int row, int col);  // slot za klik na celico
    void onNewGame();

private:
    QPushButton *cells[3][3];   // 3x3 gumbi za ploščo
    QLabel      *statusLabel;   // "Na vrsti je: X"
    QComboBox   *depthCombo;    // izbira globine AI
};
```

```cpp
// gamewindow.cpp
#include "gamewindow.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

GameWindow::GameWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Tic Tac Toe AI");
    setFixedSize(350, 420);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Status label
    statusLabel = new QLabel("Na vrsti je: X");
    statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statusLabel);

    // 3x3 mreža gumbov
    QGridLayout *grid = new QGridLayout();
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            cells[r][c] = new QPushButton("");
            cells[r][c]->setFixedSize(90, 90);
            cells[r][c]->setStyleSheet("font-size: 32px;");
            // lambda z zajetimi r in c
            connect(cells[r][c], &QPushButton::clicked, [=]() {
                onCellClicked(r, c);
            });
            grid->addWidget(cells[r][c], r, c);
        }
    }
    mainLayout->addLayout(grid);

    // Spodnja vrstica: dropdown + gumb
    QHBoxLayout *bottom = new QHBoxLayout();
    depthCombo = new QComboBox();
    for (int i = 1; i <= 9; i++)
        depthCombo->addItem("Globina " + QString::number(i));
    bottom->addWidget(depthCombo);

    QPushButton *newGame = new QPushButton("Nova igra");
    connect(newGame, &QPushButton::clicked, this, &GameWindow::onNewGame);
    bottom->addWidget(newGame);

    mainLayout->addLayout(bottom);
}
```

---

## 7. POSODABLJANJE UI IZ LOGIKE (MVC PRISTOP)

Ločuj **logiko igre** (`BoardState`, `makeMove`, `checkWin`) od **prikaza** (`GameWindow`).

```
BoardState  ──→  GameWindow::updateBoard(BoardState&)  ──→  cells[r][c]->setText("X")
```

```cpp
void GameWindow::updateBoard(const BoardState &state) {
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            int val = state.board[r * 3 + c];
            if (val == 0) cells[r][c]->setText("");
            else if (val == 1) cells[r][c]->setText("X");
            else cells[r][c]->setText("O");
        }
    }
}
```

---

## 8. STYLESHEET (CSS ZA QT)

Qt podpira CSS-podoben sistem za stiliziranje.

```cpp
widget->setStyleSheet("property: value;");

// Primeri:
btn->setStyleSheet("background-color: #4CAF50; color: white; border-radius: 8px; font-size: 20px;");
label->setStyleSheet("color: red; font-weight: bold;");
window->setStyleSheet("background-color: #2b2b2b;");

// Stil samo za določen tip znotraj widgeta:
window->setStyleSheet("QPushButton { background: lightgray; } QPushButton:hover { background: gray; }");
```

---

## 9. QSTRING – DELO Z BESEDILOM

Qt ne uporablja `std::string`, ampak `QString`.

```cpp
QString s = "Pozdrav";
s += " svet";                          // concatenacija
s = QString::number(42);               // int → QString
s = "Vrednost: " + QString::number(x); // kombiniranje
int n = s.toInt();                     // QString → int
std::string std_s = s.toStdString();   // QString → std::string
QString q = QString::fromStdString(std_s); // std::string → QString

// Formatiranje (kot printf)
s = QString("Globina: %1, Poteza: %2").arg(depth).arg(move);
```

---

## 10. QTTIMER – ZA AI POTEZO Z ZAMIKOM

Če AI izračun vzame čas ali hočeš animacijo, uporabi `QTimer::singleShot`.

```cpp
#include <QTimer>

// Pokliče lambda čez 500ms (ne blokira UI)
QTimer::singleShot(500, [=]() {
    // izračunaj AI potezo
    // posodobi UI
});
```

---

## 11. KORISTNI DEBUGI

```cpp
#include <QDebug>
qDebug() << "Vrednost:" << x;          // izpis v konzolo (kot cout)
qDebug() << "QString:" << myQString;
qWarning() << "Opozorilo!";
```

---

## 12. HITER PREGLED – KAJ RABIM ZA TICTACTOE GUI

| Potreba                        | Qt komponenta                          |
|--------------------------------|----------------------------------------|
| 3×3 mreža gumbov (polja)       | `QPushButton` v `QGridLayout`          |
| Prikaz "Na vrsti je X"         | `QLabel`                               |
| Izbira globine AI              | `QComboBox` ali `QSpinBox`             |
| Gumb "Nova igra"               | `QPushButton`                          |
| Sporočilo ob zmagi             | `QMessageBox::information()`           |
| Klik na celico → logika igre   | `connect(btn, clicked, lambda)`        |
| Posodobitev prikaze plošče     | `btn->setText("X")` v zanki            |
| Zaklepanje gumbov med AI       | `btn->setEnabled(false/true)`          |
| Log potez                      | `QTextEdit` (readOnly) + `append()`    |
| AI z zamikom (ne zamrzne UI)   | `QTimer::singleShot(ms, lambda)`       |

