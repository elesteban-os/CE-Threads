#ifndef WIDGET_H
#define WIDGET_H

#include "../flow/flow.h"
#include "../schedulers/scheduler.h"
#include "../processmanagement.h"
#include "../CE_threads.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void animateAndWait(SignDirection direction);

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
