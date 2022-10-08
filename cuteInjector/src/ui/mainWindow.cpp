#include "mainWindow.h"
#include "ui_mainWindow.h"
#include "src/data/jsonSerializer.h"
#include "menuBar.h"
#include "src/Settings.h"

#include "qfiledialog.h"
#include "qinputdialog.h"
#include "qtabwidget.h"


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

    createActions();
    setMenuBar(new MenuBar(this));

    processManager = new ProcessManager(nullptr);
    selectProcessDialog = new SelectProcessDialog(processManager, this);

    connect(selectProcessDialog, &SelectProcessDialog::signalProcessSelected, this, &MainWindow::slotProcessSelected);
    connect(processManager, &ProcessManager::processFound, this, &MainWindow::slotProcessFound);
    connect(processManager, &ProcessManager::processNotFound, this, &MainWindow::slotProcessNotFound);
    connect(&injector, &Injector::signalInjectionFinished, this, &MainWindow::slotInjectionFinished);

    // connect ui elements
    connect(ui->buttonInjectFile, &QPushButton::pressed, this, &MainWindow::inject);
    connect(ui->buttonRemoveFile, &QPushButton::pressed, this, &MainWindow::removeFile);
    connect(ui->buttonAddFile, &QPushButton::pressed, this, &MainWindow::addFile);
    connect(ui->buttonSelectProcess, &QPushButton::pressed, this, &MainWindow::selectProcess);
    connect(ui->tableViewDllFiles, &QTableView::clicked, this, &MainWindow::tableViewDllFilesClicked);
    connect(ui->inputProcessName, &QLineEdit::textChanged, this, &MainWindow::inputProcessNameTextChanged);

    selectedDll = DllFile();
    QString processNameToSet = "";

    // Load the data while locked
    selectedProcessMutex.lock();
    selectedProcess = new Process();

    if (JsonSerializer::instance()->loadData(dllFiles, *selectedProcess))
    {
       refreshDllFileTableViewContents();

       if (selectedProcess->name.length() > 0)
       {
           processNameToSet = selectedProcess->name;
       }
    }

    selectedProcessMutex.unlock();

    ui->inputProcessName->setText(processNameToSet);

    processManager->startProcessScanThread(selectedProcess, selectedProcessMutex);
}


MainWindow::~MainWindow()
{
    disconnect(selectProcessDialog, &SelectProcessDialog::signalProcessSelected, this, &MainWindow::slotProcessSelected);
    disconnect(processManager, &ProcessManager::processFound, this, &MainWindow::slotProcessFound);
    disconnect(processManager, &ProcessManager::processNotFound, this, &MainWindow::slotProcessNotFound);
    disconnect(&injector, &Injector::signalInjectionFinished, this, &MainWindow::slotInjectionFinished);

    // Stops the process scan thread
    delete processManager;

    selectedProcessMutex.lock();
    JsonSerializer::instance()->saveData(dllFiles, *selectedProcess);
    selectedProcessMutex.unlock();

    delete selectProcessDialog;
    delete dllFileTableViewModel;

    delete selectedProcess;

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
void MainWindow::updateInjectButtonAndAction()
{
    selectedProcessMutex.lock();
    DWORD id = selectedProcess->id;
    QString path = selectedProcess->path;
    QString arch = selectedProcess->architecture;
    selectedProcessMutex.unlock();

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
    actions.injectAct->setEnabled(enabled);
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
    this->inputProcessNameTextChanged(name);
}


void MainWindow::slotProcessFound(const DWORD procId,
                                  const QString name,
                                  const QString path,
                                  const QString architecture
                                  )
{
    bool hasChanged = false;
    selectedProcessMutex.lock();
    if (selectedProcess->id != procId)
    {
        hasChanged = true;
        selectedProcess->id = procId;
        selectedProcess->name = name;
        selectedProcess->path = path;
        selectedProcess->architecture = architecture;
    }
    selectedProcessMutex.unlock();

    if (hasChanged)
    {
        QPixmap icon = processManager->getCachedIconOfExeFile(path, 64);
        ui->iconProcess->setPixmap(icon);

        ui->inputProcessId->setText(QString::number(procId));
        ui->inputArchitecture->setText(architecture);

        updateInjectButtonAndAction();
    }
}


void MainWindow::slotProcessNotFound()
{
    selectedProcessMutex.lock();
    selectedProcess->id = 0;
    selectedProcess->path = "";
    selectedProcess->architecture = "";
    selectedProcessMutex.unlock();

    ui->inputProcessId->setText("");
    ui->inputArchitecture->setText("");

    updateInjectButtonAndAction();
}


void MainWindow::slotInjectionFinished(bool success, const QString message)
{
    ui->statusbar->showMessage(message, 8000);
}


void MainWindow::inputProcessNameTextChanged(const QString &text)
{
    ui->iconProcess->clear();

    ui->inputProcessId->setText("");
    ui->inputArchitecture->setText("");

    selectedProcessMutex.lock();
    selectedProcess->id = 0;
    selectedProcess->path = "";
    selectedProcess->name = "";
    selectedProcess->architecture = "";

    if (!text.contains("."))
        selectedProcess->searchName = text + ".exe";
    else
        selectedProcess->searchName = text;

    selectedProcessMutex.unlock();
}


void MainWindow::tableViewDllFilesClicked(const QModelIndex &index)
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

        updateInjectButtonAndAction();

        ui->buttonRemoveFile->setEnabled(true);
    }
}

