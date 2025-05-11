#include "widget.h"
#include "ui_widget.h"
#include <iostream>
#include <QPixmap>
#include <QDebug>
#include <QPropertyAnimation>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    QPropertyAnimation *anim = new QPropertyAnimation(ui->imagenDeportivo, "geometry");
    anim->setDuration(10000); // duración en milisegundos (2 segundos)

    QRect startRect = ui->imagenDeportivo->geometry();
    QRect endRect = startRect.translated(600, 0); // mover 200 píxeles a la derecha

    anim->setStartValue(startRect);
    anim->setEndValue(endRect);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
