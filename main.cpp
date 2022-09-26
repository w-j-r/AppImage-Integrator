#include "mainwindow.h"
#include <QApplication>
#include <QTextStream>
#include <QPalette>
#include <QSettings>
#include <QDir>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QSettings* settings = new QSettings(QString(QDir::homePath() + "/.config/appimage-integrator/settings.conf"), QSettings::NativeFormat);
    settings->beginGroup("ui_settings");
    if (settings->value("dark_mode") == "true") {

        QPalette palette;
        palette.setColor(QPalette::Window, QColor(53,53,53));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, QColor(40,40,40));
        palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
        palette.setColor(QPalette::ToolTipBase, Qt::white);
        palette.setColor(QPalette::ToolTipText, Qt::white);
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Button, QColor(53,53,53));
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::Highlight, QColor(31,53,56).lighter());
        palette.setColor(QPalette::HighlightedText, Qt::white);
        palette.setColor(QPalette::ToolTipBase, Qt::black);
        palette.setColor(QPalette::ToolTipText, Qt::white);
        a.setPalette(palette);
    }
    delete settings;

    a.setApplicationName("AppImage Integrator");
    MainWindow w;
    w.show();
    return a.exec();
}
