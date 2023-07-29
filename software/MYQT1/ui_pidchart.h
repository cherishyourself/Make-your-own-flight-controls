/********************************************************************************
** Form generated from reading UI file 'pidchart.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PIDCHART_H
#define UI_PIDCHART_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PIDChart
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *PIDChart)
    {
        if (PIDChart->objectName().isEmpty())
            PIDChart->setObjectName(QString::fromUtf8("PIDChart"));
        PIDChart->resize(800, 600);
        centralwidget = new QWidget(PIDChart);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        PIDChart->setCentralWidget(centralwidget);
        menubar = new QMenuBar(PIDChart);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 25));
        PIDChart->setMenuBar(menubar);
        statusbar = new QStatusBar(PIDChart);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        PIDChart->setStatusBar(statusbar);

        retranslateUi(PIDChart);

        QMetaObject::connectSlotsByName(PIDChart);
    } // setupUi

    void retranslateUi(QMainWindow *PIDChart)
    {
        PIDChart->setWindowTitle(QCoreApplication::translate("PIDChart", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PIDChart: public Ui_PIDChart {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PIDCHART_H
