#ifndef SELECTPROCESSDIALOG_H
#define SELECTPROCESSDIALOG_H

#include "src/utils/process/Process.h"
#include "src/utils/process/ProcessManager.h"

#include <QDialog>
#include <QObject>
#include <QTableWidget>
#include <QBoxLayout>


class SelectProcessDialog : public QDialog
{
    Q_OBJECT

public:
    SelectProcessDialog(ProcessManager* processManager, QWidget* parent = nullptr);
    ~SelectProcessDialog();

    void showWindow();

    QTableWidget* table = nullptr;
    QBoxLayout* windowLayout = nullptr;

    ProcessManager* processManager = nullptr;
    std::vector<std::unique_ptr<Process>>* processList = nullptr;

    int columnCount = 3;
    int scrollBarWidth = 0;


private slots:
    void slotCellDoubleClicked(int row, int col);


signals:
    void signalProcessSelected(const QString path);
};

#endif // SELECTPROCESSDIALOG_H
