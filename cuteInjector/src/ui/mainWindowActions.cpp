#include "mainWindow.h"
#include "ui_mainWindow.h"

#include "qfiledialog.h"
#include "qaction.h"


void MainWindow::createActions()
{
    actions.injectAct = new QAction("Inject", this);
    actions.injectAct->setShortcut(QKeySequence::fromString("ctrl+i"));
    connect(actions.injectAct, &QAction::triggered, this, &MainWindow::inject);
    addAction(actions.injectAct);

    actions.addFileAct = new QAction("Add File", this);
    actions.addFileAct->setShortcut(QKeySequence::fromString("ctrl+a"));
    connect(actions.addFileAct, &QAction::triggered, this, &MainWindow::addFile);
    addAction(actions.addFileAct);

    actions.removeFileAct = new QAction("Remove File", this);
    actions.removeFileAct->setShortcut(QKeySequence::fromString("ctrl+r"));
    connect(actions.removeFileAct, &QAction::triggered, this, &MainWindow::removeFile);
    addAction(actions.removeFileAct);

    actions.selectProcessAct = new QAction("Select Process", this);
    actions.selectProcessAct->setShortcut(QKeySequence::fromString("ctrl+s"));
    connect(actions.selectProcessAct, &QAction::triggered, this, &MainWindow::selectProcess);
    addAction(actions.selectProcessAct);
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

    ui->buttonRemoveFile->setEnabled(true);
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

    // Disable the remove button
    if (dllFileTableViewModel->rowCount() <= 0)
        ui->buttonRemoveFile->setEnabled(false);
}


void MainWindow::selectProcess()
{
    selectProcessDialog->showWindow();
}


