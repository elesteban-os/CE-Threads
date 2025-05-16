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
    int threadID;
    int time;
};

Q_DECLARE_METATYPE(SignDirection)

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    qRegisterMetaType<SignDirection>("SignDirection");
    qRegisterMetaType<carImageData*>("carImageData*");

    /* setear los labels de informacion flujo, calendarizacion y cola */
    scheduleLabel = new QLabel(this);
    scheduleLabel->setGeometry(390, 180, 201, 41);
    scheduleLabel->setStyleSheet("color: white; font: 14pt Nimbus Sans Narrow;");

    flowLabel = new QLabel(this);
    flowLabel->setGeometry(640, 180, 171, 41);
    flowLabel->setStyleSheet("color: white; font: 14pt Nimbus Sans Narrow;");

    queueLabel = new QLabel(this);
    queueLabel->setGeometry(40, 610, 1081, 41);
    queueLabel->setStyleSheet("color: white; font: 14pt Nimbus Sans Narrow;");

    actualThreadLabel = new QLabel(this);
    actualThreadLabel->setGeometry(400, 560, 61, 31);
    actualThreadLabel->setStyleSheet("color: white; font: 14pt Nimbus Sans Narrow;");

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


void Widget::animateAndWait(carImageData* carData, int percentage, int time)
{
    QLabel* carLabel = carData->label;
    QRect* startRect = carData->actualRect;
    SignDirection direction = carData->direction;

    QPropertyAnimation* anim = new QPropertyAnimation(carLabel, "geometry");
    anim->setDuration(3000);
    QRect endRect = (direction == SignDirection::RIGHT)
                    ? startRect->translated((950 * percentage) / 100, 0)
                    : startRect->translated((-950 * percentage) / 100, 0);
    QRect* endRectCpy = new QRect(endRect);
    anim->setStartValue(*startRect);
    anim->setEndValue(endRect);

    // Eliminar el QLabel cuando el porcentaje llegue a 100
    if (percentage == 100) {
        connect(anim, &QPropertyAnimation::finished, [this, carLabel, anim]() {
            carLabel->deleteLater();
            anim->deleteLater();
            emit animationFinished();  // Señal para desbloquear el hilo
        });
    } else {
        connect(anim, &QPropertyAnimation::finished, [this, anim]() {
            emit animationFinished();  // Señal para desbloquear el hilo
        });
    }

    carData->actualRect = endRectCpy;

    anim->start();
}

carImageData* Widget::addNewCarImage(int carID, SignDirection direction) {
    QRect* startRect;
    QLabel* carLabel = new QLabel(this);
    carLabel->setScaledContents(true);
    carLabel->resize(141, 101);

    if (direction == SignDirection::LEFT) {
        carLabel->setPixmap(sportCarPixmapMirrored);
        startRect = new QRect(980, 310, 141, 101);  // desde la derecha
    } else {
        carLabel->setPixmap(sportCarPixmap);
        startRect = new QRect(30, 310, 141, 101);  // desde la izquierda
    }

    carLabel->setGeometry(*startRect);
    carLabel->show();

    // Agregar el nuevo QLabel al vector de carImages
    carImageData *newCarImage = new carImageData;
    newCarImage->carID = carID;
    newCarImage->label = carLabel;
    newCarImage->direction = direction;
    newCarImage->actualRect = startRect;
    carImages.push_back(newCarImage);
    return newCarImage;
}


void* thread_task(void* arg)
{
    ThreadArgs* args = static_cast<ThreadArgs*>(arg);
    Widget* widget = args->widget;
    SignDirection direction = args->direction;

    bool found = false;
    carImageData* newCarImage;

    for (const auto& carImage : widget->carImages) {
        if (carImage->carID == args->threadID) {
            found = true;
            newCarImage = carImage;
            break;
        }
    }

    QEventLoop loop;

    QObject::connect(widget, &Widget::animationFinished, &loop, &QEventLoop::quit, Qt::QueuedConnection);

    if (!found) {
        // Agregar el nuevo QLabel al vector de carImages
        // Llamar a la animación desde el hilo de la GUI
        newCarImage = widget->addNewCarImage(args->threadID, direction);
        QMetaObject::invokeMethod(widget, "animateAndWait",
                                  Qt::QueuedConnection,
                                  Q_ARG(carImageData*, newCarImage),
                                  Q_ARG(int, 30),
                                  Q_ARG(int, 1));
        std::cout << "esperando respuesta..."<< std::endl;
        loop.exec();  // Espera hasta que la animación emita animationFinished
        std::cout << "NO" << std::endl;

    } else {
        // Si el hilo ya existe, animar el QLabel existente
        std::cout << "SI" << std::endl;
        // Llamar a la animación desde el hilo de la GUI
        QMetaObject::invokeMethod(widget, "animateAndWait",
                                  Qt::QueuedConnection,
                                  Q_ARG(carImageData*, newCarImage),
                                  Q_ARG(int, 30),
                                  Q_ARG(int, 1));
        std::cout << "esperando respuesta..."<< std::endl;
        loop.exec();  // Espera hasta que la animación emita animationFinished
    }

    delete args;
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
    scheduleLabel->show();
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
    flowLabel->show();
}

void Widget::setQueueLabel(std::queue<int> queue)
{
    QString queueStr = "";
    while (!queue.empty()) {
        queueStr += QString::number(queue.front()) + " ";
        queue.pop();
    }
    queueLabel->setText(queueStr.trimmed());
    queueLabel->show();
}

void Widget::setActualThreadLabel(int id)
{
    QString idstr = QString::number(id);
    actualThreadLabel->setText(idstr);
    actualThreadLabel->show();
}



void Widget::on_pushButton_clicked()
{
    //ProcessManagement pm(ScheduleType::SJF, 3, FlowAlgorithm::SIGN, 3);


    while (true) {
        ProcessData* data = pm->getData();

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

        ThreadArgs* args = new ThreadArgs{ this, data->direction, data->actualProcess->getProcessID(), 1};

        CEthread_t thread;

        std::cout << "Creando thread" << std::endl;
        CEthread_create(&thread, thread_task, args);
        std::cout << "Thread creado" << std::endl;
        CEthread_join(&thread);
        std::cout << "Join thread" << std::endl;

    }
    // ocultar los labels despues de terminar la simulacion
    scheduleLabel->hide();
    flowLabel->hide();
    queueLabel->hide();
    actualThreadLabel->hide();
}

void Widget::on_pushButton_2_clicked()
{
    pm = new ProcessManagement(ScheduleType::SJF, 3, FlowAlgorithm::SIGN, 3);

    // // Agregar procesos al lado izquierdo
    pm->newLeftProcess(Process::withBurstTime(1, 1));
    pm->newLeftProcess(Process::withBurstTime(2, 2));
    pm->newLeftProcess(Process::withBurstTime(3, 3));

    // // Agregar procesos al lado derecho
    pm->newRightProcess(Process::withBurstTime(4, 4));
    pm->newRightProcess(Process::withBurstTime(5, 5));
    pm->newRightProcess(Process::withBurstTime(6, 6));
}
