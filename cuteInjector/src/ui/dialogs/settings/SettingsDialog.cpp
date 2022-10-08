#include "SettingsDialog.h"
#include "HotKeySettingsTab.h"
#include "src/ui/WindowsTheme.h"
#include "src/definitions/Themes.h"

#include "qtabwidget.h"
#include "qboxlayout.h"


SettingsDialog::SettingsDialog()
{
    if (WindowsTheme::getWindowsSystemTheme() == SYSTEMTHEMES.DARK)
        WindowsTheme::setDarkTitleBarWindows(reinterpret_cast<HWND>(winId()));

    setMinimumSize(420, 300);
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
