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

    static const int DESK_SIZE = 3;
    static const int FIELD_SIZE = 3;

public:
    TicTacToe(QWidget *parent = 0)
        : QWidget(parent)
        , nextMove(-1, -1)
        , player(0)

    {
        setFixedSize(600, 600);
    }
    void clearBoard();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);

private:
    QRect cellRect(int fieldRow, int fieldCol, int row, int col) const;
    QRect fieldRect(int row, int col) const;
    int cellWidth() const { return width() / (DESK_SIZE * FIELD_SIZE); }
    int cellHeight() const { return height() / (DESK_SIZE * FIELD_SIZE); }
    int fieldWidth() const { return width() / DESK_SIZE; }
    int fieldHeight() const { return height() / DESK_SIZE; }
    void updateField(int f_i, int f_j, QMouseEvent *event);
    void checkWinner(int f_i, int f_j);
    void checkGlobalWinner();
    void showMessage(const QString &str);

    field desk[DESK_SIZE][DESK_SIZE];
    std::pair<int, int> nextMove;
    int player;

    Ui::tictactoeClass ui;
};

#endif // TICTACTOE_H
