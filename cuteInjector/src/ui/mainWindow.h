#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "selectProcessDialog.h"
#include "../utils/process/processManager.h"
#include "../utils/injector/injector.h"
#include "../utils/dllFile.h"

#include <QMainWindow>
#include <QStandardItemModel>
#include <QDialog>
#include <QPixmap>


QT_BEGIN_NAMESPACE
namespace Ui { class mainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void refreshDllFileTableViewContents();
    void updateInjectButtonAndAction();
    void selectDllFileInTable(int index);
    DllFile findDllInDllFiles(const QString path);

private slots:
    void slotProcessSelected(const QString name);
    void slotProcessFound(const DWORD procId,
                          const QString name,
                          const QString path,
                          const QString architecture
                          );
    void slotProcessNotFound();
    void slotInjectionFinished(bool success, const QString message);
    void inputProcessNameTextChanged(const QString &text);
    void tableViewDllFilesClicked(const QModelIndex &index);
    void inject();
    void addFile();
    void removeFile();
    void selectProcess();

private:
    void createActions();

public:
    struct Actions
    {
        QAction* injectAct = nullptr;
        QAction* addFileAct = nullptr;
        QAction* removeFileAct = nullptr;
        QAction* selectProcessAct = nullptr;
    };
    Actions actions;

private:
    Ui::mainWindow *ui;
    Injector injector;
    ProcessManager* processManager;
    Process* selectedProcess;
    std::vector<DllFile> dllFiles;
    DllFile selectedDll;

    QStandardItemModel* dllFileTableViewModel = nullptr;
    std::vector<std::unique_ptr<QStandardItem>> dllFileTableItems;

    SelectProcessDialog* selectProcessDialog = nullptr;

    std::mutex selectedProcessMutex;
};

#endif // MAINWINDOW_H
