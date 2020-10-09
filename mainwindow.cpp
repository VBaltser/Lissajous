#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pixmap(800,600)
{
    ui->setupUi(this);

    coords.X = 0;
    coords.Y = 0;

    //Создание таймера, проверяющего обновления
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
    timer->start(50);

    PixMapPainter = new QPainter(&pixmap);

    pixmap.fill(Qt::white);
    PixMapPainter->setBrush(QBrush(Qt::red));
    PixMapPainter->setPen(QPen(Qt::red));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Обновление
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter FormPainter(this);
    //Отрисовать pixmap на форме
    FormPainter.drawPixmap(0,0,800,600,pixmap);
}

//Отрисовка
void MainWindow::render()
{
    //Отрисовать точку на pixmap
    PixMapPainter->drawEllipse(coords.X + 400, coords.Y + 300, 5, 5);
    //Вызвать обновление
    update();
}

//Таймер проверяющий обновления
void MainWindow::slotTimerAlarm()
{
    //Если координаты обновлены, запросить новые
    if(func->isCoordUpdated())
    {
        WaitForSingleObject(func->hMutex, 2000);
        coords = func->getCoords();
        ReleaseMutex(func->hMutex);
    }

    //Отрисовать
    render();
}

void MainWindow::on_pushButton_OnOff_clicked()
{
    //Если программа работает - выключить
    //Если нет - запустить
    if(!func->startWork())
        func->endWork();
}

//Обновление параметров A W D
void MainWindow::on_spinBox_Ax_valueChanged(int arg1)
{
    func->setFuncParamsX(ui->spinBox_Ax->value(), ui->spinBox_Wx->value(), ui->spinBox_Dx->value());
}

void MainWindow::on_spinBox_Wx_valueChanged(int arg1)
{
    func->setFuncParamsX(ui->spinBox_Ax->value(), ui->spinBox_Wx->value(), ui->spinBox_Dx->value());
}

void MainWindow::on_spinBox_Dx_valueChanged(int arg1)
{
    func->setFuncParamsX(ui->spinBox_Ax->value(), ui->spinBox_Wx->value(), ui->spinBox_Dx->value());
}

void MainWindow::on_spinBox_Ay_valueChanged(int arg1)
{
    func->setFuncParamsY(ui->spinBox_Ay->value(), ui->spinBox_Wy->value(), ui->spinBox_Dy->value());
}

void MainWindow::on_spinBox_Wy_valueChanged(int arg1)
{
    func->setFuncParamsY(ui->spinBox_Ay->value(), ui->spinBox_Wy->value(), ui->spinBox_Dy->value());
}

void MainWindow::on_spinBox_Dy_valueChanged(int arg1)
{
    func->setFuncParamsY(ui->spinBox_Ay->value(), ui->spinBox_Wy->value(), ui->spinBox_Dy->value());
}

//Очистка pixmap при нажатии на "очистка"
void MainWindow::on_pushButton_clear_clicked()
{
    pixmap.fill(Qt::white);
}

//Изменение частоты запроса новых координат
void MainWindow::on_spinBox_request_valueChanged(int arg1)
{
    timer->setInterval(arg1);
}
