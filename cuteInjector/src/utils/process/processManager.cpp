#include "processManager.h"

#include <psapi.h>
#include <TlHelp32.h>
#include <shlobj_core.h>

#include <QDebug>
#include <future>
#include <QImage>
#include <QPixmap>


ProcessManager::ProcessManager(QObject *parent)
    : QObject{parent}
{

}


ProcessManager::~ProcessManager()
{
    if (isScanLoopThreadRunning)
        isScanLoopThreadRunning = false;

    if (scanLoopThread.joinable())
        scanLoopThread.join();
}


void ProcessManager::queryProcessInformation(const QString name)
{
    bool found = false;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);

        if (Process32First(hSnap, &procEntry))
        {
            do
            {
                if (QString::fromWCharArray(procEntry.szExeFile) == name)
                {
                    SetLastError(0);
                    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, procEntry.th32ProcessID);
                    DWORD errorID = ::GetLastError();
                    if (errorID != 0)
                        continue;

                    BOOL wIsX86Process = FALSE;
                    IsWow64Process(processHandle, &wIsX86Process);

                    QString arch = "x64";
                    if (wIsX86Process) arch = "x86";

                    wchar_t filePath[MAX_PATH];
                    GetModuleFileNameExW(processHandle, NULL, filePath, MAX_PATH);

                    CloseHandle(processHandle);

                    emit processFound(procEntry.th32ProcessID,
                                      name,
                                      QString::fromStdWString(filePath),
                                      arch
                                      );
                    found = true;
                    break;
                }

            } while (Process32Next(hSnap, &procEntry));
        }
    }

    if (!found)
        emit processNotFound();
    CloseHandle(hSnap);
}


void ProcessManager::startAsyncProcessScan(const QString &name)
{
    if (!name.endsWith(".exe"))
        return;

    auto future = std::async(std::launch::async, &ProcessManager::queryProcessInformation, this, name);
}


void ProcessManager::startScanLoop(Process &selectedProcess, std::mutex &selectedProcessMutex)
{
    scanLoopThread = std::thread(&ProcessManager::scanLoop, this, std::ref(selectedProcess), std::ref(selectedProcessMutex));
    scanLoopThread.detach();
}


void ProcessManager::scanLoop(Process &selectedProcess, std::mutex &selectedProcessMutex)
{
    isScanLoopThreadRunning = true;

    while (isScanLoopThreadRunning)
    {
        std::unique_lock<std::mutex> lock(selectedProcessMutex);
        QString name = selectedProcess.name;
        lock.unlock();

        queryProcessInformation(name);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


std::vector<std::unique_ptr<Process>> *ProcessManager::getUpdatedProcessList()
{
    processList.clear();
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);

        if (Process32First(hSnap, &procEntry))
        {
            do
            {
                SetLastError(0);
                HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, procEntry.th32ProcessID);
                DWORD errorID = ::GetLastError();
                if (errorID != 0)
                    continue;

                wchar_t filePath[MAX_PATH];
                GetModuleFileNameExW(processHandle, NULL, filePath, MAX_PATH);

                CloseHandle(processHandle);

                QPixmap icon = getIconFromExe(QString::fromStdWString(filePath), 16);

                processList.push_back(std::make_unique<Process>(
                                          procEntry.th32ProcessID,
                                          QString::fromWCharArray(procEntry.szExeFile),
                                          "", // Arch is not relevant here
                                          QString::fromWCharArray(filePath),
                                          icon
                                          ));

            } while (Process32Next(hSnap, &procEntry));
        }
    }
    CloseHandle(hSnap);

    return &processList;
}


QPixmap ProcessManager::getIconFromExe(const QString &path, int size)
{
    HICON hIcon = 0;
    SHDefExtractIcon(path.toStdWString().c_str(), 0, 0, &hIcon, NULL, size);

    // Default icon
    if (hIcon == 0) {
        hIcon = getStandardHIcon();
    }

    QPixmap icon = QPixmap::fromImage(QImage::fromHICON(hIcon).scaled(size, size, Qt::IgnoreAspectRatio, Qt::FastTransformation));
    DestroyIcon(hIcon);
    return icon;
}


HICON ProcessManager::getStandardHIcon()
{
    return ::LoadIcon(0, IDI_APPLICATION);
}


QPixmap ProcessManager::getStandardIcon()
{
    HICON hIcon = getStandardHIcon();
    QPixmap icon = QPixmap::fromImage(QImage::fromHICON(hIcon));
    DestroyIcon(hIcon);
    return icon;
}
