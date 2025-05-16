#include "widget.h"
#include "ui_widget.h"

#include <iostream>
#include <QPixmap>
#include <QDebug>
#include <QPropertyAnimation>
#include <unistd.h>
#include <QThread>

// struct para los parametros del thread
std::atomic<bool> shouldExit = false;

struct ThreadArgs
{
    Widget* widget;
    SignDirection direction;
    int threadID;
    int time;
    int percentage;
    int typecar;
};

Q_DECLARE_METATYPE(SignDirection)

void Widget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Q)
    {
        qDebug() << "Saliendo del programa por tecla Q...";
        shouldExit = true;
        close(); // Esto dispara el evento closeEvent
    }
    else if (event->key() == Qt::Key_A) {  // Tecla "A" + verificar que pm existe
        generateNewCar();
    }
}

void Widget::closeEvent(QCloseEvent *event)
{
    shouldExit = true; // Reafirma el cierre por seguridad
    event->accept();
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    qRegisterMetaType<SignDirection>("SignDirection");
    qRegisterMetaType<carImageData*>("carImageData*");

    comboBoxType = findChild<QComboBox*>("comboBox");
    comboBoxSide = findChild<QComboBox*>("comboBox_2");

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

    normalCarPixmap = QPixmap(":/assets/normal.png");
    normalCarPixmapMirrored = normalCarPixmap.transformed(QTransform().scale(-1, 1));

    normalCarLabel = new QLabel(this);
    normalCarLabel->setPixmap(normalCarPixmap);
    normalCarLabel->setScaledContents(true);
    normalCarLabel->resize(141, 141);
    normalCarLabel->hide();

    ambulanceCarPixmap = QPixmap(":/assets/emergencia.png");
    ambulanceCarPixmapMirrored = ambulanceCarPixmap.transformed(QTransform().scale(-1, 1));

    ambulanceCarLabel = new QLabel(this);
    ambulanceCarLabel->setPixmap(ambulanceCarPixmap);
    ambulanceCarLabel->setScaledContents(true);
    ambulanceCarLabel->resize(141, 141);
    ambulanceCarLabel->hide();

    signLabel = new QLabel(this);
    signLabel->setGeometry(510, 220, 71, 61);
    signLabel->setStyleSheet("color: white; font: 60pt Nimbus Sans Narrow;");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setSignLabel(SignDirection sd) {
    QString signText;
    if (sd == SignDirection::RIGHT) {
        signText = "→";
    } else {
        signText = "←";
    }
    signLabel->setText(signText);
}

void Widget::generateNewCar(){
    QString carType = findChild<QComboBox*>("comboBox")->currentText();
    QString sideStr = findChild<QComboBox*>("comboBox_2")->currentText();

    SignDirection direction = (sideStr == "Izquierda") ? SignDirection::LEFT : SignDirection::RIGHT;

    int base_speed = 10;
    int burst_time_normal = 100 / base_speed; // 10
    int burst_time_sport = 100 / (base_speed * 2); // 5
    int burst_time_emergency = 100 / (base_speed * 4); // 2.5
    int burst_time;
    int priority;

    if (carType == "Normal") {
        burst_time = burst_time_normal;
        priority = 3;
    } else if (carType == "Deportivo") {
        burst_time = burst_time_sport;
        priority = 2;
    } else {
        burst_time = burst_time_emergency;
        priority = 1;
    }

    if (direction == SignDirection::LEFT) {
        pm->newRightProcess(Process(this->carID, burst_time, priority, burst_time * 10, burst_time / 3, priority * this->carID));
        //pm->newLeftProcess(Process::withBurstTime(carID, 3));
        carID++;
    } else {
        pm->newLeftProcess(Process(this->carID, burst_time, priority, burst_time * 10, burst_time / 3, priority * this->carID));
        carID++;
    }
    qDebug() << "Carro generado - Tipo:" << carType << "| Lado:" << sideStr << "| ID:" << carID;
}

void Widget::animateAndWait(carImageData* carData, int percentage, int time)
{
    QLabel* carLabel = carData->label;
    QRect* startRect = carData->actualRect;
    SignDirection direction = carData->direction;

    QPropertyAnimation* anim = new QPropertyAnimation(carLabel, "geometry");
    anim->setDuration(time * 1000);
    QRect endRect = (direction == SignDirection::RIGHT)
                    ? startRect->translated(((1160 * percentage) / 100) - startRect->x(), 0)
                    : startRect->translated(((-1160 * percentage) / 100) + (980 - startRect->x()), 0);
    QRect* endRectCpy = new QRect(endRect);
    anim->setStartValue(*startRect);
    anim->setEndValue(endRect);

    // Eliminar el QLabel cuando el porcentaje llegue a 100
    if (percentage == 100) {
        connect(anim, &QPropertyAnimation::finished, [this, carLabel, anim]() {
            //carLabel->deleteLater();
            //anim->deleteLater();
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

carImageData* Widget::addNewCarImage(int carID, SignDirection direction, int typecar) {
    QRect* startRect;
    QLabel* carLabel = new QLabel(this);
    carLabel->setScaledContents(true);
    carLabel->resize(141, 101);
    if (typecar == 1){
        if (direction == SignDirection::LEFT) {
            carLabel->setPixmap(ambulanceCarPixmapMirrored);
            startRect = new QRect(980, 290, 141, 141);  // desde la derecha
        } else {
            carLabel->setPixmap(ambulanceCarPixmap);
            startRect = new QRect(30, 290, 141, 141);  // desde la izquierda
        }
    }
    if (typecar == 2){
        if (direction == SignDirection::LEFT) {
            carLabel->setPixmap(sportCarPixmapMirrored);
            startRect = new QRect(980, 420, 141, 101);  // desde la derecha
        } else {
            carLabel->setPixmap(sportCarPixmap);
            startRect = new QRect(30, 420, 141, 101);  // desde la izquierda
        }
    }
    if (typecar == 3){
        if (direction == SignDirection::LEFT) {
            carLabel->setPixmap(normalCarPixmapMirrored);
            startRect = new QRect(980, 350, 141, 141);  // desde la derecha
        } else {
            carLabel->setPixmap(normalCarPixmap);
            startRect = new QRect(30, 350, 141, 141);  // desde la izquierda
        }
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
    int time = args->time;
    int percentage = args->percentage;

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
        newCarImage = widget->addNewCarImage(args->threadID, direction, args->typecar);
        QMetaObject::invokeMethod(widget, "animateAndWait",
                                  Qt::QueuedConnection,
                                  Q_ARG(carImageData*, newCarImage),
                                  Q_ARG(int, percentage),
                                  Q_ARG(int, time));
        loop.exec();  // Espera hasta que la animación emita animationFinished

    } else {
        // Si el hilo ya existe, animar el QLabel existente
        // Llamar a la animación desde el hilo de la GUI
        QMetaObject::invokeMethod(widget, "animateAndWait",
                                  Qt::QueuedConnection,
                                  Q_ARG(carImageData*, newCarImage),
                                  Q_ARG(int, percentage),
                                  Q_ARG(int, time));
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

        // Imprimir el algoritmo de flujo:
        std::cout << "Algoritmo de flujo: ";
        switch (data->flowAlgorithm) {
            case FlowAlgorithm::EQUITY:
            std::cout << "Equidad";
            break;
            case FlowAlgorithm::SIGN:
            std::cout << "Letrero";
            break;
            case FlowAlgorithm::FIFO:
            std::cout << "FIFO";
            break;
        }
        std::cout << std::endl;


        std::cout << "Tiempo dn movimiento: " << data->time << std::endl;
        std::cout << "Porcentaje de la calle que se debe mover " << data->street_percentage << "%" << std::endl;

        ThreadArgs* args = new ThreadArgs{ this, data->direction, data->actualProcess->getProcessID(), data->time, data->street_percentage, data->actualProcess->getPriority()};

        setSignLabel(data->direction);

        CEthread_t thread;
        CEthread_create(&thread, thread_task, args);
        CEthread_join(&thread);

        if (shouldExit) {
            break;
        }

    }
    for (auto& carImage : carImages) {
        //carImage->label->deleteLater();
        carImage->label->hide();
        delete carImage->actualRect;
        delete carImage;
    }
    carImages.clear();
    // ocultar los labels despues de terminar la simulacion
    scheduleLabel->hide();
    flowLabel->hide();
    queueLabel->hide();
    actualThreadLabel->hide();

    //pm = new ProcessManagement(ScheduleType::PRIORITY, 3, FlowAlgorithm::EQUITY, 3, 3, 100); // volver al default para manual
}

void Widget::on_pushButton_2_clicked()
{
    Configuration config;
    try
    {
        QString fileName = QFileDialog::getOpenFileName(this, "Select a File", "", "Text Files (*.json);;All Files (*)");

        if (!fileName.isEmpty()) {
            QMessageBox::information(this, "File Selected", "You selected:\n" + fileName);
        }
        std::string fileNameqrst = fileName.toStdString();
        config = read_configuration(fileNameqrst);
    }
    catch(const exception& e)
    {
        cerr << "Error: " << e.what() << endl;
    }

    // Tipo calendarizacion
    ScheduleType schedule_type;
    if (config.calendarizer == "FCFS") {
        schedule_type = ScheduleType::FCFS;
    } else if (config.calendarizer == "SJF") {
        schedule_type = ScheduleType::SJF;
    } else if (config.calendarizer == "RR") {
        schedule_type = ScheduleType::RR;
    } else if (config.calendarizer == "Prioridad") {
        schedule_type = ScheduleType::PRIORITY;
    } else if (config.calendarizer == "Real Time") {
        schedule_type = ScheduleType::REALTIME;
    } else {
        cerr << "Error: Unknown calendarizer type '" << config.calendarizer << "'" << endl;
        return;
    }

    // Tamanio de la cola
    int queue_size = config.parameters.queue_size;

    // Algoritmo de flujo
    FlowAlgorithm flow_algorithm;
    if (config.control_method == "Equidad") {
        flow_algorithm = FlowAlgorithm::EQUITY;
    } else if (config.control_method == "Letrero") {
        flow_algorithm = FlowAlgorithm::SIGN;
    } else if (config.control_method == "FIFO") {
        flow_algorithm = FlowAlgorithm::FIFO;
    } else {
        cerr << "Error: Unknown flow algorithm type '" << config.control_method << "'" << endl;
        return;
    }

    // Atributo compartido (W o Time)
    int flattr;
    if (flow_algorithm == FlowAlgorithm::EQUITY) {
        flattr = config.parameters.w_param;
    } else {
        flattr = config.parameters.sign_time;
    }

    int quantum = config.parameters.quantum;

    // Crear una instancia de ProcessManagement
    int street = config.street.size;
    pm = new ProcessManagement(schedule_type, queue_size, flow_algorithm, flattr, quantum, street);

    // Diferentes burst times:
    // y = mx
    // d = vt
    // t = d / v

    int base_speed = config.street.base_speed;
    int burst_time_normal = street / base_speed; // 10
    int burst_time_sport = street / (base_speed * 2); // 5
    int burst_time_emergency = street / (base_speed * 4); // 2.5

    // Agregar procesos al lado izquierdo
    // Prueba SJF
    // Agregar carros alternando el tipo
    int car_id = 1;
    int leftPeriod = 3;
    while (config.left.normal > 0 || config.left.deportive > 0 || config.left.emergency > 0) {
        if (config.left.normal > 0) {
            pm->newLeftProcess(Process(car_id++, burst_time_normal, 3, burst_time_normal * 20, burst_time_normal / 10, leftPeriod));
            config.left.normal--;
            leftPeriod += 3;
        }
        if (config.left.deportive > 0) {
            pm->newLeftProcess(Process(car_id++, burst_time_sport, 2, burst_time_sport * 18, burst_time_sport / 6, leftPeriod));
            config.left.deportive--;
            leftPeriod += 6;
        }
        if (config.left.emergency > 0) {
            pm->newLeftProcess(Process(car_id++, burst_time_emergency, 1, burst_time_emergency * 16, burst_time_emergency / 3, leftPeriod));
            config.left.emergency--;
            leftPeriod += 9;
        }
    }

    // Agregar procesos al lado derecho
    // Prueba SJF
    // Agregar carros alternando el tipo
    int rightPeriod = 1;
    while (config.right.normal > 0 || config.right.deportive > 0 || config.right.emergency > 0) {
        if (config.right.normal > 0) {
            pm->newRightProcess(Process(car_id++, burst_time_normal, 3, burst_time_normal * 10, burst_time_normal / 3, rightPeriod));
            config.right.normal--;
            rightPeriod += 3;
        }
        if (config.right.deportive > 0) {
            pm->newRightProcess(Process(car_id++, burst_time_sport, 2, burst_time_sport * 6, burst_time_sport / 3, rightPeriod));
            config.right.deportive--;
            rightPeriod += 6;
        }
        if (config.right.emergency > 0) {
            pm->newRightProcess(Process(car_id++, burst_time_emergency, 1, burst_time_emergency * 4, burst_time_emergency / 3, rightPeriod));
            config.right.emergency--;
            rightPeriod += 9;
        }
    }
}
