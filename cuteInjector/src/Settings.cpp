
#include "qsettings.h"
#include "qfileinfo.h"
#include "qdir.h"

#include "Settings.h"


Settings* Settings::instance_ = nullptr;


Settings *Settings::instance()
{
    if (instance_ == nullptr)
        instance_ = new Settings();

    return instance_;
}

Settings::Settings()
{
    defaults.insert("hotkeys/inject", "ctrl+i");
    defaults.insert("hotkeys/addFile", "ctrl+a");
    defaults.insert("hotkeys/removeFile", "ctrl+r");
    defaults.insert("hotkeys/selectProcess", "ctrl+s");

    fileName = QDir::homePath() + "/.CuteInjector/settings.ini";
    QFile file = QFile(fileName);
    if (!file.exists())
    {
        // Create the file
        file.open(QIODevice::ReadOnly);
        file.close();
    }

    // Write all defaults on each start
    QSettings settings = QSettings(fileName, QSettings::IniFormat);

    for (const auto& key : defaults.keys())
    {
        if (!settings.contains(key))
            settings.setValue(key, defaults.value(key));
    }
}


QVariant Settings::value(const QString &key) const
{
    QSettings settings = QSettings(fileName, QSettings::IniFormat);
    return settings.value(key, defaults.value(key));
}


void Settings::setValue(const QString &key, const QVariant &value) const
{
    QSettings settings = QSettings(fileName, QSettings::IniFormat);
    settings.setValue(key, value);
}



