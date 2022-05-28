#include "injector.h"

#include <filesystem>


Injector::Injector(QObject *parent)
    : QObject{parent}
{

}


void Injector::run(const DWORD &procId, const QString &dllPath, const QString &arch)
{
    DWORD exitCode = 0;
    std::string sDllPath = dllPath.toStdString();

    HANDLE processHandle = OpenProcess(
                PROCESS_CREATE_THREAD|PROCESS_VM_READ|PROCESS_VM_WRITE|PROCESS_VM_OPERATION, FALSE, procId);

    PTHREAD_START_ROUTINE loadLibraryAddress;
    bool isInjectable = true;

    const std::filesystem::path fsFilePath = dllPath.toStdString();
    if (!std::filesystem::exists(fsFilePath))
    {
        CloseHandle(processHandle);
        emit signalInjectionFinished(false, "DllFile not found");
        return;
    }

    if (arch == "x64")
    {
        HMODULE kernel32base = GetModuleHandle(L"kernel32.dll");
        FARPROC loadLibraryAddressX64 = GetProcAddress(kernel32base, "LoadLibraryA");
        loadLibraryAddress = reinterpret_cast<LPTHREAD_START_ROUTINE>(loadLibraryAddressX64);
    }
    else
    {
        int loadLibraryAddressX32 = system("x86Bridge.exe");
        loadLibraryAddress = reinterpret_cast<LPTHREAD_START_ROUTINE>(loadLibraryAddressX32);

        if (loadLibraryAddress == 0)
            isInjectable = false;
    }

    if (isInjectable)
    {
        // Allocate space in the process
        void* baseAddress = VirtualAllocEx(processHandle, NULL, strlen(sDllPath.c_str()) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

        if (baseAddress != 0)
        {
            // Write the dll path into the allocated space
            WriteProcessMemory(processHandle, baseAddress, sDllPath.c_str(), strlen(sDllPath.c_str()) + 1, NULL);


            HANDLE thread = CreateRemoteThread(
                processHandle,
                NULL,
                0,
                loadLibraryAddress,
                baseAddress,
                0,
                NULL
            );

            // Block thread execution to make sure that our dll is injected
            WaitForSingleObject(thread, INFINITE);
            GetExitCodeThread(thread, &exitCode);

            VirtualFreeEx(processHandle, baseAddress, 0, MEM_RELEASE);
            CloseHandle(thread);

            emit signalInjectionFinished(true, "Successfully injected");
        }
        else
        {
            emit signalInjectionFinished(false, "Injection failed");
        }
    }
    else
    {
        emit signalInjectionFinished(false, "Injection failed");
    }

    CloseHandle(processHandle);
}
