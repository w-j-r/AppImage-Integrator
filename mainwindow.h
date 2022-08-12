#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    //appimage file filters
    const QString file_filter = "AppImage files (*.appimage *.AppImage *.APPIMAGE) ;; All files (*.*)";

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //getters
    QString get_install_lineedit();
    QString get_uninstall_lineedit();
    QString get_defaultdir_lineedit();
    bool get_remove_app_checkbox();
    QString get_file_filter();

    //setters
    void set_install_dir_lineEdit(const QString *const path);
    void set_remove_dir_lineEdit(const QString *const path);
    void set_default_dir_lineEdit(const QString *const path);
    //save settings
    void save_settings();

    //restore settings
    void get_settings();

    //remove app
    void uninstall_app();

    //msg box



private slots:
    void on_select_open_dir1_clicked();

    void on_select_open_dir2_clicked();

    void on_slct_default_pushButton_clicked();

    void on_install_pushButton_clicked();

    void on_remove_pushButton_clicked();

    void on_save_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
