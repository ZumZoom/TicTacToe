#include <QtGui>

#include "TicTacToe.h"

void TicTacToe::clearBoard()
{
    for(size_t i = 0; i < DESK_SIZE; ++i)
        for(size_t j = 0; j < DESK_SIZE; ++j)
            desk[i][j] = field();
    player = 0;
    nextMove = std::make_pair(-1, -1);
    update();
}

void TicTacToe::updateField(int f_i, int f_j, QMouseEvent *event) {
    for(size_t i = 0; i < FIELD_SIZE; ++i)
        for(size_t j = 0; j < FIELD_SIZE; ++j) {
            QRect cell = cellRect(f_i, f_j, i, j);
            if (cell.contains(event->pos())) {
                if (desk[f_i][f_j].data[i][j] == field::Empty) {
                    desk[f_i][f_j].freeCells--;
                    if (player % 2 == 0)
                        desk[f_i][f_j].data[i][j] = field::Cross;
                    else
                        desk[f_i][f_j].data[i][j] = field::Nought;
                    if(desk[nextMove.first][nextMove.second].winner == field::Empty)
                        checkWinner(nextMove.first, nextMove.second);
                    player ^= 1;
                    if(desk[i][j].freeCells == 0) {
                        nextMove.first = -1;
                        nextMove.second = -1;
                    }
                    else {
                        nextMove.first = i;
                        nextMove.second = j;
                    }
                    update();
                    return;
                }
            }
        }
}

void TicTacToe::mousePressEvent(QMouseEvent *event)
{
    if(nextMove.first == -1 && nextMove.second == -1) {
        for(size_t f_i = 0; f_i < DESK_SIZE; ++f_i)
            for(size_t f_j = 0; f_j < DESK_SIZE; ++f_j) {
                QRect field = fieldRect(f_i, f_j);
                if(field.contains(event->pos())) {
                    updateField(f_i, f_j, event);
                    return;
                }
            }
    }
    else {
        updateField(nextMove.first, nextMove.second, event);
    }
}

void TicTacToe::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(Qt::transparent));

    for(size_t i = 0; i < DESK_SIZE; ++i)
        for(size_t j = 0; j < DESK_SIZE; ++j)
            switch ( desk[i][j].winner )
            {
            case field::Cross :
            {
                painter.setBrush(Qt::red);
                painter.drawRect(fieldRect(i, j));
                break;
            }
            case field::Nought :
            {
                painter.setBrush(Qt::blue);
                painter.drawRect(fieldRect(i, j));
                break;
            }
            case field::Draw :
            {
                painter.setBrush(Qt::darkGray);
                painter.drawRect(fieldRect(i, j));
                break;
            }
            default :
            {
                break;
            }
            }

    painter.setBrush(Qt::transparent);

    painter.setPen(QPen(Qt::black, 4));
    painter.drawLine(fieldWidth(), 0, fieldWidth(), height());
    painter.drawLine(2 * fieldWidth(), 0, 2 * fieldWidth(), height());
    painter.drawLine(0, fieldHeight(), width(), fieldHeight());
    painter.drawLine(0, 2 * fieldHeight(), width(), 2 * fieldHeight());


    painter.setPen(QPen(Qt::black, 1));

    for(size_t i = 0; i < DESK_SIZE; ++i)
        for(size_t j = 0; j < DESK_SIZE; ++j) {
            painter.drawLine(i * fieldWidth() + cellWidth(),
                             j * fieldHeight() + fieldHeight() * 0.05,
                             i * fieldWidth() + cellWidth(),
                             j * fieldHeight() + fieldHeight() * 0.95);
            painter.drawLine(i * fieldWidth() + 2 * cellWidth(),
                             j * fieldHeight() + fieldHeight() * 0.05,
                             i * fieldWidth() + 2 * cellWidth(),
                             j * fieldHeight() + fieldHeight() * 0.95);
            painter.drawLine(i * fieldWidth() + fieldWidth() * 0.05,
                             j * fieldHeight() + cellHeight(),
                             i * fieldWidth() + fieldWidth() * 0.95,
                             j * fieldHeight() + cellHeight());
            painter.drawLine(i * fieldWidth() + fieldWidth() * 0.05,
                             j * fieldHeight() + 2 * cellHeight(),
                             i * fieldWidth() + fieldWidth() * 0.95,
                             j * fieldHeight() + 2 * cellHeight());
        }

    painter.setPen(QPen(Qt::black, 2));

    for(size_t f_i = 0; f_i < DESK_SIZE; ++f_i)
        for(size_t f_j = 0; f_j < DESK_SIZE; ++f_j)
            for(size_t i = 0; i < FIELD_SIZE; ++i)
                for(size_t j = 0; j < FIELD_SIZE; ++j) {
                    QRect cell = cellRect(f_i, f_j, i, j);
                    if (desk[f_i][f_j].data[i][j] == field::Cross) {
                        painter.drawLine(cell.topLeft(), cell.bottomRight());
                        painter.drawLine(cell.topRight(), cell.bottomLeft());
                    } else if (desk[f_i][f_j].data[i][j] == field::Nought) {
                        painter.drawEllipse(cell);
                    }
                }

    if(nextMove.first != -1 && nextMove.second != -1) {
        painter.setPen(QPen(Qt::yellow, 4));
        painter.drawRect(fieldRect(nextMove.first, nextMove.second));
    }
}

QRect TicTacToe::cellRect(int fieldRow, int fieldCol, int row, int col) const
{
    const int HMargin = width() / (DESK_SIZE * FIELD_SIZE * 10);
    const int VMargin = height() / (DESK_SIZE * FIELD_SIZE * 10);
    return QRect(fieldCol * fieldWidth() + col * cellWidth() + HMargin,
                 fieldRow * fieldHeight() + row * cellHeight() + VMargin,
                 cellWidth() - 2 * HMargin,
                 cellHeight() - 2 * VMargin);
}

QRect TicTacToe::fieldRect(int row, int column) const
{
    return QRect(column * fieldWidth(),
                 row * fieldHeight(),
                 fieldWidth(),
                 fieldHeight());
}

void TicTacToe::checkWinner(int f_i, int f_j) {
    for(size_t ci = 0; ci < FIELD_SIZE; ++ci) {
        if(desk[f_i][f_j].data[ci][0] == desk[f_i][f_j].data[ci][1] &&
           desk[f_i][f_j].data[ci][0] == desk[f_i][f_j].data[ci][2] &&
           desk[f_i][f_j].data[ci][0] != field::Empty)
        {
            desk[f_i][f_j].winner = desk[f_i][f_j].data[ci][0];
            checkGlobalWinner();
            return;
        }
        if(desk[f_i][f_j].data[0][ci] == desk[f_i][f_j].data[1][ci] &&
           desk[f_i][f_j].data[0][ci] == desk[f_i][f_j].data[2][ci] &&
           desk[f_i][f_j].data[0][ci] != field::Empty)
        {
            desk[f_i][f_j].winner = desk[f_i][f_j].data[0][ci];
            checkGlobalWinner();
            return;
        }
    }
    if(desk[f_i][f_j].data[0][0] == desk[f_i][f_j].data[1][1] &&
       desk[f_i][f_j].data[0][0] == desk[f_i][f_j].data[2][2] &&
       desk[f_i][f_j].data[0][0] != field::Empty)
    {
        desk[f_i][f_j].winner = desk[f_i][f_j].data[0][0];
        checkGlobalWinner();
        return;
    }
    if(desk[f_i][f_j].data[0][2] == desk[f_i][f_j].data[1][1] &&
       desk[f_i][f_j].data[0][2] == desk[f_i][f_j].data[2][0] &&
       desk[f_i][f_j].data[0][2] != field::Empty)
    {
        desk[f_i][f_j].winner = desk[f_i][f_j].data[0][2];
        checkGlobalWinner();
        return;
    }

    if(desk[f_i][f_j].freeCells == 0) {
        desk[f_i][f_j].winner = field::Draw;
        checkGlobalWinner();
        return;
    }
}

namespace
{
const QString WinMessages[] = { "", "Cross wins the match!", "Nought wins the match!", "Draw!" };
}

void TicTacToe::checkGlobalWinner() {
    update();
    for(size_t ci = 0; ci < DESK_SIZE; ++ci) {
        if(desk[ci][0].winner == desk[ci][1].winner &&
           desk[ci][0].winner == desk[ci][2].winner &&
           ( desk[ci][0].winner == field::Cross ||
             desk[ci][0].winner == field::Nought ))
        {
            showMessage(WinMessages[desk[ci][0].winner]);
            clearBoard();
            return;
        }
        if(desk[0][ci].winner == desk[1][ci].winner &&
           desk[0][ci].winner == desk[2][ci].winner &&
           ( desk[0][ci].winner == field::Cross ||
             desk[0][ci].winner == field::Nought ))
        {
            showMessage(WinMessages[desk[0][ci].winner]);
            clearBoard();
            return;
        }
    }
    if(desk[0][0].winner == desk[1][1].winner &&
       desk[0][0].winner == desk[2][2].winner &&
       ( desk[0][0].winner == field::Cross ||
         desk[0][0].winner == field::Nought ))
    {
        showMessage(WinMessages[desk[0][0].winner]);
        clearBoard();
        return;
    }
    if(desk[0][2].winner == desk[1][1].winner &&
       desk[0][2].winner == desk[2][0].winner &&
       ( desk[0][2].winner == field::Cross ||
         desk[0][2].winner == field::Nought ))
    {
        showMessage(WinMessages[desk[0][2].winner]);
        clearBoard();
        return;
    }

    for(size_t i = 0; i < DESK_SIZE; ++i)
        for(size_t j = 0; j < DESK_SIZE; ++j)
            if(desk[i][j].freeCells > 0)
                return;

    showMessage(WinMessages[3]);
    clearBoard();
}

void TicTacToe::showMessage(const QString &str) {
    QMessageBox msgbox;
    msgbox.setText(str);
    msgbox.exec();
}
