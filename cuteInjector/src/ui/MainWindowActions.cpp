#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "src/Settings.h"
#include "src/ui/dialogs/settings/SettingsDialog.h"

#include "qfiledialog.h"
#include "qaction.h"
#include "Windows.h"


void MainWindow::createActions()
{
    actions.injectAct = new QAction("Inject", this);
    actions.injectAct->setDisabled(true);
    connect(actions.injectAct, &QAction::triggered, this, &MainWindow::inject);
    addAction(actions.injectAct);

    actions.addFileAct = new QAction("Add File", this);
    connect(actions.addFileAct, &QAction::triggered, this, &MainWindow::addFile);
    addAction(actions.addFileAct);

    actions.removeFileAct = new QAction("Remove File", this);
    actions.removeFileAct->setDisabled(true);
    connect(actions.removeFileAct, &QAction::triggered, this, &MainWindow::removeFile);
    addAction(actions.removeFileAct);

    actions.selectProcessAct = new QAction("Select Process", this);
    connect(actions.selectProcessAct, &QAction::triggered, this, &MainWindow::selectProcess);
    addAction(actions.selectProcessAct);

    actions.openSettingsAct = new QAction("Settings", this);
    connect(actions.openSettingsAct, &QAction::triggered, this, &MainWindow::openSettings);
    addAction(actions.openSettingsAct);

    setActionHotkeys();
}


void MainWindow::setActionHotkeys()
{
    Settings* settings = Settings::instance();

    if (globalRegisteredHotkeyIds.size() > 0)
        unregisterGlobalWindowsHotKeys();

    if (settings->value("globalHotkeys/inject").toBool())
        registerGlobalWindowsHotKey(settings->value("hotkeys/inject").toString(), "globalHotkeys/inject");
    else
        actions.injectAct->setShortcut(QKeySequence::fromString(settings->value("hotkeys/inject").toString()));

    actions.addFileAct->setShortcut(QKeySequence::fromString(settings->value("hotkeys/addFile").toString()));
    actions.removeFileAct->setShortcut(QKeySequence::fromString(settings->value("hotkeys/removeFile").toString()));
    actions.selectProcessAct->setShortcut(QKeySequence::fromString(settings->value("hotkeys/selectProcess").toString()));
    actions.openSettingsAct->setShortcut(QKeySequence::fromString(settings->value("hotkeys/openSettings").toString()));
}


void MainWindow::registerGlobalWindowsHotKey(const QString& hotKeySequence, const QString& settingsKey)
{
    QKeySequence seq = QKeySequence::fromString(hotKeySequence);
    QList<QString> keys = seq.toString().split("+");

    QList<UINT> modifiers;
    int modifier = 0;

    for (const auto& key : keys)
    {
        if (key == "Ctrl")
            modifier += MOD_CONTROL;

        else if (key == "Alt")
            modifier += MOD_ALT;

        else if (key == "Shift")
            modifier += MOD_SHIFT;

        else
        {
            if (key != keys.at(keys.size() - 1))
            {
                qWarning() << "Only Ctrl, Alt and Shift modifiers in combination with a single key is allowed on a global hotkey. ";
                return;
            }
        }
    }

    if(modifier == 0)
    {
        qWarning() << "Unable to register global hotkey, modifier is 0";
        return;
    }

    // Get the key code
    QString key = keys.at(keys.size() - 1);
    QKeySequence keyOnlyKeySequence = QKeySequence::fromString(key);

    int id = 0;
    for (const auto& registeredId : globalRegisteredHotkeyIds)
    {
        if (registeredId >= id)
            id = registeredId + 1;
    }

    if (!RegisterHotKey(HWND(effectiveWinId()), id, modifier, keyOnlyKeySequence[0].key()))
    {
        qWarning() << "Unable to register global hotkey: " << hotKeySequence;
        return;
    }

    globalRegisteredHotkeyIds.push_back(id);
}


void MainWindow::unregisterGlobalWindowsHotKeys()
{
    for (const auto& id : globalRegisteredHotkeyIds)
    {
        if (UnregisterHotKey(HWND(effectiveWinId()), id) == 0)
            qWarning() << "Unable to unregister global hotkey: ";
    }

    globalRegisteredHotkeyIds.clear();
}


void MainWindow::inject()
{
    QString dllPath = selectedDll.path;

    selectedProcessMutex.lock();
    DWORD id = selectedProcess->id;
    QString arch = selectedProcess->architecture;
    selectedProcessMutex.unlock();

    if (dllPath.length() < 1 || id == 0)
    {
        ui->statusbar->showMessage("No file selected", 8000);
        updateInjectButtonAndAction();
        return;
    }

    injector.run(id, dllPath, arch);
}


void MainWindow::addFile()
{
    QUrl url = QFileDialog::getOpenFileUrl(this, "Select dll", QDir::currentPath(), "dll Files (*.dll)");
    if (!url.isValid())
        return;

    QString path = url.path().mid(1, url.path().length());

    // Do not allow the same dll multiple times
    if(findDllInDllFiles(path).path.length() > 0)
    {
        ui->statusbar->showMessage("File already added", 8000);

        // Select the dll instead
        for (int i = 0; i < dllFiles.size(); i++)
        {
            if (dllFiles[i].path == path)
            {
                selectDllFileInTable(i);
            }
        }
        return;
    }

    dllFiles.push_back(DllFile(path));

    refreshDllFileTableViewContents();

    // Always select the newest dll
    if (dllFiles.size() > 0)
    {
        selectDllFileInTable(dllFiles.size() - 1);
    }

    updateInjectButtonAndAction();
    updateRemoveButtonAndAction();
}


void MainWindow::removeFile()
{
    QModelIndexList indexes = ui->tableViewDllFiles->selectionModel()->selectedRows();

    while (!indexes.isEmpty())
    {
        dllFiles.erase(dllFiles.begin() + indexes.last().row());
        indexes.removeLast();
    }

    refreshDllFileTableViewContents();

    selectedDll = DllFile();

    updateInjectButtonAndAction();
    updateRemoveButtonAndAction();
}


void MainWindow::selectProcess()
{
    selectProcessDialog->showWindow();
}


void MainWindow::openSettings()
{
    SettingsDialog settingsDialog = SettingsDialog();

    connect(&settingsDialog, &SettingsDialog::hotKeyChanged, this, [this] {
        setActionHotkeys();
    });

    settingsDialog.execDialog();
}
