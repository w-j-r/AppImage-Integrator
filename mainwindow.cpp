#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "installer.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QCheckBox>
#include <QSettings>
#include <QStandardPaths>
#include <QProcess>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //restore settings if they exist or set defaults
    get_settings();
}

MainWindow::~MainWindow()
{
    MainWindow::save_settings();
    delete ui;
}

//get ui elements
QString MainWindow::get_install_lineedit() { return ui->inst_dir_lineEdit->text(); }
QString MainWindow::get_uninstall_lineedit() { return ui->remv_dir_lineEdit->text(); }
QString MainWindow::get_defaultdir_lineedit() { return ui->default_dir_lineEdit->text(); }
bool MainWindow::get_remove_app_checkbox() { return ui->delete_app_checkBox->isChecked(); }
//other
QString MainWindow::get_file_filter() { return file_filter; }

//setters
void MainWindow::set_install_dir_lineEdit(const QString *const path) { ui->inst_dir_lineEdit->setText(*path); }
void MainWindow::set_remove_dir_lineEdit(const QString *const path) { ui->remv_dir_lineEdit->setText(*path); }
void MainWindow::set_default_dir_lineEdit(const QString *const path) { ui->default_dir_lineEdit->setText(*path); }

//open the directory to the appimage to be installed
void MainWindow::on_select_open_dir1_clicked()
{
    QString select_appimage = QFileDialog::getOpenFileName (
                this,
                "Select an AppImage",
                QStandardPaths::writableLocation(QStandardPaths::DownloadLocation),
                //QDir::homePath() + "/Downloads",
                MainWindow::get_file_filter()
    );

    //send selected directory to qlineedit
    MainWindow::set_install_dir_lineEdit(&select_appimage);

}

//open the directory to the appimage to be removed.
void MainWindow::on_select_open_dir2_clicked()
{
    QString select_appimage = QFileDialog::getOpenFileName (
                this,
                "Select an AppImage",
                QDir::homePath() + "/Applications",
                MainWindow::get_file_filter()
    );

    //send selected directory to qlineedit
    //ui->remv_dir_lineEdit->setText(select_appimage);
    MainWindow::set_remove_dir_lineEdit(&select_appimage);
}

//the select button to set the default directory.
void MainWindow::on_slct_default_pushButton_clicked()
{
    QString default_dir = QFileDialog::getExistingDirectory(this);

    //send the text to the qline
    //ui->default_dir_lineEdit->setText(default_dir);
    MainWindow::set_default_dir_lineEdit(&default_dir);
}

//verify directories/file and begin the install
//send info to installer.
void MainWindow::on_install_pushButton_clicked()
{
    if (QFileInfo(MainWindow::get_install_lineedit()).dir().exists() &&
        QDir(MainWindow::get_defaultdir_lineedit()).exists() &&
        QFileInfo(MainWindow::get_install_lineedit()).isFile()) {

        Installer(MainWindow::get_install_lineedit(),
                  QFileInfo(MainWindow::get_install_lineedit()).fileName(),
                  MainWindow::get_defaultdir_lineedit());
    } else { //show error
        QMessageBox::about(this, "Error", "Invalid file or directories");
    }
}

//begin the uninstall
void MainWindow::on_remove_pushButton_clicked()
{
    MainWindow::uninstall_app();
}


//save user settings on button click
void MainWindow::on_save_pushButton_clicked()
{
    MainWindow::save_settings();
}

//save settings function
void MainWindow::save_settings() {

    QSettings settings(QString(QDir::homePath() + "/.config/appimage-installer/settings.conf"), QSettings::NativeFormat);

    settings.beginGroup("ui_settings");
    settings.setValue("start", "true");
    settings.setValue("install_directory", QVariant(MainWindow::get_install_lineedit()));
    settings.setValue("default_directory", QVariant(MainWindow::get_defaultdir_lineedit()));
    settings.setValue("delete_app_check", ui->delete_app_checkBox->isChecked());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("window_state", saveState());
    settings.endGroup();

    //qDebug() << "Settings saved.";
    std::cout << "Settings saved." << std::endl;
}

void MainWindow::get_settings() {

    QSettings settings(QString(QDir::homePath() + "/.config/appimage-installer/settings.conf"), QSettings::NativeFormat);

    settings.beginGroup("ui_settings");

    //if this no settings file, set initial defaults
    if (!settings.contains("start")) {
        ui->default_dir_lineEdit->setText(QDir::homePath() + "/Applications");
        ui->remv_dir_lineEdit->setText(QDir::homePath() + "/Applications");
        ui->inst_dir_lineEdit->setText(QDir::homePath() + "/Downloads");
        restoreGeometry(settings.value("myWidget/geometry").toByteArray());
        restoreState(settings.value("myWidget/windowState").toByteArray());
        MainWindow::save_settings();

    // else restore saved settings.
    } else {
        ui->inst_dir_lineEdit->setText(QVariant(settings.value("install_directory")).toString());
        ui->remv_dir_lineEdit->setText(QVariant(settings.value("default_directory")).toString());
        ui->default_dir_lineEdit->setText(QVariant(settings.value("default_directory")).toString());
        ui->delete_app_checkBox->setChecked(QVariant(settings.value("delete_app_check")).toBool());
        settings.endGroup();
    }
    std::cout << "Settings loaded." << std::endl;
}

void MainWindow::uninstall_app() {
    QString app_to_remove_path = ui->remv_dir_lineEdit->text();
    QString app_name = QFileInfo(app_to_remove_path).fileName();
    QString desktop_file_path;
    QString icon_path;

    QSettings paths(QString(QDir::homePath() + "/.config/appimage-installer/installed_list"), QSettings::NativeFormat);

    paths.beginGroup(app_name);
    desktop_file_path = QVariant(paths.value("desktop_file")).toString();
    icon_path = QVariant(paths.value("icon")).toString();
    paths.endGroup();

    //qDebug() << icon_path;

    QDir icon, file, app;

    icon.remove(icon_path);
    file.remove(desktop_file_path);

    if (MainWindow::get_remove_app_checkbox()) {
        app.remove(app_to_remove_path);
    }

    //remove entry from installed_list file
    paths.remove(app_name);
}


