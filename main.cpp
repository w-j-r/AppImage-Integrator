#include "mainwindow.h"
#include "installer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("appimage_installer_icon.png"));
    a.setApplicationName("AppImage Installer");
    MainWindow w;
    w.show();
    return a.exec();
}
