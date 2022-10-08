#include "qlabel.h"
#include "qboxlayout.h"
#include "qlistview.h"
#include "qscrollarea.h"

#include "HotKeySettingsTab.h"
#include "HotKeySettingsLine.h"


HotKeySettingsTab::HotKeySettingsTab(QWidget *parent)
{
    setParent(parent);
    layout = new QGridLayout(this);
    setLayout(layout);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    QWidget* scrollAreaWidget = new QWidget(scrollArea);
    scrollAreaWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    scrollAreaWidget->setLayout(new QVBoxLayout(scrollAreaWidget));
    scrollArea->setWidget(scrollAreaWidget);

    layout->addWidget(scrollArea);

    QList<HotKeySettingsLine*> lines;

    lines.push_back(new HotKeySettingsLine("Inject", "hotkeys/inject", this));
    lines.push_back(new HotKeySettingsLine("Add File", "hotkeys/addFile", this));
    lines.push_back(new HotKeySettingsLine("Remove File", "hotkeys/removeFile", this));
    lines.push_back(new HotKeySettingsLine("Select Process", "hotkeys/selectProcess", this));

    for (const auto& line : lines)
    {
        connect(line, &HotKeySettingsLine::hotKeyChanged, this, [this] {
            emit hotKeyChanged();
        });
        scrollAreaWidget->layout()->addWidget(line);
    }
}
