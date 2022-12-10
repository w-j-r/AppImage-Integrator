#include "mainwindow.h"
#include "integrator.h"
#include <QProcess>
#include <QFileInfo>
#include <QDir>
#include <QVariant>
#include <QTextStream>
#include <QSettings>
#include <iostream>
#include <QDebug>

//constructor
Integrator::Integrator(QString install_directory, QStringList apps_to_install)
    : install_directory{install_directory}
    , apps_to_install{apps_to_install}
{}

Integrator::~Integrator(){}

//getters
QStringList Integrator::get_apps_to_install() { return apps_to_install; }
QString Integrator::get_directory_only() { return directory_only; }
QString Integrator::get_app_name() { return app_name; }
QString Integrator::get_install_directory() { return install_directory; }
QString Integrator::get_icon_path() { return icon_path; }
QString Integrator::get_desktop_file_path() { return desktop_file_path; }
QString Integrator::get_current_app_install() { return current_app_install; }


//setters
void Integrator::set_directory_only(QString p) { directory_only = p; }
void Integrator::set_app_name(QString n) { app_name = n; }
void Integrator::set_icon_path(QString *p) { icon_path = *p; }
void Integrator::set_desktop_file_path(QString *p) { desktop_file_path = *p; }
void Integrator::set_current_app_install(QString p) { current_app_install = p; }

// Used to extract items from the appimage
void Integrator::extract_process(QString abs_path_to_app, QStringList args) {

    QProcess process;
    process.setWorkingDirectory(Integrator::get_directory_only());
    for (auto item: args) {
        process.waitForStarted();
        process.start(abs_path_to_app,
                      QStringList() <<
                      "--appimage-extract" <<
                      item,
                      QIODevice::ReadOnly);
        std::cout << process.readAllStandardOutput().toStdString() << std::endl;
        std::cout << process.readAllStandardError().toStdString() << std::endl;
        process.waitForFinished(-1);
    }
}

/* "--appimage-extract *.filetype" does not follow symlinks issue at github https://github.com/AppImage/AppImageKit/issues/1106,
 * So we have to extract it, check if it is a symlink and if it is, extract the link target */
// Figures out if item extracted from appimage is a symlink, returns the path to item in appimage.
QString Integrator::check_if_symlink(QStringList items) {
    const QStringList filter = items;
    QString item_name;
    QString path_to_squashfs = this->get_directory_only() + "/squashfs-root";
    QDir dir(path_to_squashfs);
    dir.setFilter(QDir::AllEntries | QDir::System);  // has to be set or else broken links will not be found in entryInfoList()
    QFileInfoList directory_list = dir.entryInfoList();

    qDebug() << directory_list;
    for (auto file : directory_list) {
        for (int i {}; i < filter.size(); ++i) {
            QFileInfo fileinfo = file;
            if (fileinfo.suffix() == filter.at(i)) {
                if (fileinfo.isSymLink()) {
                    item_name = fileinfo.symLinkTarget();
                    item_name.remove(path_to_squashfs + "/"); // we only want the part that starts with 'usr/'. To be used as an extraction arg.
                    this->extract_process(this->get_current_app_install(), QStringList(item_name));
                    item_name.prepend(path_to_squashfs + "/"); // add it back.
                    std::cout << item_name.toStdString() << " is a symlink " << std::endl;
                } else {
                    item_name = fileinfo.absoluteFilePath();
                    std::cout << item_name.toStdString() << "is not a symlink " << std::endl;
                }
            }
        }
    }
    return item_name;
}

//iterate the list and integrate one by one.
void Integrator::iterate_app_list() {

    for (QString path : this->get_apps_to_install()) {
        this->set_directory_only(QFileInfo(path).absoluteDir().absolutePath());
        this->set_app_name(QFileInfo(path).fileName());
        std::cout << "Integrating " << path.toStdString() << std::endl;
        this->set_current_app_install(path);
        //begin
        this->set_appimage_exec_permission(path);
    }
    //tell mainwindow we are done
    emit done_signal();
}


//Set the appimage executable permission to true
void Integrator::set_appimage_exec_permission(QString directory) {

    QProcess process;
    process.start("chmod", QStringList() << "+x" << directory);
    process.waitForFinished();

    //extract icons
    this->retrieve_icon_source_path();
}


// Extract the icon, check for symlink.
/* Some apps throw a "fopen" error if trying to extract an icon directly in /usr/share/icons/.
   not sure what to do about it, either mount or unpack the whole appimage.*/

void Integrator::retrieve_icon_source_path() {

    this->extract_process(this->get_current_app_install(), {"*.svg", "*.jpg", "*.png", "*.jpeg"});
    //get the icon path in squashfs-root
    QString icon_path = this->check_if_symlink({"jpeg", "jpg", "svg", "png"});

    if (icon_path.isEmpty()) {
        std::cout << "No icon for this app found." << std::endl;
    }
    this->set_icon_path(&icon_path); //set the icon path.

    this->make_icon_dir_and_move_icon();
}

// Make the path to destination icon directory then move the icon there.
void Integrator::make_icon_dir_and_move_icon() {

    QString icon_to_dir = QDir::homePath() + "/.local/share/icons/appimage-icons"; //should it be another directory?
    QString icon_name = QFileInfo(this->get_icon_path()).fileName();
    QDir dir;
    QString destination = icon_to_dir + "/" + icon_name;
    QString source = this->get_icon_path();

    dir.setFilter(QDir::AllEntries | QDir::System);
    dir.mkdir(icon_to_dir);

    dir.rename(source, destination);
    qDebug() << "destination: " << destination << "source: " << source;

    //set the icons new path
    this->set_icon_path(&destination);

    this->retrieve_desktop_file_source_path();
}

//extract *.desktop file, check if symlink, rewrite contents
void Integrator::retrieve_desktop_file_source_path() {
    this->extract_process(this->get_current_app_install(), QStringList("*.desktop"));

    QString desktop_file = this->check_if_symlink(QStringList("desktop"));

    this->set_desktop_file_path(&desktop_file);
    this->write_desktop_file();

}

// Write the destination paths into the desktop file, if one is not found..make one <- not tested.
void Integrator::write_desktop_file() {

    QFile file(this->get_desktop_file_path());
    QString file_data;
    QStringList file_data_split;

    //if empty(no desktop file found), write it <= not sure how common this is.
    // this part has not been tested to see if it actually works.
    if (this->get_desktop_file_path().isEmpty()) {
        std::cout << "No desktop file found, creating one.." << std::endl;
        file_data = "[Desktop Entry]\nExec=" +
                    this->get_install_directory() +
                    "/" +
                    this->get_app_name() +
                    "\nName=" +
                    this->get_app_name() +
                    "\nIcon=" +
                    this->get_icon_path();

        //Note: not sure if it works
        QString new_desktop_file = this->get_directory_only() + "/squashfs-root/" + this->get_app_name() + ".desktop";
        QFile file(new_desktop_file);
        file.open(QFile::ReadWrite);
        file.seek(0);
        QTextStream out(&file);
        out << file_data;
        file.close();

    } else { //write our destination paths
        file.open(QFile::ReadWrite);
        file_data = file.readAll();
        file_data_split = file_data.split('\n'); //split up by newline

        for (auto item: file_data_split) {
            if (item.contains("Exec")) {
                //get the index of item and replace
                file_data_split[file_data_split.indexOf(item)] =
                        "Exec=" +
                        this->get_install_directory() +
                        "/" +
                        this->get_app_name();
            }
            if (item.contains("Icon")) {
                file_data_split[file_data_split.indexOf(item)] =
                        "Icon=" +
                        this->get_icon_path();
            }
        }

        //replace newlines and write back to file
        file_data = file_data_split.join('\n');
        file.seek(0);
        QTextStream out(&file);
        out << file_data;
        file.close();
    }
    this->move_desktop_file();
}

void Integrator::move_desktop_file() {

    QString desktop_to_dir = QDir::homePath() + "/.local/share/applications";
    QString desktop_file_name = QFileInfo(this->get_desktop_file_path()).fileName();
    QDir dir;
    QString destination = desktop_to_dir + "/" + desktop_file_name;
    QString source = this->get_desktop_file_path();

    dir.rename(source, destination); //move file

    this->set_desktop_file_path(&destination); //to be called later when saving paths to apps_list.
    this->move_appimage();
}

void Integrator::move_appimage() {

    QDir dir;
    QString destination = this->get_install_directory() + "/" + this->get_app_name();
    QString source = this->get_current_app_install();

    //make sure source and destination arent the same else we are caught in a loop
    if (destination != source) {
        dir.rename(source, destination);

        //check that the file copy is complete. Note: Might be a better way to handle this.
        bool is_done {false};
        while (!is_done) {
            if (QFileInfo(destination).exists() && !QFileInfo(source).exists()) {
                is_done = true;
            } else {
                std::cout << "Waiting for appimage move to complete." << std::endl;
                continue;
            }
        }
    }
    std::cout << "Appimage copy complete" << std::endl;
    this->clean_up_files();
}

//delete leftover squashfs before extracting new appimage
void Integrator::clean_up_files() {

    QDir dir(this->get_directory_only() + "/squashfs-root");
    dir.removeRecursively();

    this->save_paths();
}

//save paths to icon, appimage, and desktop file to apps_list for easy removal
void Integrator::save_paths() {

    QSettings paths(QString(QDir::homePath() + "/.config/appimage-integrator/apps_list"), QSettings::NativeFormat);
    QString appimage = this->get_app_name();

    paths.beginGroup(appimage);
    paths.setValue("icon", QVariant(this->get_icon_path()));
    paths.setValue("desktop_file", QVariant(this->get_desktop_file_path()));
    paths.setValue("app", QVariant(this->get_install_directory() + "/" + appimage));
    paths.endGroup();
}

//======SIGNALS=======//
void done_signal() { // tell 'em were done
    std::cout << "Done." << std::endl;
}




