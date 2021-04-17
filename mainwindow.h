#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void HideButtons();
    void ConfigurePlot(double now);
    void ConfigureScrollBar(double now);
    void ConfigureTracer();
    void ConfigureAlarmList();
    void StartTimer();
    void BuildSignalSlot();

private slots:
    void on_welcome_next_clicked();
    void on_realtime_prev_clicked();
    void on_realtime_next_clicked();
    void on_history_prev_clicked();
    void on_history_next_clicked();
    void on_param_prev_clicked();
    void onTimeOut();
    void horzScrollBarPressed();
    void horzScrollBarMoved(int);
    void horzScrollBarReleased();
    void xAxisChanged(QCPRange);
    void plotMouseMove(QMouseEvent*);

private:
    Ui::MainWindow *ui;
    bool system_on_ = true;
    QTimer *timer_;
    QCPItemTracer *tracer_;
};
#endif // MAINWINDOW_H
