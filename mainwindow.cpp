#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "integrator.h"
#include <iostream>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QString>
#include <QFileDialog>
#include <QStandardPaths>
#include <QListWidgetItem>
#include <QDropEvent>
#include <QFileInfo>
#include <QList>
#include <QSettings>
#include <QMessageBox>
#include <QDir>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    this->setWindowTitle("AppImage Integrator");
    setAcceptDrops(true);
    get_settings();
}

MainWindow::~MainWindow()
{
    save_settings();
    delete ui;
}

//setter
void MainWindow::set_app_list(QString path) { app_list.append(path); }
void MainWindow::set_remove_apps_list(QString path) { remove_apps_list.append(path); }
void MainWindow::set_remove_tab_clicked(bool b) { remove_tab_clicked = b; }

//removers and clearers
void MainWindow::remove_path_from_list(QString path) { app_list.removeAll(path); }
void MainWindow::clear_app_list() { app_list.clear(); }

//getter
QStringList MainWindow::get_app_list() { return app_list; }
QStringList MainWindow::get_remove_apps_list() { return remove_apps_list; }
bool MainWindow::get_remove_tab_clicked() { return remove_tab_clicked; }

//drag and drop events
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        foreach (QUrl url, event->mimeData()->urls()) {
            if (url.path().endsWith(".appimage", Qt::CaseInsensitive)) {
                ui->add_list_widget->addItem(url.path());
                MainWindow::set_app_list(url.path());
            }
        }
    }
}

//settings
void MainWindow::save_settings() {

    QSettings settings(QString(QDir::homePath() + "/.config/appimage-integrator/settings.conf"), QSettings::NativeFormat);

    settings.beginGroup("ui_settings");
    settings.setValue("start", "true");
    settings.setValue("dark_mode", QVariant(ui->dark_mode_enable->isChecked()).toBool());
    settings.setValue("default_directory", QVariant(ui->default_dir_line_edit->text()));
    settings.setValue("geometry", saveGeometry());
    settings.setValue("window_state", saveState());
    settings.endGroup();

    std::cout << "Settings saved." << std::endl;
}

void MainWindow::get_settings() {

    QSettings settings(QString(QDir::homePath() + "/.config/appimage-integrator/settings.conf"), QSettings::NativeFormat);

    settings.beginGroup("ui_settings");

    //if this no settings file, set initial defaults
    if (!settings.contains("start")) {
        ui->default_dir_line_edit->setText(QDir::homePath() + "/Applications");
        ui->delete_app_check_box->setChecked(false);
        MainWindow::save_settings();

    // else restore saved settings.
    } else {
        ui->default_dir_line_edit->setText(QVariant(settings.value("default_directory")).toString());
        restoreGeometry(settings.value("geometry").toByteArray());
        ui->dark_mode_enable->setChecked(QVariant(settings.value("dark_mode")).toBool());
        restoreState(settings.value("windowState").toByteArray());
        settings.endGroup();
    }
    std::cout << "Settings loaded." << std::endl;
}

//QListWidgetitems in remove tab
void MainWindow::remove_list_widget_items() {

    //get added apps from file to populate remove list widget
    QString settings_path = QDir::homePath() + "/.config/appimage-integrator/apps_list";
    if (QFileInfo(settings_path).exists()) {

         QSettings paths(QString(settings_path), QSettings::NativeFormat);
         QStringList keys = paths.childGroups();

          //add keys to qlistwidget and make checkable
         for(int i=0; i < keys.count(); ++i){
            MainWindow::set_remove_apps_list(keys.at(i));
            QListWidgetItem* remove_items = new QListWidgetItem;
            remove_items->setText(keys.at(i));
            remove_items->setFlags(remove_items->flags() | Qt::ItemIsUserCheckable);
            remove_items->setCheckState(Qt::Unchecked);
            if (ui->dark_mode_enable->isChecked()) { //maybe find out how to properly theme the checkbox
                remove_items->setBackground(QColor("#3C3C3C"));
            }
            ui->remove_list_widget->addItem(remove_items);
        }

    } else {
        std::cout << "No apps_list file found." << std::endl;
    }
}

void MainWindow::remove_apps(QStringList *apps_to_remove, QList<QListWidgetItem*> *items_to_delete) {

    for (QString x : *apps_to_remove) {
        QString app_name = x;
        QString app_to_remove_path;
        QString desktop_file_path;
        QString icon_path;

        QSettings paths(QString(QDir::homePath() + "/.config/appimage-integrator/apps_list"), QSettings::NativeFormat);

        paths.beginGroup(app_name);
        desktop_file_path = QVariant(paths.value("desktop_file")).toString();
        icon_path = QVariant(paths.value("icon")).toString();
        app_to_remove_path = QVariant(paths.value("app")).toString();
        paths.endGroup();

        QDir icon, file, app;

        icon.remove(icon_path);
        file.remove(desktop_file_path);

        if (ui->delete_app_check_box->isChecked()) {
            app.remove(app_to_remove_path);
        }

        //remove entry from apps_list file
        paths.remove(app_name);
    }

    //remove the entries from remove_list_widget
    foreach(QListWidgetItem * item, *items_to_delete) {
        delete ui->remove_list_widget->takeItem(ui->remove_list_widget->row(item));
    }
}


//==ADD TAB SLOTS===//
void MainWindow::on_add_button_clicked()
{
    QStringList select_appimage = QFileDialog::getOpenFileNames (
                this,
                "Select an AppImage",
                QStandardPaths::writableLocation(QStandardPaths::DownloadLocation),
                "AppImage files (*.appimage *.AppImage *.APPIMAGE) ;; All files (*.*)"
    );
    //send to listwidget
    for (QString item: select_appimage) {
        ui->add_list_widget->addItem(item);
        MainWindow::set_app_list(item);
    }
}

//remove selected items from qlistwidget
void MainWindow::on_remove_botton_clicked()
{
    //delete the entry from our list
    const QString item_to_remove = ui->add_list_widget->currentItem()->text();
    if (MainWindow::get_app_list().contains(item_to_remove)) {
        MainWindow::remove_path_from_list(item_to_remove);
    }
    // and delete entry from the qlistwidget
    qDeleteAll(ui->add_list_widget->selectedItems());
}


//initiate integration.
void MainWindow::on_integrate_button_clicked()
{
    if (!ui->default_dir_line_edit->text().isEmpty() &&
        QFileInfo(ui->default_dir_line_edit->text()).exists() &&
        !MainWindow::get_app_list().isEmpty()) {

        //create Integrator object, connect the done signal, begin the process
        integrator = new Integrator(ui->default_dir_line_edit->text(), MainWindow::get_app_list());
        QObject::connect(integrator, SIGNAL(done_signal()), this, SLOT(done_slot()), Qt::DirectConnection);

        integrator->iterate_app_list();

    } else {
      QMessageBox::about(this, "Error", "Missing a valid app directory or no apps were added.");
    }
}

//==remove TAB SLOTS==//
void MainWindow::on_remove_push_button_clicked()
{
    QStringList apps_to_remove;  //the paths to apps to be removed.
    QList<QListWidgetItem*> items_to_delete;  //also the items to be deleted from remove_list_qwidget

    for (int i {}; i < ui->remove_list_widget->count(); ++i) {
        if (ui->remove_list_widget->item(i)->checkState() == Qt::Checked) {
            apps_to_remove.append(MainWindow::get_remove_apps_list().at(i));
            items_to_delete.push_back(ui->remove_list_widget->item(i));
        }
    }
    MainWindow::remove_apps(&apps_to_remove, &items_to_delete);
}


void MainWindow::on_change_dir_push_button_clicked()
{
    //Get a directory and set the default dir qline text. Dont set text if no directory selected.
    if (!QFileDialog::getExistingDirectory(this).isEmpty()) {
        ui->default_dir_line_edit->setText(QFileDialog::getExistingDirectory(this));
    }
}


void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index == 1 && !MainWindow::get_remove_tab_clicked()) {
        MainWindow::remove_list_widget_items();
        MainWindow::set_remove_tab_clicked(true);
    }
}

void MainWindow::on_select_all_stateChanged(int arg1)
{
    //if 2, then is checked
    for (int i {}; i < ui->remove_list_widget->count(); ++i) {
        if (arg1 == 2) {
            ui->remove_list_widget->item(i)->setCheckState(Qt::Checked);
        } else {
            ui->remove_list_widget->item(i)->setCheckState(Qt::Unchecked);
        }
    }
}

//If user has the delete apps checkbox checked, turn it red. (hopefully to prevent accidents)
void MainWindow::on_delete_app_check_box_stateChanged(int arg1)
{
    if (arg1 == 2) {
        ui->delete_app_check_box->setStyleSheet("QCheckBox { color: red }");
    } else {
        ui->delete_app_check_box->setStyleSheet("QCheckBox { color: white }");
    }
}

//signal slots
//clean up after Integrator sends the done signal.
void MainWindow::done_slot()
{
    ui->add_list_widget->clear();                //clear qlistwidgetitems from add tab
    MainWindow::clear_app_list();                //reset the list
    if (MainWindow::get_remove_tab_clicked()) {  //if the remove tab has already been clicked
        ui->remove_list_widget->clear();         //clear the list items
        MainWindow::remove_list_widget_items();  //update the remove list widget
    }
    delete integrator;
}
