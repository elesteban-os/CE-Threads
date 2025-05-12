#include "widget.h"
#include "ui_widget.h"

#include <iostream>
#include <QPixmap>
#include <QDebug>
#include <QPropertyAnimation>
#include <unistd.h>
#include <QThread>

// struct para los parametros del thread
struct ThreadArgs {
    Widget* widget;
    SignDirection direction;
};

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


void Widget::animateAndWait(SignDirection direction) {
    QLabel* movingLabel = new QLabel(this);
    movingLabel->setPixmap(QPixmap(":/assets/deportivo.png"));
    movingLabel->setScaledContents(true);
    movingLabel->setGeometry(30, 310, 141, 101);

    if (direction == SignDirection::LEFT) {
        QPixmap original(":/assets/deportivo.png");
        QPixmap mirrored = original.transformed(QTransform().scale(-1, 1));  // reflejar horizontalmente
        movingLabel->setPixmap(mirrored);
        movingLabel->setGeometry(980, 310, 141, 101);  // posición inicial desde la derecha
    }

    movingLabel->show();

    QPropertyAnimation* anim = new QPropertyAnimation(movingLabel, "geometry");
    anim->setDuration(3000);
    QRect startRect = movingLabel->geometry();
    QRect endRect = (direction == SignDirection::RIGHT)
                    ? startRect.translated(950, 0)    // derecha
                    : startRect.translated(-950, 0);  // izquierda
    anim->setStartValue(startRect);
    anim->setEndValue(endRect);

    connect(anim, &QPropertyAnimation::finished, [movingLabel, anim]() {
        movingLabel->deleteLater();
        anim->deleteLater();
    });

    anim->start();

    QEventLoop loop;
    connect(anim, &QPropertyAnimation::finished, &loop, &QEventLoop::quit);
    loop.exec();
}




void* thread_task(void* arg) {
    ThreadArgs* args = static_cast<ThreadArgs*>(arg);
    Widget* widget = args->widget;
    SignDirection direction = args->direction;
    delete args;  // liberar memoria después de usar

    std::cout << "ejecutando hilo" << std::endl;

    if (QThread::currentThread() == widget->thread()) {
        widget->animateAndWait(direction);
    } else {
        QMetaObject::invokeMethod(widget, "animateAndWait", Qt::BlockingQueuedConnection,
                                  Q_ARG(SignDirection, direction));
    }

    return nullptr;
}



void Widget::on_pushButton_clicked()
{
    ProcessManagement pm(ScheduleType::SJF, 3, FlowAlgorithm::EQUITY, 3);

    // // Agregar procesos al lado izquierdo
    pm.newLeftProcess(Process::withBurstTime(1, 10));
    pm.newLeftProcess(Process::withBurstTime(2, 5));
    pm.newLeftProcess(Process::withBurstTime(3, 8));

    // // Agregar procesos al lado derecho
    pm.newRightProcess(Process::withBurstTime(4, 7));
    pm.newRightProcess(Process::withBurstTime(5, 3));
    pm.newRightProcess(Process::withBurstTime(6, 12));

    while (true) {
        ProcessData* data = pm.getData();

        if (data == nullptr) {
            std::cout << "No hay más procesos para ejecutar." << std::endl;
            break;
        }

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

        // Pausar la ejecución para simular tiempo real
        //sleep(0.5);

    }
}
