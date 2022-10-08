
#ifndef CUTEINJECTOR_SETTINGSDIALOG_H
#define CUTEINJECTOR_SETTINGSDIALOG_H


#include "qdialog.h"


class QBoxLayout;
class QTabWidget;
class HotKeySettingsTab;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SettingsDialog();
    void execDialog();

signals:
    void hotKeyChanged();

private:
    QBoxLayout* layout = nullptr;
    QTabWidget* tabWidget = nullptr;
};


#endif //CUTEINJECTOR_SETTINGSDIALOG_H
