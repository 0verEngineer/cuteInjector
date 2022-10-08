#ifndef CUTEINJECTOR_HOTKEYSETTINGSTAB_H
#define CUTEINJECTOR_HOTKEYSETTINGSTAB_H

#include "qwidget.h"


class QGridLayout;

class HotKeySettingsTab : public QWidget
{
    Q_OBJECT

public:
    HotKeySettingsTab(QWidget* parent = nullptr);

signals:
    void hotKeyChanged();

private:
    QGridLayout* layout = nullptr;
};


#endif //CUTEINJECTOR_HOTKEYSETTINGSTAB_H
