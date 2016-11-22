#include <iostream>
#include <QCoreApplication>
#include "server.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Server sr(2233);
    app.exec();
}
