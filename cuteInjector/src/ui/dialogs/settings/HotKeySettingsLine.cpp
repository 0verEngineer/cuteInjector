#include "HotKeySettingsLine.h"
#include "src/Settings.h"

#include "qboxlayout.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "qkeysequenceedit.h"
#include "qtoolbutton.h"
#include "qaction.h"


HotKeySettingsLine::HotKeySettingsLine(const QString &name, const QString &settingsKey, QWidget *parent)
{
    setParent(parent);
    layout = new QHBoxLayout(this);
    setLayout(layout);

    Settings* settings = Settings::instance();

    QLabel* label = new QLabel(this);
    label->setText(name);

    QKeySequenceEdit* edit = new QKeySequenceEdit(this);
    edit->setKeySequence(settings->value(settingsKey).toString());

    QSizePolicy spLabel = QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    spLabel.setHorizontalStretch(1);
    label->setSizePolicy(spLabel);

    QSizePolicy splineEdit = QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    splineEdit.setHorizontalStretch(3);
    edit->setSizePolicy(splineEdit);

    layout->addWidget(label);
    layout->addWidget(edit);

    connect(edit, &QKeySequenceEdit::editingFinished, this, [=] {
        if (settings->value(settingsKey).toString() != edit->keySequence().toString())
        {
            settings->setValue(settingsKey, edit->keySequence().toString());
            emit hotKeyChanged();
        }
    });
}
