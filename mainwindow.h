#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <func.h>
#include <QTimer>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_OnOff_clicked();
    void on_spinBox_Ax_valueChanged(int arg1);
    void on_spinBox_Wx_valueChanged(int arg1);
    void on_spinBox_Dx_valueChanged(int arg1);
    void on_spinBox_Ay_valueChanged(int arg1);
    void on_spinBox_Wy_valueChanged(int arg1);
    void on_spinBox_Dy_valueChanged(int arg1);

    void slotTimerAlarm();

    void on_pushButton_clear_clicked();
    void on_spinBox_request_valueChanged(int arg1);

private:
    void paintEvent(QPaintEvent*);
    void render();
    Ui::MainWindow *ui;
    QTimer *timer;

private:
    QPixmap pixmap;
    QPainter *PixMapPainter;

    Func *func = new Func();

    COORD coords;

    int requestTime;
};
#endif // MAINWINDOW_H
