#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include "src/utils/DllFile.h"
#include "src/utils/process/Process.h"

#include "vendor/nlohmann/json.hpp"

#include <fstream>
#include <iomanip>
#include <QDir>


using json = nlohmann::json;
using std::fstream;


class JsonSerializer
{
public:
    static JsonSerializer* instance()
    {
        if (instance_ == nullptr)
            instance_ = new JsonSerializer;
        return instance_;
    }

    bool loadData(std::vector<DllFile> &dllFiles, Process &selectedProcess);
    void saveData(std::vector<DllFile> &dllFiles, Process &selectedProcess);

private:
    static JsonSerializer* instance_;
    QString path = "";

    JsonSerializer();
};

#endif // JSONSERIALIZER_H
