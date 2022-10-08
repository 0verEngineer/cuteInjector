#include "ui/MainWindow.h"
#include "ui/Style.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationName("OverEngineer");
    QApplication::setApplicationName("CuteInjector");
    QApplication::setApplicationVersion("0.1.0");

    ApplyStyle(&a);

    MainWindow w;
    w.show();
    return a.exec();
}
