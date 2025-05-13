#include "widget.h"
#include "ui_widget.h"

#include <iostream>
#include <QPixmap>
#include <QDebug>
#include <QPropertyAnimation>
#include <unistd.h>
#include <QThread>

// struct para los parametros del thread
struct ThreadArgs
{
    Widget* widget;
    SignDirection direction;
};

Q_DECLARE_METATYPE(SignDirection)

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    qRegisterMetaType<SignDirection>("SignDirection");

    /* setear los labels de informacion flujo, calendarizacion y cola */
    scheduleLabel = new QLabel(this);
    scheduleLabel->setGeometry(390, 180, 201, 41);
    scheduleLabel->setStyleSheet("color: white; font: 14pt Nimbus Sans Narrow;");
    scheduleLabel->show();

    flowLabel = new QLabel(this);
    flowLabel->setGeometry(640, 180, 171, 41);
    flowLabel->setStyleSheet("color: white; font: 14pt Nimbus Sans Narrow;");
    flowLabel->show();

    queueLabel = new QLabel(this);
    queueLabel->setGeometry(40, 610, 1081, 41);
    queueLabel->setStyleSheet("color: white; font: 14pt Nimbus Sans Narrow;");
    queueLabel->show();

    actualThreadLabel = new QLabel(this);
    actualThreadLabel->setGeometry(400, 560, 61, 31);
    actualThreadLabel->setStyleSheet("color: white; font: 14pt Nimbus Sans Narrow;");
    actualThreadLabel->show();

    /* setear los labels relacionados al carro deportivo */
    sportCarPixmap = QPixmap(":/assets/deportivo.png");
    sportCarPixmapMirrored = sportCarPixmap.transformed(QTransform().scale(-1, 1));

    sportCarLabel = new QLabel(this);
    sportCarLabel->setPixmap(sportCarPixmap);
    sportCarLabel->setScaledContents(true);
    sportCarLabel->resize(141, 101);
    sportCarLabel->hide();
}

Widget::~Widget()
{
    delete ui;
}


void Widget::animateAndWait(SignDirection direction)
{
    QRect startRect;

    if (direction == SignDirection::LEFT) {
        sportCarLabel->setPixmap(sportCarPixmapMirrored);
        startRect = QRect(980, 310, 141, 101);  // desde la derecha
    } else {
        sportCarLabel->setPixmap(sportCarPixmap);
        startRect = QRect(30, 310, 141, 101);  // desde la izquierda
    }

    sportCarLabel->setGeometry(startRect);
    sportCarLabel->show();

    QPropertyAnimation* anim = new QPropertyAnimation(sportCarLabel, "geometry");
    anim->setDuration(3000);
    QRect endRect = (direction == SignDirection::RIGHT)
                    ? startRect.translated(950, 0)
                    : startRect.translated(-950, 0);
    anim->setStartValue(startRect);
    anim->setEndValue(endRect);

    // No destruimos el label, solo ocultamos cuando termina
    connect(anim, &QPropertyAnimation::finished, [this, anim]() {
        sportCarLabel->hide();
        anim->deleteLater();
    });

    anim->start();

    QEventLoop loop;
    connect(anim, &QPropertyAnimation::finished, &loop, &QEventLoop::quit);
    loop.exec();
}


void* thread_task(void* arg)
{
    ThreadArgs* args = static_cast<ThreadArgs*>(arg);
    Widget* widget = args->widget;
    SignDirection direction = args->direction;

    std::cout << "ejecutando hilo" << std::endl;

    if (QThread::currentThread() == widget->thread()) {
        widget->animateAndWait(direction);
    } else {
        bool success = QMetaObject::invokeMethod(widget, "animateAndWait", Qt::BlockingQueuedConnection,
                                                 Q_ARG(SignDirection, direction));
        if (!success) {
            std::cerr << "Error: invokeMethod falló." << std::endl;
        }
    }
    delete args;  // liberar memoria después de usar

    return nullptr;
}

void Widget::setScheduleTypeLabel(ScheduleType scheduler)
{
    QString scheduleStr;
    switch (scheduler) {
        case ScheduleType::FCFS: scheduleStr = "FCFS"; break;
        case ScheduleType::SJF: scheduleStr = "SJF"; break;
        case ScheduleType::RR: scheduleStr = "RR"; break;
        case ScheduleType::PRIORITY: scheduleStr = "PRIORIDAD"; break;
        case ScheduleType::REALTIME: scheduleStr = "REALTIME"; break;
    }
    scheduleLabel->setText(scheduleStr);
}

void Widget::setFlowLabel(FlowAlgorithm flowAlgorithm)
{
    QString flowStr;
    switch (flowAlgorithm) {
        case FlowAlgorithm::EQUITY: flowStr = "EQUIDAD"; break;
        case FlowAlgorithm::SIGN: flowStr = "LETRERO"; break;
        case FlowAlgorithm::FIFO: flowStr = "FIFO"; break;
    }
    flowLabel->setText(flowStr);
}

void Widget::setQueueLabel(std::queue<int> queue)
{
    QString queueStr = "";
    while (!queue.empty()) {
        queueStr += QString::number(queue.front()) + " ";
        queue.pop();
    }
    queueLabel->setText(queueStr.trimmed());
}

void Widget::setActualThreadLabel(int id)
{
    QString idstr = QString::number(id);
    actualThreadLabel->setText(idstr);
}



void Widget::on_pushButton_clicked()
{
    ProcessManagement pm(ScheduleType::SJF, 3, FlowAlgorithm::SIGN, 3);

    // // Agregar procesos al lado izquierdo
    pm.newLeftProcess(Process::withBurstTime(1, 1));
    pm.newLeftProcess(Process::withBurstTime(2, 2));
    pm.newLeftProcess(Process::withBurstTime(3, 3));

    // // Agregar procesos al lado derecho
    pm.newRightProcess(Process::withBurstTime(4, 4));
    pm.newRightProcess(Process::withBurstTime(5, 5));
    pm.newRightProcess(Process::withBurstTime(6, 6));

    while (true) {
        ProcessData* data = pm.getData();

        if (data == nullptr) {
            std::cout << "No hay más procesos para ejecutar." << std::endl;
            break;
        }

        setScheduleTypeLabel(data->scheduleType);
        setFlowLabel(data->flowAlgorithm);
        setQueueLabel(data->actualProcessIDQueue);
        setActualThreadLabel(data->actualProcess->getProcessID());

        // Imprimir la información de los datos obtenidos
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Direccion del flujo: "
                  << (data->direction == SignDirection::LEFT ? "Izquierda" : "Derecha")
                  << std::endl;

        std::cout << "Procesos en la cola actual: ";
        std::queue<int> tempQueue = data->actualProcessIDQueue;
        while (!tempQueue.empty()) {
            std::cout << tempQueue.front() << " ";
            tempQueue.pop();
        }
        std::cout << std::endl;

        if (data->actualProcess != nullptr) {
            std::cout << "Proceso en ejecucion: " << data->actualProcess->getProcessID() << std::endl;
        } else {
            std::cout << "No hay proceso en ejecucion." << std::endl;
        }

        std::cout << "Algoritmo de planificacion: ";
        switch (data->scheduleType) {
            case ScheduleType::FCFS:
                std::cout << "FCFS";
                break;
            case ScheduleType::SJF:
                std::cout << "SJF";
                break;
            case ScheduleType::RR:
                std::cout << "RR";
                break;
            case ScheduleType::PRIORITY:
                std::cout << "PRIORITY";
                break;
            case ScheduleType::REALTIME:
                std::cout << "REALTIME";
                break;
        }
        std::cout << std::endl;

        ThreadArgs* args = new ThreadArgs{ this, data->direction };

        CEthread_t thread;
        CEthread_create(&thread, thread_task, args);  // pasás el puntero al Widget
        CEthread_join(&thread);

    }
    // ocultar los labels despued de terminar la simulacion
    scheduleLabel->hide();
    flowLabel->hide();
    queueLabel->hide();
    actualThreadLabel->hide();
}
