#include "menuBar.h"


MenuBar::MenuBar(MainWindow* parent)
    : QMenuBar{parent}
{
    file = new QMenu("File", this);
    addMenu(file);

    edit = new QMenu("Edit", this);
    addMenu(edit);

    view = new QMenu("View", this);
    //addMenu(view);

    file->addAction(parent->actions.injectAct);
    file->addAction(parent->actions.addFileAct);
    file->addAction(parent->actions.removeFileAct);
    file->addAction(parent->actions.selectProcessAct);

    edit->addAction(parent->actions.openSettingsAct);
}
