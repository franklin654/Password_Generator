#include "homepage.h"
#include "global_variables.h"

#include <QApplication>

Singleton* Singleton::instancePtr = nullptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (!passwordDir->exists())
        QDir::home().mkdir("pmstorage");
    HomePage w;
    w.show();
    return a.exec();
}
