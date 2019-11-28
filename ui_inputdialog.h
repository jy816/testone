/********************************************************************************
** Form generated from reading UI file 'inputdialog.ui'
**
** Created: Mon Jul 8 23:47:31 2019
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INPUTDIALOG_H
#define UI_INPUTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_inputDialog
{
public:
    QLabel *label_4;
    QLabel *timer_state;
    QLabel *label_7;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit_oil_i;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_5;
    QLineEdit *lineEdit_oil_ii;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_6;
    QLineEdit *lineEdit_spd_ii;
    QWidget *layoutWidget3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QLineEdit *lineEdit_spd_i;
    QWidget *layoutWidget4;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QLineEdit *profibus_num;
    QWidget *layoutWidget5;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *pushButton;
    QPushButton *etrButton;

    void setupUi(QDialog *inputDialog)
    {
        if (inputDialog->objectName().isEmpty())
            inputDialog->setObjectName(QString::fromUtf8("inputDialog"));
        inputDialog->resize(661, 202);
        label_4 = new QLabel(inputDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 20, 291, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\215\216\346\226\207\346\245\267\344\275\223"));
        font.setPointSize(16);
        label_4->setFont(font);
        timer_state = new QLabel(inputDialog);
        timer_state->setObjectName(QString::fromUtf8("timer_state"));
        timer_state->setGeometry(QRect(570, 20, 31, 20));
        label_7 = new QLabel(inputDialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(480, 20, 101, 18));
        layoutWidget = new QWidget(inputDialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(21, 70, 281, 30));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\215\216\346\226\207\346\245\267\344\275\223"));
        label->setFont(font1);

        horizontalLayout->addWidget(label);

        lineEdit_oil_i = new QLineEdit(layoutWidget);
        lineEdit_oil_i->setObjectName(QString::fromUtf8("lineEdit_oil_i"));

        horizontalLayout->addWidget(lineEdit_oil_i);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 1);
        layoutWidget1 = new QWidget(inputDialog);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(342, 70, 301, 30));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(layoutWidget1);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font1);

        horizontalLayout_2->addWidget(label_5);

        lineEdit_oil_ii = new QLineEdit(layoutWidget1);
        lineEdit_oil_ii->setObjectName(QString::fromUtf8("lineEdit_oil_ii"));

        horizontalLayout_2->addWidget(lineEdit_oil_ii);

        horizontalLayout_2->setStretch(0, 6);
        horizontalLayout_2->setStretch(1, 5);
        layoutWidget2 = new QWidget(inputDialog);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(341, 110, 301, 30));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(layoutWidget2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font1);

        horizontalLayout_3->addWidget(label_6);

        lineEdit_spd_ii = new QLineEdit(layoutWidget2);
        lineEdit_spd_ii->setObjectName(QString::fromUtf8("lineEdit_spd_ii"));

        horizontalLayout_3->addWidget(lineEdit_spd_ii);

        horizontalLayout_3->setStretch(0, 6);
        layoutWidget3 = new QWidget(inputDialog);
        layoutWidget3->setObjectName(QString::fromUtf8("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(20, 110, 281, 30));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget3);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font1);

        horizontalLayout_4->addWidget(label_2);

        lineEdit_spd_i = new QLineEdit(layoutWidget3);
        lineEdit_spd_i->setObjectName(QString::fromUtf8("lineEdit_spd_i"));

        horizontalLayout_4->addWidget(lineEdit_spd_i);

        horizontalLayout_4->setStretch(0, 1);
        horizontalLayout_4->setStretch(1, 1);
        layoutWidget4 = new QWidget(inputDialog);
        layoutWidget4->setObjectName(QString::fromUtf8("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(20, 150, 281, 30));
        horizontalLayout_5 = new QHBoxLayout(layoutWidget4);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(layoutWidget4);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font1);

        horizontalLayout_5->addWidget(label_3);

        profibus_num = new QLineEdit(layoutWidget4);
        profibus_num->setObjectName(QString::fromUtf8("profibus_num"));

        horizontalLayout_5->addWidget(profibus_num);

        horizontalLayout_5->setStretch(0, 1);
        horizontalLayout_5->setStretch(1, 1);
        layoutWidget5 = new QWidget(inputDialog);
        layoutWidget5->setObjectName(QString::fromUtf8("layoutWidget5"));
        layoutWidget5->setGeometry(QRect(340, 150, 301, 30));
        horizontalLayout_6 = new QHBoxLayout(layoutWidget5);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(layoutWidget5);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_6->addWidget(pushButton);

        etrButton = new QPushButton(layoutWidget5);
        etrButton->setObjectName(QString::fromUtf8("etrButton"));
        etrButton->setAutoDefault(true);
        etrButton->setDefault(false);

        horizontalLayout_6->addWidget(etrButton);

        horizontalLayout_6->setStretch(0, 1);
        horizontalLayout_6->setStretch(1, 1);

        retranslateUi(inputDialog);

        QMetaObject::connectSlotsByName(inputDialog);
    } // setupUi

    void retranslateUi(QDialog *inputDialog)
    {
        inputDialog->setWindowTitle(QApplication::translate("inputDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("inputDialog", "\347\233\221\346\265\213\347\263\273\347\273\237\351\205\215\347\275\256\350\256\276\347\275\256\357\274\232", 0, QApplication::UnicodeUTF8));
        timer_state->setText(QString());
        label_7->setText(QApplication::translate("inputDialog", "\350\256\241\346\225\260\345\200\222\350\256\241\346\227\266\357\274\232", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("inputDialog", "\346\273\221\346\262\271\345\216\213\345\212\233\344\275\216I\345\200\274\346\212\245\350\255\246\357\274\232", 0, QApplication::UnicodeUTF8));
        lineEdit_oil_i->setPlaceholderText(QString());
        label_5->setText(QApplication::translate("inputDialog", "\346\273\221\346\262\271\345\216\213\345\212\233\344\275\216II\345\200\274\346\212\245\350\255\246\357\274\232", 0, QApplication::UnicodeUTF8));
        lineEdit_oil_ii->setPlaceholderText(QString());
        label_6->setText(QApplication::translate("inputDialog", "\350\266\205\351\200\237II\345\200\274\346\212\245\350\255\246\357\274\232", 0, QApplication::UnicodeUTF8));
        lineEdit_spd_ii->setPlaceholderText(QString());
        label_2->setText(QApplication::translate("inputDialog", "\350\266\205\351\200\237I\345\200\274\346\212\245\350\255\246\345\200\274\357\274\232", 0, QApplication::UnicodeUTF8));
        lineEdit_spd_i->setPlaceholderText(QString());
        label_3->setText(QApplication::translate("inputDialog", "PROFIBUS\346\240\210\345\217\267\357\274\232", 0, QApplication::UnicodeUTF8));
        profibus_num->setPlaceholderText(QString());
        pushButton->setText(QApplication::translate("inputDialog", "\345\220\257\345\212\250\345\272\224\347\224\250\347\250\213\345\272\217", 0, QApplication::UnicodeUTF8));
        etrButton->setText(QApplication::translate("inputDialog", "\351\273\230\350\256\244\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class inputDialog: public Ui_inputDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INPUTDIALOG_H
