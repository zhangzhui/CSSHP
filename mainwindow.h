#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "transducer/transducer_thread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void HideButtons();
    void ConfigurePlot(double now);
    void ConfigureScrollBar(double now);
    void ConfigureTracer();
    void ConfigureAlarmList();
    void StartTimer();
    void BuildSignalSlot();

private:
    void StartWorker();
    void ChangeOnOffBtnIcon();

private slots:
    void on_welcome_next_clicked();
    void on_realtime_prev_clicked();
    void on_realtime_next_clicked();
    void on_history_prev_clicked();
    void on_history_next_clicked();
    void on_param_prev_clicked();
    void onTimeOut();
    void onShijianTimeout();
    void horzScrollBarPressed();
    void horzScrollBarMoved(int);
    void horzScrollBarReleased();
    void xAxisChanged(QCPRange);
    void plotMouseMove(QMouseEvent*);
    void on_welcom_on_off_clicked();
    void on_btn_freq_up_clicked();
    void on_btn_freq_down_clicked();
    // transducer
    void OnTransducerOnOff(bool off);

signals:
    void StartTransducer();
    void StopTransducer();
    void IncreaseTransducerFreq();
    void DecreaseTransducerFreq();

private:
    Ui::MainWindow *ui;
    QTimer *timer_;
    QTimer *timer_shijian_;
    bool system_on_;
    QCPItemTracer *tracer_;
    TransducerThread t_thread_;
};
#endif // MAINWINDOW_H
