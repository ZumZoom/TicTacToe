#include "TicTacToe.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TicTacToe w;
    w.show();
    return a.exec();
}
