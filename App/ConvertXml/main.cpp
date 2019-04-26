#include "convertxml.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConvertXml w;
    w.show();

    return a.exec();
}
