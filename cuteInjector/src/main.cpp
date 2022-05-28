#include "ui/mainWindow.h"
#include "ui/style.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ApplyStyle(&a);

    MainWindow w;
    w.show();
    return a.exec();
}
