#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>
#include <QPixmap>
#include <Windows.h>
#include <mutex>


class Process
{
public:

    Process();

    Process(const DWORD &id,
            const QString &name,
            const QString &path,
            const QString &architecture
            );

    Process(const DWORD &id,
            const QString &name,
            const QString &path,
            const QString &architecture,
            const QPixmap &icon
            );

    ~Process() = default;

    QString getIdAsQString();

    DWORD id;
    QString name;
    QString path;
    QString architecture;
    QString searchName;

    QPixmap icon;
};

#endif // PROCESS_H
