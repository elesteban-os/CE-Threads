/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGraphicsView *graphicsView_2;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QComboBox *comboBox;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QComboBox *comboBox_2;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_4;
    QWidget *verticalLayoutWidget_4;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_5;
    QGraphicsView *graphicsView_3;
    QGraphicsView *graphicsView_4;
    QGraphicsView *graphicsView_5;
    QGraphicsView *graphicsView_6;
    QFrame *line;
    QFrame *line_2;
    QWidget *widget;
    QWidget *widget_2;
    QWidget *widget_3;
    QWidget *widget_4;
    QWidget *widget_5;
    QWidget *widget_6;
    QWidget *widget_7;
    QWidget *widget_8;
    QWidget *widget_9;
    QWidget *widget_10;
    QPushButton *pushButton;
    QGraphicsView *graphicsView;
    QPushButton *pushButton_2;
    QWidget *verticalLayoutWidget_5;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_6;
    QGraphicsView *graphicsView_7;
    QWidget *verticalLayoutWidget_6;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_7;
    QGraphicsView *graphicsView_8;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1159, 774);
        graphicsView_2 = new QGraphicsView(Widget);
        graphicsView_2->setObjectName(QString::fromUtf8("graphicsView_2"));
        graphicsView_2->setGeometry(QRect(30, 20, 1101, 92));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(graphicsView_2->sizePolicy().hasHeightForWidth());
        graphicsView_2->setSizePolicy(sizePolicy);
        graphicsView_2->setStyleSheet(QString::fromUtf8("background-color:  #1C223A;  border-radius: 12px;"));
        verticalLayoutWidget = new QWidget(Widget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(40, 40, 358, 55));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("font: 11pt \"Nimbus Sans Narrow\";"));

        verticalLayout->addWidget(label);

        verticalLayoutWidget_3 = new QWidget(Widget);
        verticalLayoutWidget_3->setObjectName(QString::fromUtf8("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(40, 130, 151, 29));
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(verticalLayoutWidget_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setStyleSheet(QString::fromUtf8("font: 14pt \"Nimbus Sans Narrow\";"));

        verticalLayout_3->addWidget(label_2);

        comboBox = new QComboBox(Widget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(30, 180, 161, 41));
        comboBox->setStyleSheet(QString::fromUtf8("background-color:  #1C223A;  border-radius: 5px; color: white; font: 12pt \"Nimbus Sans Narrow\";"));
        horizontalLayoutWidget = new QWidget(Widget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(210, 130, 151, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(horizontalLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setStyleSheet(QString::fromUtf8("font: 14pt \"Nimbus Sans Narrow\";"));

        horizontalLayout->addWidget(label_3);

        comboBox_2 = new QComboBox(Widget);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));
        comboBox_2->setGeometry(QRect(210, 180, 151, 41));
        comboBox_2->setStyleSheet(QString::fromUtf8("background-color:  #1C223A;  border-radius: 5px; color: white; font: 12pt \"Nimbus Sans Narrow\"; "));
        verticalLayoutWidget_2 = new QWidget(Widget);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(380, 130, 225, 29));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(verticalLayoutWidget_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setStyleSheet(QString::fromUtf8("font: 14pt \"Nimbus Sans Narrow\";"));

        verticalLayout_2->addWidget(label_4);

        verticalLayoutWidget_4 = new QWidget(Widget);
        verticalLayoutWidget_4->setObjectName(QString::fromUtf8("verticalLayoutWidget_4"));
        verticalLayoutWidget_4->setGeometry(QRect(630, 130, 141, 29));
        verticalLayout_4 = new QVBoxLayout(verticalLayoutWidget_4);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(verticalLayoutWidget_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setStyleSheet(QString::fromUtf8("font: 14pt \"Nimbus Sans Narrow\";"));

        verticalLayout_4->addWidget(label_5);

        graphicsView_3 = new QGraphicsView(Widget);
        graphicsView_3->setObjectName(QString::fromUtf8("graphicsView_3"));
        graphicsView_3->setGeometry(QRect(30, 120, 791, 51));
        graphicsView_3->setStyleSheet(QString::fromUtf8("background-color:  #1C223A;  border-radius: 12px; border: 2px solid #6C63FF;"));
        graphicsView_4 = new QGraphicsView(Widget);
        graphicsView_4->setObjectName(QString::fromUtf8("graphicsView_4"));
        graphicsView_4->setGeometry(QRect(380, 180, 231, 41));
        graphicsView_4->setStyleSheet(QString::fromUtf8("background-color:  #1C223A;  border-radius: 5px; "));
        graphicsView_5 = new QGraphicsView(Widget);
        graphicsView_5->setObjectName(QString::fromUtf8("graphicsView_5"));
        graphicsView_5->setGeometry(QRect(630, 180, 191, 41));
        graphicsView_5->setStyleSheet(QString::fromUtf8("background-color:  #1C223A;  border-radius: 5px; "));
        graphicsView_6 = new QGraphicsView(Widget);
        graphicsView_6->setObjectName(QString::fromUtf8("graphicsView_6"));
        graphicsView_6->setGeometry(QRect(0, 290, 1161, 241));
        graphicsView_6->setStyleSheet(QString::fromUtf8("background-color:  #282634; "));
        line = new QFrame(Widget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(0, 290, 1161, 20));
        line->setStyleSheet(QString::fromUtf8(""));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(Widget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(0, 510, 1161, 20));
        line_2->setStyleSheet(QString::fromUtf8(""));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        widget = new QWidget(Widget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(50, 410, 51, 16));
        widget->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);"));
        widget_2 = new QWidget(Widget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setGeometry(QRect(160, 410, 51, 16));
        widget_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);"));
        widget_3 = new QWidget(Widget);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setGeometry(QRect(270, 410, 51, 16));
        widget_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);"));
        widget_4 = new QWidget(Widget);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        widget_4->setGeometry(QRect(380, 410, 51, 16));
        widget_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);"));
        widget_5 = new QWidget(Widget);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        widget_5->setGeometry(QRect(490, 410, 51, 16));
        widget_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);"));
        widget_6 = new QWidget(Widget);
        widget_6->setObjectName(QString::fromUtf8("widget_6"));
        widget_6->setGeometry(QRect(600, 410, 51, 16));
        widget_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);"));
        widget_7 = new QWidget(Widget);
        widget_7->setObjectName(QString::fromUtf8("widget_7"));
        widget_7->setGeometry(QRect(710, 410, 51, 16));
        widget_7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);"));
        widget_8 = new QWidget(Widget);
        widget_8->setObjectName(QString::fromUtf8("widget_8"));
        widget_8->setGeometry(QRect(820, 410, 51, 16));
        widget_8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);"));
        widget_9 = new QWidget(Widget);
        widget_9->setObjectName(QString::fromUtf8("widget_9"));
        widget_9->setGeometry(QRect(930, 410, 51, 16));
        widget_9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);"));
        widget_10 = new QWidget(Widget);
        widget_10->setObjectName(QString::fromUtf8("widget_10"));
        widget_10->setGeometry(QRect(1040, 410, 51, 16));
        widget_10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);"));
        pushButton = new QPushButton(Widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(420, 670, 251, 81));
        pushButton->setStyleSheet(QString::fromUtf8("background-color:  #1C223A;  border-radius: 12px; color: white; font: 18pt \"Nimbus Sans Narrow\"; font-weight: bold; border: 2px solid #6C63FF;"));
        graphicsView = new QGraphicsView(Widget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(-1, -1, 1161, 771));
        graphicsView->setStyleSheet(QString::fromUtf8("background-color: #0D1025;"));
        pushButton_2 = new QPushButton(Widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(860, 120, 251, 51));
        pushButton_2->setStyleSheet(QString::fromUtf8("background-color:  #1C223A;  border-radius: 12px; color: white; font: 14pt \"Nimbus Sans Narrow\"; font-weight: bold; border: 2px solid #6C63FF;"));
        verticalLayoutWidget_5 = new QWidget(Widget);
        verticalLayoutWidget_5->setObjectName(QString::fromUtf8("verticalLayoutWidget_5"));
        verticalLayoutWidget_5->setGeometry(QRect(30, 560, 151, 31));
        verticalLayout_5 = new QVBoxLayout(verticalLayoutWidget_5);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(verticalLayoutWidget_5);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setStyleSheet(QString::fromUtf8("font: 14pt \"Nimbus Sans Narrow\";"));

        verticalLayout_5->addWidget(label_6);

        graphicsView_7 = new QGraphicsView(Widget);
        graphicsView_7->setObjectName(QString::fromUtf8("graphicsView_7"));
        graphicsView_7->setGeometry(QRect(30, 610, 1101, 41));
        graphicsView_7->setStyleSheet(QString::fromUtf8("background-color:  #1C223A;  border-radius: 5px; "));
        verticalLayoutWidget_6 = new QWidget(Widget);
        verticalLayoutWidget_6->setObjectName(QString::fromUtf8("verticalLayoutWidget_6"));
        verticalLayoutWidget_6->setGeometry(QRect(220, 560, 158, 31));
        verticalLayout_6 = new QVBoxLayout(verticalLayoutWidget_6);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(verticalLayoutWidget_6);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setStyleSheet(QString::fromUtf8("font: 14pt \"Nimbus Sans Narrow\";"));

        verticalLayout_6->addWidget(label_7);

        graphicsView_8 = new QGraphicsView(Widget);
        graphicsView_8->setObjectName(QString::fromUtf8("graphicsView_8"));
        graphicsView_8->setGeometry(QRect(390, 560, 61, 31));
        graphicsView_8->setStyleSheet(QString::fromUtf8("background-color:  #1C223A;  border-radius: 5px; "));
        graphicsView->raise();
        graphicsView_4->raise();
        graphicsView_5->raise();
        graphicsView_3->raise();
        graphicsView_2->raise();
        graphicsView_6->raise();
        verticalLayoutWidget->raise();
        verticalLayoutWidget_3->raise();
        comboBox->raise();
        horizontalLayoutWidget->raise();
        comboBox_2->raise();
        verticalLayoutWidget_2->raise();
        verticalLayoutWidget_4->raise();
        line->raise();
        line_2->raise();
        widget->raise();
        widget_2->raise();
        widget_3->raise();
        widget_4->raise();
        widget_5->raise();
        widget_6->raise();
        widget_7->raise();
        widget_8->raise();
        widget_9->raise();
        widget_10->raise();
        pushButton_2->raise();
        verticalLayoutWidget_5->raise();
        graphicsView_7->raise();
        pushButton->raise();
        verticalLayoutWidget_6->raise();
        graphicsView_8->raise();

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        label->setText(QApplication::translate("Widget", "<html><head/><body><p><span style=\" font-size:28pt; font-weight:700; color:#ffffff;\">Scheduling Cars System</span></p></body></html>", nullptr));
        label_2->setText(QApplication::translate("Widget", "<html><head/><body><p><span style=\" font-weight:700; color:#ffffff;\">Tipo de auto</span></p></body></html>", nullptr));
        comboBox->setItemText(0, QApplication::translate("Widget", "Normal", nullptr));
        comboBox->setItemText(1, QApplication::translate("Widget", "Deportivo", nullptr));
        comboBox->setItemText(2, QApplication::translate("Widget", "Emergencia", nullptr));

        label_3->setText(QApplication::translate("Widget", "<html><head/><body><p><span style=\" font-weight:700; color:#ffffff;\">Lado</span></p></body></html>", nullptr));
        comboBox_2->setItemText(0, QApplication::translate("Widget", "Izquierda", nullptr));
        comboBox_2->setItemText(1, QApplication::translate("Widget", "Derecha", nullptr));

        label_4->setText(QApplication::translate("Widget", "<html><head/><body><p><span style=\" font-weight:700; color:#ffffff;\">Algoritmo en calendarizaci\303\263n:</span></p></body></html>", nullptr));
        label_5->setText(QApplication::translate("Widget", "<html><head/><body><p><span style=\" font-weight:700; color:#ffffff;\">Algoritmo en flujo:</span></p></body></html>", nullptr));
        pushButton->setText(QApplication::translate("Widget", "Iniciar simulaci\303\263n", nullptr));
        pushButton_2->setText(QApplication::translate("Widget", "Cargar archivo de configuraci\303\263n", nullptr));
        label_6->setText(QApplication::translate("Widget", "<html><head/><body><p><span style=\" font-weight:700; color:#ffffff;\">Cola de ejecuci\303\263n:</span></p></body></html>", nullptr));
        label_7->setText(QApplication::translate("Widget", "<html><head/><body><p><span style=\" font-weight:696; color:#ffffff;\">Thread en ejecuci\303\263n:</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
