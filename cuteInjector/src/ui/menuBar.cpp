#include "menuBar.h"


MenuBar::MenuBar(MainWindow* parent)
    : QMenuBar{parent}
{
    file = new QMenu("File", this);
    addMenu(file);

    view = new QMenu("Edit", this);
    //addMenu(view);

    edit = new QMenu("View", this);
    //addMenu(edit);

    file->addAction(parent->actions.injectAct);
    file->addAction(parent->actions.addFileAct);
    file->addAction(parent->actions.removeFileAct);
    file->addAction(parent->actions.selectProcessAct);
}
