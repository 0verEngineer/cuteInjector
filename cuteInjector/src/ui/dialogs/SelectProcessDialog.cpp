#include "SelectProcessDialog.h"
#include "src/ui/WindowsTheme.h"
#include "src/definitions/Themes.h"

#include <QHeaderView>
#include <QMouseEvent>
#include <QScrollBar>
#include <QLabel>


SelectProcessDialog::SelectProcessDialog(ProcessManager* processManager, QWidget* parent)
{
    if (WindowsTheme::getWindowsSystemTheme() == SYSTEMTHEMES.DARK)
        WindowsTheme::setDarkTitleBarWindows(reinterpret_cast<HWND>(winId()));

    this->setWindowTitle("Select Process");

    this->processManager = processManager;

    this->table = new QTableWidget();
    this->windowLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom, this);

    this->table->setColumnCount(columnCount);
    this->table->setParent(this);

    this->table->horizontalHeader()->hide();
    this->table->verticalHeader()->hide();
    this->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->windowLayout->addWidget(table);

    // set the border to 0px
    windowLayout->setContentsMargins(0, 0, 0, 0);

    // in windows the scrollBarWidth is 3px wider
    scrollBarWidth = style()->pixelMetric(QStyle::PM_ScrollBarExtent) + 3;

    connect(table, &QTableWidget::cellDoubleClicked, this, &SelectProcessDialog::slotCellDoubleClicked);
    connect(table, &QTableWidget::cellActivated, this, &SelectProcessDialog::slotCellDoubleClicked);
}


SelectProcessDialog::~SelectProcessDialog()
{
    disconnect(table, &QTableWidget::cellDoubleClicked, this, &SelectProcessDialog::slotCellDoubleClicked);
    disconnect(table, &QTableWidget::cellActivated, this, &SelectProcessDialog::slotCellDoubleClicked);

    delete table;
    delete windowLayout;
}


void SelectProcessDialog::showWindow()
{
    if (processList != nullptr)
        processList->clear();
    processList = processManager->getUpdatedProcessList();

    table->clear();
    table->setRowCount(processList->size());

    std::vector<QTableWidgetItem*> items;
    std::vector<QLabel*> labels;

    for (int i = 0; i < processList->size(); i++)
    {
        QLabel* iconLabel = new QLabel();
        iconLabel->setPixmap(processList->at(i)->icon);
        iconLabel->setAlignment(Qt::AlignCenter);
        table->setCellWidget(i, 0, iconLabel);
        labels.push_back(iconLabel);

        QTableWidgetItem* itemProcessId = new QTableWidgetItem();
        QString procId = QString::number(processList->at(i)->id, 16);
        itemProcessId->setText(procId);
        table->setItem(i, 1, itemProcessId);
        items.push_back(itemProcessId);

        QTableWidgetItem* itemName = new QTableWidgetItem();
        itemName->setText(processList->at(i)->name);
        table->setItem(i, 2, itemName);
        items.push_back(itemName);
    }

    table->resizeColumnsToContents();
    table->setCurrentCell(0, 2);

    // The Dialog does not use the table size
    int width = 0;
    for (int i = 0; i < columnCount; i++)
        width += table->columnWidth(i);
    this->setMinimumWidth(width + scrollBarWidth);

    this->exec();

    for (auto item : items)
        delete item;

    for (auto label : labels)
        delete label;
}


void SelectProcessDialog::slotCellDoubleClicked(int row, int col)
{
    emit signalProcessSelected(processList->at(row)->name);
    this->close();
}
