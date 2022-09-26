#ifndef INTEGRATOR_H
#define INTEGRATOR_H
#include <QObject>

class Integrator: public QObject {

    Q_OBJECT

private:

    const QString install_directory;
    const QStringList apps_to_install;

    //variables for the current install
    QString current_app_install {}; //the/path/to/the.appimage
    QString directory_only {}; //directory/to/appimage/only/
    QString app_name {};       //nameof.appimage
    QString icon_path {};      //path/to/icon.jpg
    QString desktop_file_path {}; //path/to/appimage.desktop


public:
    //constructor
    Integrator(QString install_directory, QStringList apps_to_install);
    ~Integrator();

    //getters
    QStringList get_apps_to_install();  //list of appimage paths
    QString get_directory_only();       //return the directory to appimage
    QString get_app_name();             //return full app name
    QString get_install_directory();    //appimage destination path
    QString get_icon_path();            //source icon path...later set to destination path
    QString get_desktop_file_path();    //same as above
    QString get_current_app_install();

    //setters
    void set_directory_only(QString p);
    void set_app_name(QString n);
    void set_icon_path(QString *p);
    void set_desktop_file_path(QString *p);
    void set_current_app_install(QString p);

    //doers
    void extract_process(QString abs_path_to_app, QStringList args); //"--appimage-extract" process
    QString check_if_symlink(QStringList items);

    void iterate_app_list();
    void set_appimage_exec_permission(QString directory);
    void retrieve_icon_source_path();
    void retrieve_desktop_file_source_path();
    void make_icon_directory();
    void make_icon_dir_and_move_icon();

    void write_desktop_file();
    void move_desktop_file();
    void move_appimage();
    void clean_up_files();
    void save_paths();

signals:
    //tell mainwindow when we are done.
    void done_signal();
};

#endif // INTEGRATOR_H
