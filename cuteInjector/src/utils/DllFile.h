#ifndef DLLFILE_H
#define DLLFILE_H

#include <QString>
#include <Windows.h>


struct DosHeader {
    WORD   e_magic;
    WORD   e_cblp;
    WORD   e_cp;
    WORD   e_crlc;
    WORD   e_cparhdr;
    WORD   e_minalloc;
    WORD   e_maxalloc;
    WORD   e_ss;
    WORD   e_sp;
    WORD   e_csum;
    WORD   e_ip;
    WORD   e_cs;
    WORD   e_lfarlc;
    WORD   e_ovno;
    WORD   e_res[4];
    WORD   e_oemid;
    WORD   e_oeminfo;
    WORD   e_res2[10];
    LONG   e_lfanew;
};


struct ImageFileHeader {
    WORD    Machine;
    WORD    NumberOfSections;
    DWORD   TimeDateStamp;
    DWORD   PointerToSymbolTable;
    DWORD   NumberOfSymbols;
    WORD    SizeOfOptionalHeader;
    WORD    Characteristics;
};


class DllFile
{
public:
    DllFile();
    DllFile(const QString &path);
    DllFile(const QString &path, const QString &name, const QString &arch);
    ~DllFile() = default;

    QString path;
    QString name;
    QString arch;

private:
    static int getPlatform(const QString &path);
};

#endif // DLLFILE_H
