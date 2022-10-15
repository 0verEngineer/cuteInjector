#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "src/ui/dialogs/SelectProcessDialog.h"
#include "../utils/process/ProcessManager.h"
#include "../utils/injector/Injector.h"
#include "../utils/DllFile.h"

#include <QMainWindow>
#include <QStandardItemModel>
#include <QDialog>
#include <QPixmap>
#include <QSettings>


class SettingsDialog;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void refreshDllFileTableViewContents();
    void updateInjectButtonAndAction();
    void updateRemoveButtonAndAction();
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
    void openSettings();

private:
    void createActions();
    void setActionHotkeys();
    void registerGlobalWindowsHotKey(const QString& hotKeySequence, const QString& settingsKey);
    void unregisterGlobalWindowsHotKeys();

    bool nativeEvent(const QByteArray &eventType, void *message, long long *result);

public:
    struct Actions
    {
        QAction* injectAct = nullptr;
        QAction* addFileAct = nullptr;
        QAction* removeFileAct = nullptr;
        QAction* selectProcessAct = nullptr;
        QAction* openSettingsAct = nullptr;
    };
    Actions actions;

private:
    Ui::MainWindow *ui;
    Injector injector;
    ProcessManager* processManager;
    Process* selectedProcess;
    std::vector<DllFile> dllFiles;
    DllFile selectedDll;

    QStandardItemModel* dllFileTableViewModel = nullptr;
    std::vector<std::unique_ptr<QStandardItem>> dllFileTableItems;

    SelectProcessDialog* selectProcessDialog = nullptr;

    std::mutex selectedProcessMutex;

    QList<int> globalRegisteredHotkeyIds;
};

#endif // MAINWINDOW_H
