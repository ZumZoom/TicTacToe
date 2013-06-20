#include <QtGui>

#include "TicTacToe.h"

void TicTacToe::clearBoard()
{
    for(size_t i = 0; i < 3; ++i)
        for(size_t j = 0; j < 3; ++j)
            desk[i][j] = field();
    update();
}

void TicTacToe::mousePressEvent(QMouseEvent *event)
{
    if(nextMove.first == -1 && nextMove.second == -1) {
        for(int f_i = 0; f_i < 3; ++f_i)
            for(int f_j = 0; f_j < 3; ++f_j)
                for(int i = 0; i < 3; ++i)
                    for(int j = 0; j < 3; ++j) {
                        QRect cell = cellRect(f_i, f_j, i, j);
                        if (cell.contains(event->pos())) {
                            if (desk[f_i][f_j].data[i][j] == field::Empty) {
                                desk[f_i][f_j].freeCells--;
                                if (player % 2 == 0)
                                    desk[f_i][f_j].data[i][j] = field::Cross;
                                else
                                    desk[f_i][f_j].data[i][j] = field::Nought;
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
                                break;
                            }
                        }
                    }
    }
    else {
        for(int i = 0; i < 3; ++i)
            for(int j = 0; j < 3; ++j) {
                QRect cell = cellRect(nextMove.first, nextMove.second, i, j);
                if (cell.contains(event->pos())) {
                    if (desk[nextMove.first][nextMove.second].data[i][j] == field::Empty) {
                        desk[nextMove.first][nextMove.second].freeCells--;
                        if (player % 2 == 0)
                            desk[nextMove.first][nextMove.second].data[i][j] = field::Cross;
                        else
                            desk[nextMove.first][nextMove.second].data[i][j] = field::Nought;
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
                        break;
                    }
                }
            }
    }
}

void TicTacToe::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(Qt::transparent));

    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j)
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
                painter.setBrush(Qt::darkMagenta);
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

    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j) {
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

    for(int f_i = 0; f_i < 3; ++f_i)
        for(int f_j = 0; f_j < 3; ++f_j)
            for(int i = 0; i < 3; ++i)
                for(int j = 0; j < 3; ++j) {
                    QRect cell = cellRect(f_i, f_j, i, j);
                    if (desk[f_i][f_j].data[i][j] == field::Cross) {
                        painter.drawLine(cell.topLeft(), cell.bottomRight());
                        painter.drawLine(cell.topRight(), cell.bottomLeft());
                    } else if (desk[f_i][f_j].data[i][j] == field::Nought) {
                        painter.drawEllipse(cell);
                    }
                }
}

QRect TicTacToe::cellRect(int fieldRow, int fieldCol, int row, int col) const
{
    const int HMargin = width() / 90;
    const int VMargin = height() / 90;
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
    for(int ci = 0; ci < 3; ++ci) {
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
const QString WinMessages[] = { "2222222222222222", "Cross wins the match!", "Nought wins the match!", "Draw!" };
}

void TicTacToe::checkGlobalWinner() {
    for(int ci = 0; ci < 3; ++ci) {
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

    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j)
            if(desk[i][j].freeCells > 0)
                return;

    showMessage(WinMessages[3]);
    clearBoard();
}

void TicTacToe::showMessage(const QString &str) {
    QMessageBox msgbox;
    msgbox.setText(str);
//    msgbox.setStyleSheet("* { background-color:rgb(199,147,88); padding: 7px ; color:rgb(255,255,255)}");
    msgbox.exec();
}
