#include "mainwindow.h"
#include "installer.h"
#include <QProcess>
#include <QFileInfo>
#include <QDir>
#include <QVariant>
#include <QDirIterator>
#include <QTextStream>
#include <QSettings>
#include <QMessageBox>
#include <iostream>


//constructor
Installer::Installer(QString iad, QString ap, QString id)
    : directory_and_app{iad}                                                      //the source directory + app
    , directory_only{QFileInfo(directory_and_app).absoluteDir().absolutePath()}   //source path without app
    , app_name{ap}                                                                //app name only
    , install_directory{id}                                                       //destination path app will go to
    , icon_path{""}                                                               //icon source path, this changes to destintion path
    , desktop_file_path{""} {                                                     //same as icon ^^^.
        //begin
        Installer::extract_app(&directory_and_app);
}

Installer::~Installer(){};

//getters
QString Installer::get_directory_and_app() { return directory_and_app; }
QString Installer::get_directory_only() { return directory_only; }
QString Installer::get_app_name() { return app_name; }
QString Installer::get_install_directory() { return install_directory; }
QString Installer::get_icon_path() { return icon_path; }
QString Installer::get_desktop_file_path() { return desktop_file_path; }

//setters
void Installer::set_icon_path(QString *p) { icon_path = *p; }
void Installer::set_desktop_file_path(QString *p) { desktop_file_path = *p; }

//extract the appimage to get its contents
void Installer::extract_app(const QString *const directory) {

    QProcess process;

    //all the stuff to extract
    const QStringList extract_args = {"usr/share/icons",
                                      "*.desktop",
                                      "*.jpg",
                                      "*.svg",
                                      "*.jpeg",
                                      "*.png"} ;

    //get the path without filename
    QString absolute_path = Installer::get_directory_only();

    // set as working directory so extracted files are there
    process.setWorkingDirectory(absolute_path);

    //set file permission to executable
    process.start("chmod", QStringList() << "+x" << *directory);
    process.waitForFinished();

    for (auto filter: extract_args) {

        process.start(*directory,
                      QStringList() <<
                      "--appimage-extract" <<
                      filter,
                      QIODevice::ReadOnly);
        process.waitForFinished();

        QString StdOut = process.readAllStandardOutput(); //Reads standard output
        std::cout<<std::endl<<StdOut.toStdString();

        QString StdError = process.readAllStandardError(); //Reads standard error
        std::cout<<std::endl<<StdError.toStdString();
    }
    Installer::retrieve_icon();
}


void Installer::retrieve_icon() {

    const QStringList img_filter = {"jpeg", "jpg", "svg", "png"};
    QString icon_name;
    QString path = Installer::get_directory_only() + "/squashfs-root";

    QDir dir(path);
    QFileInfoList directory_list = dir.entryInfoList();

    for (auto file : directory_list) {
        for (int i {}; i < img_filter.size(); ++i) {
            QFileInfo fileinfo = file;
            if (fileinfo.suffix() == img_filter.at(i)) {
                if (fileinfo.isSymLink()) {
                    icon_name = fileinfo.symLinkTarget();
                } else {
                    icon_name = fileinfo.absoluteFilePath();
                }
            }
        }
    }
    if (icon_name.isEmpty()) {
        //qDebug() << "No icon found!";
        std::cout << "No icon for this app found.";
    }
    Installer::set_icon_path(&icon_name); //set the icon path.

    Installer::make_dir_and_move_icon();
}


void Installer::make_dir_and_move_icon() {
    QString icon_to_dir = Installer::get_install_directory() + "/.icons";
    QString icon_name = QFileInfo(Installer::get_icon_path()).fileName();
    QDir dir;
    QString destination = icon_to_dir + "/" + icon_name;
    QString source = Installer::get_icon_path();

    dir.mkdir(icon_to_dir);
    dir.rename(source, destination);

    //set the icons new path
    Installer::set_icon_path(&destination);

    Installer::retrieve_desktop_file();
}


void Installer::retrieve_desktop_file() {
    QString desktop_file;
    QString path = Installer::get_directory_only() + "/squashfs-root";
    QDir dir(path);
    QFileInfoList directory_list = dir.entryInfoList();

    for (auto file:directory_list) {
        QFileInfo fileinfo = file;
        if (fileinfo.suffix() == "desktop") {
            desktop_file = fileinfo.absoluteFilePath();
        }
    }
    //qDebug() << desktop_file;
    Installer::set_desktop_file_path(&desktop_file);
    Installer::write_desktop_file();

}


void Installer::write_desktop_file() {
    QFile file(Installer::get_desktop_file_path());
    QString file_data;
    QStringList file_data_split;

    //if empty(no desktop file found), write it <= not sure how common this is.
    if (Installer::get_desktop_file_path().isEmpty()) {
        //qDebug() << "No desktop file found, creating one..";
        file_data = "[Desktop Entry]\nExec=" +
                    Installer::get_install_directory() +
                    "/" +
                    Installer::get_app_name() +
                    "\nName=" +
                    Installer::get_app_name() +
                    "\nIcon=" +
                    Installer::get_icon_path();
        QFile file(Installer::get_directory_only() + "/squashfs-root/" + Installer::get_app_name() + ".desktop");
        file.open(QFile::ReadWrite);
        file.seek(0);
        QTextStream out(&file);
        out << file_data;
        file.close();
        //qDebug() << file_data;

    } else {
        file.open(QFile::ReadWrite);
        file_data = file.readAll();
        file_data_split = file_data.split('\n'); //split up by newline

        for (auto item: file_data_split) {
            if (item.contains("Exec")) {
                //get the index of item and replace
                file_data_split[file_data_split.indexOf(item)] =
                        "Exec=" +
                        Installer::get_install_directory() +
                        "/" +
                        Installer::get_app_name();
            }
            if (item.contains("Icon")) {
                file_data_split[file_data_split.indexOf(item)] =
                        "Icon=" +
                        Installer::get_icon_path();
            }
        }

        //replace newlines and write back to file
        file_data = file_data_split.join('\n');
        file.seek(0);
        QTextStream out(&file);
        out << file_data;
        file.close();
    }
    Installer::move_desktop_file();
}

void Installer::move_desktop_file() {
    QString desktop_to_dir = QDir::homePath() + "/.local/share/applications";
    QString desktop_file_name = QFileInfo(Installer::get_desktop_file_path()).fileName();
    QDir dir;
    QString destination = desktop_to_dir + "/" + desktop_file_name;
    QString source = Installer::get_desktop_file_path();

    dir.rename(source, destination);

    Installer::set_desktop_file_path(&destination); //to be called later when saving paths.
    Installer::move_appimage();
}

void Installer::move_appimage() {
    QDir dir;
    QString destination = Installer::get_install_directory() + "/" + Installer::get_app_name();
    QString source = Installer::get_directory_and_app();

    /*check that install directory exists, especially if they use the default since "Applications"
     is not on all systems.*/
    if (!QDir(Installer::get_install_directory()).exists()) {
        QDir *newdir = new QDir;
        newdir->mkdir(Installer::get_install_directory());
        delete newdir;
    }

    dir.rename(source, destination);

    Installer::clean_up_files();
}

//delete leftover squashfs
void Installer::clean_up_files() {
    QDir dir(Installer::get_directory_only() + "/squashfs-root");

    //make sure files have been moved before removing.
    while (QDir(Installer::get_directory_and_app()).exists()) {
        //qDebug() << "Waiting for files to move before removing squashfs..";
        continue;
    }
    dir.removeRecursively();

    Installer::save_paths();
}

//save paths to icon, appimage, and desktop file for easy removal
void Installer::save_paths() {
    QSettings paths(QString(QDir::homePath() + "/.config/appimage-installer/installed_list"), QSettings::NativeFormat);
    QString appimage = Installer::get_app_name();

    paths.beginGroup(appimage);
    paths.setValue("icon", QVariant(Installer::get_icon_path()));
    paths.setValue("desktop_file", QVariant(Installer::get_desktop_file_path()));
    paths.setValue("app", QVariant(Installer::get_install_directory() + "/" + appimage));
    paths.endGroup();

    //qDebug() << "paths saved.";

}

