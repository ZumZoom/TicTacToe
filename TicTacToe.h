#ifndef TICTACTOE_H
#define TICTACTOE_H

#include "ui_TicTacToe.h"

#include <QtGui/QWidget>

struct field {
    enum Owner
    {
        Empty   = 0,
        Cross   = 1,
        Nought  = 2,
        Draw    = 3
    };
    Owner data[3][3];
    Owner winner;
    int freeCells;

    field() : winner(Empty), freeCells(9)
    {
        memset(data, 0, sizeof(data));
    }
};

class TicTacToe : public QWidget
{
//    Q_OBJECT

public:
    TicTacToe(QWidget *parent = 0)
        : QWidget(parent)
        , nextMove(-1, -1)
        , player(0)

    {
        setFixedSize(800, 800);
    }
    void clearBoard();

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    QRect cellRect(int fieldRow, int fieldCol, int row, int col) const;
    QRect fieldRect(int row, int col) const;
    int cellWidth() const { return width() / 9; }
    int cellHeight() const { return height() / 9; }
    int fieldWidth() const { return width() / 3; }
    int fieldHeight() const { return height() / 3; }
    void checkWinner(int f_i, int f_j);
    void checkGlobalWinner();
    void showMessage(const QString &str);

    field desk[3][3];
    std::pair<int, int> nextMove;
    int player;

    Ui::tictactoeClass ui;
};

#endif // TICTACTOE_H
