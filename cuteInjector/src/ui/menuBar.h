#ifndef MENUBAR_H
#define MENUBAR_H

#include "qmenu.h"
#include "qmenubar.h"
#include "qwidget.h"
#include "mainWindow.h"


class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit MenuBar(MainWindow* parent = nullptr);

private:
    QMenu* file = nullptr;
    QMenu* edit = nullptr;
    QMenu* view = nullptr;
};

#endif // MENUBAR_H
