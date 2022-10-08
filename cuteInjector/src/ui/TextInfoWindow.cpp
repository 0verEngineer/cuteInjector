#include "TextInfoWindow.h"


TextInfoWindow::TextInfoWindow(QString windowTitle, QString text)
{
    this->setWindowTitle(windowTitle);

    this->layout = new QBoxLayout(QBoxLayout::Direction::LeftToRight, this);

    this->textBrowser = new QTextBrowser(this);

    this->layout->addWidget(textBrowser);

    textBrowser->setText(text);
}


TextInfoWindow::~TextInfoWindow()
{
    delete textBrowser;
    delete layout;
}


void TextInfoWindow::show()
{
    this->exec();
}
