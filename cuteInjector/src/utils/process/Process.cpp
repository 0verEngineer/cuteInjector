#include "Process.h"
#include "ProcessManager.h"


Process::Process()
{
    this->id = 0;
    this->path = "";
    this->architecture = "";
    this->name = "";
    this->icon = ProcessManager::getStandardIcon();
}


Process::Process(const DWORD &id,
                 const QString &name,
                 const QString &path,
                 const QString &architecture
                 )
{
    this->id = id;
    this->path = path;
    this->architecture = architecture;
    this->name = name;
    this->icon = ProcessManager::getStandardIcon();
}


Process::Process(const DWORD &id,
                 const QString &name,
                 const QString &path,
                 const QString &architecture,
                 const QPixmap &icon)
{
    this->id = id;
    this->path = path;
    this->architecture = architecture;
    this->name = name;
    this->icon = icon;
}


QString Process::getIdAsQString()
{
    QString id = QString::number(this->id, 16);

    while (id.length() < 8)
        id.push_front("0");

    return id.toUpper();
}
