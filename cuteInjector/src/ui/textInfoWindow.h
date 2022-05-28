#ifndef TEXTINFOWINDOW_H
#define TEXTINFOWINDOW_H

#include <QDialog>
#include <QObject>
#include <QBoxLayout>
#include <QTextBrowser>


class TextInfoWindow : public QDialog
{
    Q_OBJECT

public:
    TextInfoWindow(QString windowTitle, QString text);
    ~TextInfoWindow();

    void show();

    QBoxLayout* layout = nullptr;
    QTextBrowser* textBrowser = nullptr;
};

#endif // TEXTINFOWINDOW_H
