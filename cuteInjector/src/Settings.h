#ifndef CUTEINJECTOR_SETTINGS_H
#define CUTEINJECTOR_SETTINGS_H


#include "qvariant.h"

class Settings {

public:
    static Settings* instance();

    QVariant value(const QString& key) const;
    void setValue(const QString& key, const QVariant& value) const;

private:
    Settings();

    static Settings* instance_;
    QString fileName = "";
    QMap<QString, QString> defaults;
};


#endif //CUTEINJECTOR_SETTINGS_H
