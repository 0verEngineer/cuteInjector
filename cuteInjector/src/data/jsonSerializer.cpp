#include <QMessageBox>
#include "jsonSerializer.h"


JsonSerializer* JsonSerializer::instance_ = nullptr;

JsonSerializer::JsonSerializer()
{
    path = QDir::homePath() + "/.CuteInjector/data.json";
}

bool JsonSerializer::loadData(std::vector<DllFile> &dllFiles, Process &selectedProcess)
{
    std::ifstream file(path.toStdString());
    if (file.fail())
        return false;

    json j;
    bool jsonReadError = false;

    try
    {
        file >> j;
    }
    catch (...)
    {
        jsonReadError = true;
    }

    if (j.empty())
    {
        file.close();
        return false;
    }

    try
    {
        selectedProcess.name = QString::fromStdString(j["selectedProcess"]["name"].get<std::string>());

        for (int i = 0; i < j["dllFiles"].size(); i++)
        {
            QString name = QString::fromStdString(j["dllFiles"][i]["name"].get<std::string>());
            QString path = QString::fromStdString(j["dllFiles"][i]["path"].get<std::string>());
            QString arch = QString::fromStdString(j["dllFiles"][i]["arch"].get<std::string>());

            dllFiles.push_back(DllFile(path, name, arch));
        }
    }
    catch (nlohmann::detail::type_error&)
    {
        jsonReadError = true;
    }

    // A new file will be created with default values
    if (jsonReadError)
    {
        qWarning() << "Unable to read json " + path;
        file.close();
        QFile dataFile = QFile(path);
        if (dataFile.exists())
        {
            QString newAbsolutePath = dataFile.fileName();
            newAbsolutePath.replace(
                    ".json",
                    "_" + QDateTime::currentDateTime().toString().replace(" ", "_").replace(":", "-") + ".json"
            );

            if (QFile::rename(dataFile.fileName(), newAbsolutePath))
                qInfo() << "Backup of data file created: " << newAbsolutePath;
        }

        return false;
    }

    file.close();
    return true;
}

void JsonSerializer::saveData(std::vector<DllFile> &dllFiles, Process &selectedProcess)
{
    json j;

    j["selectedProcess"]["name"] = selectedProcess.name.toStdString();

    for (int i = 0; i < dllFiles.size(); i++)
    {
        j["dllFiles"][i]["name"] = dllFiles[i].name.toStdString();
        j["dllFiles"][i]["path"] = dllFiles[i].path.toStdString();
        j["dllFiles"][i]["arch"] = dllFiles[i].arch.toStdString();
    }

    std::ofstream outFile(path.toStdString());
    outFile << std::setw(4) << j << std::endl;
    outFile.close();
}

