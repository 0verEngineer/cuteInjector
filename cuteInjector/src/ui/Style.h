#pragma once

#include <QApplication>
#include <QStyleFactory>
#include <QPalette>


void ApplyStyle(QApplication* app)
{
    app->setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;

    QColor darkGray(53, 53, 53);
    QColor darkerGray(42,42,42);
    QColor gray(127, 127, 127);
    QColor blue(42, 130, 218);

    darkPalette.setColor(QPalette::Window,darkGray);
    darkPalette.setColor(QPalette::WindowText,Qt::white);
    darkPalette.setColor(QPalette::Base,darkerGray);
    darkPalette.setColor(QPalette::AlternateBase,QColor(66,66,66));
    darkPalette.setColor(QPalette::ToolTipBase,Qt::white);
    darkPalette.setColor(QPalette::ToolTipText,Qt::white);
    darkPalette.setColor(QPalette::Text,Qt::white);
    darkPalette.setColor(QPalette::Dark,QColor(35,35,35));
    darkPalette.setColor(QPalette::Shadow,QColor(20,20,20));
    darkPalette.setColor(QPalette::Button,darkGray);
    darkPalette.setColor(QPalette::ButtonText,Qt::white);
    darkPalette.setColor(QPalette::BrightText,Qt::red);
    darkPalette.setColor(QPalette::Link,blue);
    darkPalette.setColor(QPalette::Highlight,blue);
    darkPalette.setColor(QPalette::HighlightedText,Qt::white);

    darkPalette.setColor(QPalette::Disabled,QPalette::Highlight,QColor(80,80,80));
    darkPalette.setColor(QPalette::Disabled,QPalette::HighlightedText,gray);
    darkPalette.setColor(QPalette::Disabled,QPalette::ButtonText,gray);
    darkPalette.setColor(QPalette::Disabled,QPalette::Text,gray);
    darkPalette.setColor(QPalette::Disabled,QPalette::WindowText,gray);
    darkPalette.setColor(QPalette::Disabled, QPalette::Light, darkGray);

    app->setPalette(darkPalette);
}
