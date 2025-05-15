#ifndef WIDGET_H
#define WIDGET_H

#include "../flow/flow.h"
#include "../schedulers/scheduler.h"
#include "../processmanagement.h"
#include "../CE_threads.h"

#include <QWidget>
#include <QLabel>
#include <atomic>
#include <QKeyEvent>
#include <QApplication>
#include <QWaitCondition>
#include <QMutex>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    Q_INVOKABLE void animateAndWait(SignDirection direction);
    void setScheduleTypeLabel(ScheduleType scheduler);
    void setFlowLabel(FlowAlgorithm flowAlgorithm);
    void setQueueLabel(std::queue<int> queue);
    void setActualThreadLabel(int id);

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    QLabel* scheduleLabel = nullptr;
    QLabel* flowLabel = nullptr;
    QLabel* queueLabel = nullptr;
    QLabel* actualThreadLabel = nullptr;
    QLabel* sportCarLabel = nullptr;
    QPixmap sportCarPixmap;
    QPixmap sportCarPixmapMirrored;

signals:
    void animationFinished();
};
#endif // WIDGET_H
