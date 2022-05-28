#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include "../utils/dllFile.h"
#include "../utils/process/process.h"

#include "../../vendor/nlohmann/json.hpp"

#include <fstream>
#include <iomanip>
#include <QDir>


using json = nlohmann::json;
using std::fstream;


namespace jsonSerializer
{
    std::string path = QString(QDir::currentPath() + "/data.json").toStdString();

    bool load(std::vector<DllFile> &dllFiles, Process &selectedProcess)
    {
        std::ifstream file(path);
        if (file.fail())
            return false;

        json j;
        file >> j;

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
        catch (nlohmann::detail::type_error)
        {
            qDebug() << "ERROR: Unable to read json";
        }

        file.close();
        return true;
    }


    void save(std::vector<DllFile> &dllFiles, Process &selectedProcess)
    {
        json j;

        j["selectedProcess"]["name"] = selectedProcess.name.toStdString();

        for (int i = 0; i < dllFiles.size(); i++)
        {
            j["dllFiles"][i]["name"] = dllFiles[i].name.toStdString();
            j["dllFiles"][i]["path"] = dllFiles[i].path.toStdString();
            j["dllFiles"][i]["arch"] = dllFiles[i].arch.toStdString();
        }

        std::ofstream outFile(path);
        outFile << std::setw(4) << j << std::endl;
        outFile.close();
    }
}

#endif // JSONSERIALIZER_H
