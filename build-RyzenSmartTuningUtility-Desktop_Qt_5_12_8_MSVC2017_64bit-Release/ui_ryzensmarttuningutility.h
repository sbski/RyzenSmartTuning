/********************************************************************************
** Form generated from reading UI file 'ryzensmarttuningutility.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RYZENSMARTTUNINGUTILITY_H
#define UI_RYZENSMARTTUNINGUTILITY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RyzenSmartTuningUtility
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *RyzenSmartTuningUtility)
    {
        if (RyzenSmartTuningUtility->objectName().isEmpty())
            RyzenSmartTuningUtility->setObjectName(QString::fromUtf8("RyzenSmartTuningUtility"));
        RyzenSmartTuningUtility->resize(737, 367);
        centralwidget = new QWidget(RyzenSmartTuningUtility);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        RyzenSmartTuningUtility->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RyzenSmartTuningUtility);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 737, 21));
        RyzenSmartTuningUtility->setMenuBar(menubar);
        statusbar = new QStatusBar(RyzenSmartTuningUtility);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        RyzenSmartTuningUtility->setStatusBar(statusbar);

        retranslateUi(RyzenSmartTuningUtility);

        QMetaObject::connectSlotsByName(RyzenSmartTuningUtility);
    } // setupUi

    void retranslateUi(QMainWindow *RyzenSmartTuningUtility)
    {
        RyzenSmartTuningUtility->setWindowTitle(QApplication::translate("RyzenSmartTuningUtility", "RyzenSmartTuningUtility", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RyzenSmartTuningUtility: public Ui_RyzenSmartTuningUtility {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RYZENSMARTTUNINGUTILITY_H
