#include "mainWindow.h"
#include "ui_mainWindow.h"
#include "textInfoWindow.h"
#include "../helpers/jsonSerializer.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QTableWidget>
#include <QBoxLayout>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainWindow)
{  
    ui->setupUi(this);

    dllFileTableViewModel = new QStandardItemModel(0, 3, this);
    ui->tableViewDllFiles->horizontalHeader()->hide();
    ui->tableViewDllFiles->verticalHeader()->hide();
    ui->tableViewDllFiles->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewDllFiles->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Only allow single file selection for now
    ui->tableViewDllFiles->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->iconProcess->setScaledContents(true);

    selectProcessDialog = new SelectProcessDialog(&processManager, this);

    connect(selectProcessDialog, &SelectProcessDialog::signalProcessSelected, this, &MainWindow::slotProcessSelected);
    connect(&processManager, &ProcessManager::processFound, this, &MainWindow::slotProcessFound);
    connect(&processManager, &ProcessManager::processNotFound, this, &MainWindow::slotProcessNotFound);
    connect(&injector, &Injector::signalInjectionFinished, this, &MainWindow::slotInjectionFinished);

    std::lock_guard<std::mutex> lock(selectedProcessMutex);
    selectedProcess = Process();
    selectedDll = DllFile();

    // Set the deserialized data in the ui and start the process scanner
    if (jsonSerializer::load(dllFiles, selectedProcess))
    {
       refreshDllFileTableViewContents();

       if (selectedProcess.name.length() > 0)
       {
           ui->inputProcessName->setText(selectedProcess.name);
           processManager.startAsyncProcessScan(selectedProcess.name);
       }
    }

    processManager.startScanLoop(selectedProcess, selectedProcessMutex);
}


MainWindow::~MainWindow()
{
    disconnect(selectProcessDialog, &SelectProcessDialog::signalProcessSelected, this, &MainWindow::slotProcessSelected);
    disconnect(&processManager, &ProcessManager::processFound, this, &MainWindow::slotProcessFound);
    disconnect(&processManager, &ProcessManager::processNotFound, this, &MainWindow::slotProcessNotFound);
    disconnect(&injector, &Injector::signalInjectionFinished, this, &MainWindow::slotInjectionFinished);

    std::lock_guard<std::mutex> lock(selectedProcessMutex);
    jsonSerializer::save(dllFiles, selectedProcess);

    delete selectProcessDialog;
    delete dllFileTableViewModel;

    delete ui;
}


void MainWindow::refreshDllFileTableViewContents()
{
    // Delete old stack allocated items, this needs to happen before setRowCount
    if (!dllFileTableItems.empty())
        dllFileTableItems.clear();

    dllFileTableViewModel->setRowCount(dllFiles.size());

    for (int i = 0; i < dllFiles.size(); i++)
    {
        dllFileTableItems.push_back(std::make_unique<QStandardItem>(dllFiles.at(i).arch));
        dllFileTableViewModel->setItem(i, 0, dllFileTableItems.at(dllFileTableItems.size() - 1).get());

        dllFileTableItems.push_back(std::make_unique<QStandardItem>(dllFiles.at(i).name));
        dllFileTableViewModel->setItem(i, 1, dllFileTableItems.at(dllFileTableItems.size() - 1).get());

        dllFileTableItems.push_back(std::make_unique<QStandardItem>(dllFiles.at(i).path));
        dllFileTableViewModel->setItem(i, 2, dllFileTableItems.at(dllFileTableItems.size() - 1).get());
    }

    ui->tableViewDllFiles->setModel(dllFileTableViewModel);
    ui->tableViewDllFiles->resizeColumnsToContents();
}


// Used to activate / deactivate the inject button
void MainWindow::injectButtonToggle()
{
    std::unique_lock<std::mutex> lock(selectedProcessMutex);
    DWORD id = selectedProcess.id;
    QString path = selectedProcess.path;
    QString arch = selectedProcess.architecture;
    lock.unlock();

    QString selectedDllPath = selectedDll.path;

    bool enabled = false;

    if (id != 0 &&
        selectedDll.path.length() > 0 &&
        selectedDll.arch == arch)
    {
        const std::filesystem::path fsFilePath = selectedDllPath.toStdString();
        if (!std::filesystem::exists(fsFilePath))
            ui->statusbar->showMessage("DllFile not found", 4000);
        else
            enabled = true;
    }

    ui->buttonInjectFile->setEnabled(enabled);
}


void MainWindow::selectDllFileInTable(int index)
{
    selectedDll = DllFile(dllFiles.at(index).path,
                          dllFiles.at(index).name,
                          dllFiles.at(index).arch
                          );
    ui->tableViewDllFiles->selectRow(index);
}


DllFile MainWindow::findDllInDllFiles(const QString path)
{
    for (int i = 0; i < dllFiles.size(); i++)
    {
        if (dllFiles.at(i).path == path)
        {
            return DllFile(path, dllFiles.at(i).name, dllFiles.at(i).arch);
        }
    }

    return DllFile();
}


void MainWindow::slotProcessSelected(const QString name)
{
    ui->inputProcessName->setText(name);
    this->on_inputProcessName_textChanged(name);
}


void MainWindow::slotProcessFound(const DWORD procId,
                                  const QString name,
                                  const QString path,
                                  const QString architecture
                                  )
{
    bool hasChanged = false;
    std::unique_lock<std::mutex> lock(selectedProcessMutex);
    if (selectedProcess.id != procId)
    {
        hasChanged = true;
        selectedProcess = Process(procId, name, path, architecture);
    }
    lock.unlock();

    if (hasChanged)
    {
        QPixmap icon = processManager.getIconFromExe(path, 64);
        ui->iconProcess->setPixmap(icon);

        ui->inputProcessId->setText(QString::number(procId));
        ui->inputArchitecture->setText(architecture);

        injectButtonToggle();
    }
}


void MainWindow::slotProcessNotFound()
{
    std::unique_lock<std::mutex> lock(selectedProcessMutex);
    selectedProcess.id = 0;
    selectedProcess.path = "";
    selectedProcess.architecture = "";
    lock.unlock();

    ui->inputProcessId->setText("");
    ui->inputArchitecture->setText("");

    injectButtonToggle();
}


void MainWindow::slotInjectionFinished(bool success, const QString message)
{
    ui->statusbar->showMessage(message, 8000);
}


void MainWindow::on_buttonSelectProcess_clicked()
{
    selectProcessDialog->showWindow();
}


void MainWindow::on_buttonAddFile_clicked()
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

    injectButtonToggle();

    ui->buttonRemoveFile->setEnabled(true);
}


void MainWindow::on_buttonRemoveFile_clicked()
{
    QModelIndexList indexes = ui->tableViewDllFiles->selectionModel()->selectedRows();

    while (!indexes.isEmpty())
    {
        dllFiles.erase(dllFiles.begin() + indexes.last().row());
        indexes.removeLast();
    }

    refreshDllFileTableViewContents();

    selectedDll = DllFile();

    injectButtonToggle();

    // Disable the remove button
    if (dllFileTableViewModel->rowCount() <= 0)
        ui->buttonRemoveFile->setEnabled(false);
}


void MainWindow::on_buttonInjectFile_clicked()
{
    QString dllPath = selectedDll.path;

    std::unique_lock<std::mutex> lock(selectedProcessMutex);
    DWORD id = selectedProcess.id;
    QString arch = selectedProcess.architecture;
    lock.unlock();

    if (dllPath.length() < 1 || id == 0)
    {
        ui->statusbar->showMessage("No file selected", 8000);
        injectButtonToggle();
        return;
    }

    injector.run(id, dllPath, arch);
}


void MainWindow::on_inputProcessName_textChanged(const QString &text)
{
    ui->inputProcessId->setText("");
    ui->inputArchitecture->setText("");

    selectedProcess.id = 0;
    selectedProcess.path = "";
    selectedProcess.architecture = "";

    processManager.startAsyncProcessScan(text);
}


void MainWindow::on_tableViewDllFiles_clicked(const QModelIndex &index)
{
    QModelIndexList indexList = ui->tableViewDllFiles->selectionModel()->selectedRows();

    if (indexList.length() > 0)
    {
        QString path = ui->tableViewDllFiles->selectionModel()->selectedRows(2).value(0).data().toString();

        selectedDll = findDllInDllFiles(path);

        if (selectedDll.path.length() < 1)
        {
            ui->tableViewDllFiles->selectionModel()->clear();
            ui->statusbar->showMessage("Selected dll could not be found", 8000);
        }

        injectButtonToggle();

        ui->buttonRemoveFile->setEnabled(true);
    }
}

