#include "Server.h"
#include <QApplication>
#include <ctime>
#include <cstdlib>

int main(int argc, char *argv[])
{
    srand(time(NULL));

    QApplication a(argc, argv);
    Server w;
    w.show();

    return a.exec();
}
