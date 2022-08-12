#ifndef INSTALLER_H
#define INSTALLER_H
#include "mainwindow.h"

class Installer {
private:

    const QString directory_and_app;
    const QString directory_only;
    const QString app_name;
    const QString install_directory;

    QString icon_path;
    QString desktop_file_path;

public:
    //constructor
    Installer(QString iad, QString ap, QString id);
    ~Installer();

    //getters
    QString get_directory_and_app();
    QString get_directory_only();
    QString get_app_name();
    QString get_install_directory();
    QString get_icon_path();
    QString get_desktop_file_path();

    //setters
    void set_icon_path(QString *p);
    void set_desktop_file_path(QString *p);

    //doers
    void extract_app(const QString *const directory);
    void retrieve_icon();
    void make_icon_directory();
    void make_dir_and_move_icon();
    void retrieve_desktop_file();
    void write_desktop_file();
    void move_desktop_file();
    void move_appimage();
    void clean_up_files();
    void save_paths();
};

#endif // INSTALLER_H
