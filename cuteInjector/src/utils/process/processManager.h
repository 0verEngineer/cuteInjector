#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include "process.h"

#include <Windows.h>
#include <thread>
#include <QObject>
#include <mutex>


class ProcessManager: public QObject
{
    Q_OBJECT

public:
    explicit ProcessManager(QObject *parent = nullptr);
    ~ProcessManager();

    void queryProcessInformation(const QString name);
    void startProcessScanThread(Process* selectedProcess, std::mutex &selectedProcessMutex);
    void scanLoop(Process* selectedProcess, std::mutex &selectedProcessMutex);
    std::vector<std::unique_ptr<Process>> *getUpdatedProcessList();

    QPixmap getCachedIconOfExeFile(const QString &path, int size);
    static HICON getStandardHIcon();
    static QPixmap getStandardIcon();

private:
    std::vector<std::unique_ptr<Process>> processList;
    std::atomic<bool> isScanLoopThreadRunning = false;
    std::thread scanLoopThread;

    static QHash<QString, QPixmap> iconCache;

signals:
    void processFound(const DWORD selectedProcessId,
                      const QString name,
                      const QString path,
                      const QString architecture
    );
    void processNotFound();

};

#endif // PROCESSMANAGER_H
