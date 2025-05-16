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
#include <vector>

struct carImageData {
    QLabel* label;
    int carID;
    SignDirection direction;
    QRect* actualRect;
};

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    Q_INVOKABLE void animateAndWait(carImageData* carData, int percentage, int time);
    void setScheduleTypeLabel(ScheduleType scheduler);
    void setFlowLabel(FlowAlgorithm flowAlgorithm);
    void setQueueLabel(std::queue<int> queue);
    void setActualThreadLabel(int id);

    std::vector<carImageData*> carImages;
    carImageData* addNewCarImage(int carID, SignDirection direction);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
    QLabel* scheduleLabel = nullptr;
    QLabel* flowLabel = nullptr;
    QLabel* queueLabel = nullptr;
    QLabel* actualThreadLabel = nullptr;
    QLabel* sportCarLabel = nullptr;
    QPixmap sportCarPixmap;
    QPixmap sportCarPixmapMirrored;
    ProcessManagement* pm;

signals:
    void animationFinished();
};
#endif // WIDGET_H
