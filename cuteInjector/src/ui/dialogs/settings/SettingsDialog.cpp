
#include "SettingsDialog.h"
#include "HotKeySettingsTab.h"

#include "qtabwidget.h"
#include "qboxlayout.h"


SettingsDialog::SettingsDialog()
{
    setMinimumSize(600, 420);
    layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom, this);
    setWindowTitle("Settings");
    tabWidget = new QTabWidget(this);
    layout->addWidget(this->tabWidget);
    setLayout(layout);
}

void SettingsDialog::execDialog()
{
    tabWidget->clear();
    HotKeySettingsTab hotKeySettingsTab = HotKeySettingsTab(this);
    tabWidget->addTab(&hotKeySettingsTab, "HotKeys");

    connect(&hotKeySettingsTab, &HotKeySettingsTab::hotKeyChanged, this, [=] {
       emit hotKeyChanged();
    });

    exec();
}
