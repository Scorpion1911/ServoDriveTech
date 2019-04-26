#include "parafileconversion.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ParaFileConversion w;
    w.show();

    return a.exec();
}
