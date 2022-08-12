/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *install_tab_1;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QLineEdit *inst_dir_lineEdit;
    QPushButton *select_open_dir1;
    QPushButton *install_pushButton;
    QWidget *remove_tab_1;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *remv_dir_lineEdit;
    QPushButton *select_open_dir2;
    QPushButton *remove_pushButton;
    QWidget *settings_tab3;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *default_dir_lineEdit;
    QPushButton *slct_default_pushButton;
    QCheckBox *delete_app_checkBox;
    QPushButton *save_pushButton;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(597, 239);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        install_tab_1 = new QWidget();
        install_tab_1->setObjectName(QStringLiteral("install_tab_1"));
        verticalLayout = new QVBoxLayout(install_tab_1);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(install_tab_1);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(label, 0, Qt::AlignHCenter);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        inst_dir_lineEdit = new QLineEdit(install_tab_1);
        inst_dir_lineEdit->setObjectName(QStringLiteral("inst_dir_lineEdit"));

        horizontalLayout->addWidget(inst_dir_lineEdit);

        select_open_dir1 = new QPushButton(install_tab_1);
        select_open_dir1->setObjectName(QStringLiteral("select_open_dir1"));

        horizontalLayout->addWidget(select_open_dir1);


        verticalLayout->addLayout(horizontalLayout);

        install_pushButton = new QPushButton(install_tab_1);
        install_pushButton->setObjectName(QStringLiteral("install_pushButton"));

        verticalLayout->addWidget(install_pushButton);

        tabWidget->addTab(install_tab_1, QString());
        remove_tab_1 = new QWidget();
        remove_tab_1->setObjectName(QStringLiteral("remove_tab_1"));
        verticalLayout_3 = new QVBoxLayout(remove_tab_1);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_2 = new QLabel(remove_tab_1);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(label_2, 0, Qt::AlignHCenter);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        remv_dir_lineEdit = new QLineEdit(remove_tab_1);
        remv_dir_lineEdit->setObjectName(QStringLiteral("remv_dir_lineEdit"));

        horizontalLayout_2->addWidget(remv_dir_lineEdit);

        select_open_dir2 = new QPushButton(remove_tab_1);
        select_open_dir2->setObjectName(QStringLiteral("select_open_dir2"));

        horizontalLayout_2->addWidget(select_open_dir2);


        verticalLayout_3->addLayout(horizontalLayout_2);

        remove_pushButton = new QPushButton(remove_tab_1);
        remove_pushButton->setObjectName(QStringLiteral("remove_pushButton"));

        verticalLayout_3->addWidget(remove_pushButton);

        tabWidget->addTab(remove_tab_1, QString());
        settings_tab3 = new QWidget();
        settings_tab3->setObjectName(QStringLiteral("settings_tab3"));
        verticalLayout_4 = new QVBoxLayout(settings_tab3);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_3 = new QLabel(settings_tab3);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_3->addWidget(label_3);

        default_dir_lineEdit = new QLineEdit(settings_tab3);
        default_dir_lineEdit->setObjectName(QStringLiteral("default_dir_lineEdit"));

        horizontalLayout_3->addWidget(default_dir_lineEdit);

        slct_default_pushButton = new QPushButton(settings_tab3);
        slct_default_pushButton->setObjectName(QStringLiteral("slct_default_pushButton"));

        horizontalLayout_3->addWidget(slct_default_pushButton);


        verticalLayout_4->addLayout(horizontalLayout_3);

        delete_app_checkBox = new QCheckBox(settings_tab3);
        delete_app_checkBox->setObjectName(QStringLiteral("delete_app_checkBox"));

        verticalLayout_4->addWidget(delete_app_checkBox);

        save_pushButton = new QPushButton(settings_tab3);
        save_pushButton->setObjectName(QStringLiteral("save_pushButton"));

        verticalLayout_4->addWidget(save_pushButton, 0, Qt::AlignRight);

        tabWidget->addTab(settings_tab3, QString());

        verticalLayout_2->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Select an AppImage to install.", Q_NULLPTR));
        inst_dir_lineEdit->setText(QApplication::translate("MainWindow", "/home/whit/Downloads", Q_NULLPTR));
        select_open_dir1->setText(QApplication::translate("MainWindow", "Select", Q_NULLPTR));
        install_pushButton->setText(QApplication::translate("MainWindow", "Install", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(install_tab_1), QApplication::translate("MainWindow", "Install", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Select an AppImage to remove.", Q_NULLPTR));
        remv_dir_lineEdit->setText(QString());
        select_open_dir2->setText(QApplication::translate("MainWindow", "Select", Q_NULLPTR));
        remove_pushButton->setText(QApplication::translate("MainWindow", "Uninstall AppImge", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(remove_tab_1), QApplication::translate("MainWindow", "Uninstall", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "Default installation directory", Q_NULLPTR));
        slct_default_pushButton->setText(QApplication::translate("MainWindow", "Select", Q_NULLPTR));
        delete_app_checkBox->setText(QApplication::translate("MainWindow", "Delete AppImage after uninstalling.", Q_NULLPTR));
        save_pushButton->setText(QApplication::translate("MainWindow", "Save", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(settings_tab3), QApplication::translate("MainWindow", "Settings", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
