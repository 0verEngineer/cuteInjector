#ifndef INJECTOR_H
#define INJECTOR_H

#include <QObject>
#include <Windows.h>


class Injector : public QObject
{
    Q_OBJECT
public:
    explicit Injector(QObject *parent = nullptr);

    void run(const DWORD &procId, const QString &dllPath, const QString &arch);

signals:
    void signalInjectionFinished(bool success, const QString message);
};

#endif // INJECTOR_H
