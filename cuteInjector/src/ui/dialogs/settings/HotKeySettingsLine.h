#ifndef CUTEINJECTOR_HOTKEYSETTINGSLINE_H
#define CUTEINJECTOR_HOTKEYSETTINGSLINE_H


#include "qwidget.h"


class QHBoxLayout;

class HotKeySettingsLine : public QWidget
{
    Q_OBJECT

public:
    HotKeySettingsLine(const QString& name, const QString& settingsKey, QWidget* parent = nullptr);

signals:
    void hotKeyChanged();

private:
    QHBoxLayout* layout = nullptr;
};


#endif //CUTEINJECTOR_HOTKEYSETTINGSLINE_H
