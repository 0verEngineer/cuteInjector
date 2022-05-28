#include "dllFile.h"

#include <Windows.h>
#include <QStringList>


#define ___IMAGE_DOS_SIGNATURE                  0x5A4D
#define ___IMAGE_NT_OPTIONAL_HDR32_MAGIC        0x10b
#define ___IMAGE_NT_OPTIONAL_HDR64_MAGIC        0x20b


DllFile::DllFile()
{
    this->path = "";
    this->name = "";
    this->arch = "";
}


DllFile::DllFile(const QString &path)
{
    this->path = path;

    QStringList splittedPath = path.split("/");
    this->name = splittedPath.last();

    int iArch = getPlatform(path);
    if (iArch == 32)
        this->arch = "x86";
    else if (iArch == 64)
        this->arch = "x64";
    else
        this->arch = QString::number(iArch);
}


DllFile::DllFile(const QString &path, const QString &name, const QString &arch)
{
    this->path = path;
    this->name = name;
    this->arch = arch;
}


// Thanks to 0xRick https://0xrick.github.io
int DllFile::getPlatform(const QString &path)
{
    FILE* file;
    fopen_s(&file, path.toStdString().c_str(), "rb");

    if (file == nullptr)
    {
        return 0;
    }

    WORD peFileType;

    fseek(file, 0, SEEK_SET);

    DosHeader dosHeader;

    fread(&dosHeader, sizeof(DosHeader), 1, file);

    if (dosHeader.e_magic != ___IMAGE_DOS_SIGNATURE) {
        return 0;
    }

    fseek(file, (dosHeader.e_lfanew + sizeof(DWORD) + sizeof(ImageFileHeader)), SEEK_SET);
    fread(&peFileType, sizeof(WORD), 1, file);
    fclose(file);

    if (peFileType == ___IMAGE_NT_OPTIONAL_HDR32_MAGIC)
        return 32;

    else if (peFileType == ___IMAGE_NT_OPTIONAL_HDR64_MAGIC)
        return 64;
    else
        return 0;
}
