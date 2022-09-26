#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "integrator.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    //variables
    QStringList app_list;                    // List of apps to be added, added by user
    QStringList remove_apps_list;            // ^^^ ^^ ^^^ ^^ ^^ removeed, ^^ ^^ ^^
    QList<QListWidgetItem*> remove_widgets;  // List of paths to be added/deleted from QListWidget.
    Integrator* integrator = nullptr;        // Integrator class pointer.
    bool remove_tab_clicked {false};         // If true, then the remove tab was clicked.

public:

    //setters
    void set_app_list(QString path);
    void set_remove_apps_list(QString path);
    void set_remove_tab_clicked(bool b);

    //removers and clearers
    void remove_path_from_list(QString path);
    void clear_app_list();

    //getters
    QStringList get_app_list();
    QStringList get_remove_apps_list();
    bool get_remove_tab_clicked();

    //drag & drop events
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    //settings
    void save_settings();
    void get_settings();

    //Deals with QListWidgetItems in remove tab.
    void remove_list_widget_items();          //Get paths from Qsettings file, add them to QListWidget on the remove tab.
    void remove_apps(QStringList *apps_to_remove, QList<QListWidgetItem*> *items_to_delete);

private slots:
    //add tab
    void on_add_button_clicked();
    void on_remove_botton_clicked();
    void on_integrate_button_clicked();
    void on_remove_push_button_clicked();

    //remove tab
    void on_tabWidget_currentChanged(int index);         //load widgets if 'remove' tab is clicked.
    void on_select_all_stateChanged(int arg1);           //Check all checkboxes in remove_list_widget if checked.
    void on_delete_app_check_box_stateChanged(int arg1); //Change the color of delete_app_check_box to red if checked.

    //Settings tab
    void on_change_dir_push_button_clicked();

public slots:
    void done_slot();  // Receives signal from Integrator class to let MainWindow know that it is done.
};
#endif // MAINWINDOW_H
