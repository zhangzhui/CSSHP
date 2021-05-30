#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "easylogging++.h"
#include "transducer/transducer_manager.h"

enum {
    PAGE_WELCOME,
    PAGE_REALTIME,
    PAGE_HISTORY,
    PAGE_PARAMETER,
};

const int displaySeconds = 60;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer_(new QTimer)
    , timer_shijian_(new QTimer)
    , system_on_(false)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
    ConfigureAlarmList();
    HideButtons();
    double now = QDateTime::currentDateTime().toTime_t();
    ConfigurePlot(now);
    ConfigureScrollBar(now);
    StartTimer();
    BuildSignalSlot();
    StartWorker();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::HideButtons()
{
    ui->welcom_prev->hide();
    ui->param_next->hide();
}

void MainWindow::ConfigurePlot(double now)
{
    // now: seconds of current time, we'll use it as starting point in time for data
    // set rand seed
    srand(now);
    // set plot locaization
    ui->plot->setLocale(QLocale(QLocale::Chinese, QLocale::China));
    // configure bottom axis to show date instead of number
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("hh:mm:ss");
    ui->plot->xAxis->setTicker(dateTicker);
    // set x/y range
    ui->plot->xAxis->setRange(now - displaySeconds, now);
    ui->plot->yAxis->setRange(-5,5);
    // drag plot enable
    //ui->plot->setInteractions(QCP::iRangeDrag);
}

void MainWindow::ConfigureScrollBar(double now)
{
    ui->horizontalScrollBar->setRange(now - displaySeconds, now);
    ui->horizontalScrollBar->setTracking(false);
    ui->horizontalScrollBar->setSliderPosition(now);
}

void MainWindow::ConfigureTracer()
{
    tracer_ = new QCPItemTracer(ui->plot);
    tracer_->setInterpolating(false);
    tracer_->setStyle(QCPItemTracer::tsCircle);
    tracer_->setPen(QPen(Qt::red));
    tracer_->setBrush(Qt::red);
    tracer_->setSize(6);
}

void MainWindow::ConfigureAlarmList()
{
    ui->welcome_alarm_table->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->welcome_alarm_table->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    ui->welcome_alarm_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->welcome_alarm_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    int rowCount = ui->welcome_alarm_table->rowCount();
    ui->welcome_alarm_table->insertRow(rowCount);
    ui->welcome_alarm_table->setItem(rowCount, 0, new QTableWidgetItem(QString::fromStdWString(L"2021/3/11 15:17")));
    ui->welcome_alarm_table->setItem(rowCount, 1, new QTableWidgetItem(QString::fromStdWString(L"这是一条报警内容")));
    ui->welcome_alarm_table->setItem(rowCount, 2, new QTableWidgetItem(QString::fromStdWString(L"1.0")));

    ui->history_alarm_table->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->history_alarm_table->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    ui->history_alarm_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->history_alarm_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    rowCount = ui->history_alarm_table->rowCount();
    ui->history_alarm_table->insertRow(rowCount);
    ui->history_alarm_table->setItem(rowCount, 0, new QTableWidgetItem(QString::fromStdWString(L"2021/3/11 15:17")));
    ui->history_alarm_table->setItem(rowCount, 1, new QTableWidgetItem(QString::fromStdWString(L"这是一条报警内容")));
    ui->history_alarm_table->setItem(rowCount, 2, new QTableWidgetItem(QString::fromStdWString(L"1.0")));
}

void MainWindow::StartTimer()
{
    timer_->setInterval(1000);
    connect(timer_, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    timer_->start();

    timer_shijian_->setInterval(1000);
    connect(timer_shijian_, SIGNAL(timeout()), this, SLOT(onShijianTimeout()));
    timer_shijian_->start();
}

void MainWindow::BuildSignalSlot()
{
    connect(ui->horizontalScrollBar, SIGNAL(sliderPressed()), this, SLOT(horzScrollBarPressed()));
    connect(ui->horizontalScrollBar, SIGNAL(sliderMoved(int)), this, SLOT(horzScrollBarMoved(int)));
    connect(ui->horizontalScrollBar, SIGNAL(sliderReleased()), this, SLOT(horzScrollBarReleased()));
    //connect(ui->plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    connect(ui->plot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(plotMouseMove(QMouseEvent*)));
}

void MainWindow::StartWorker() {
    TransducerManager::Instance()->Init();
    TransducerManager::Instance()->moveToThread(&t_thread_);
    connect(TransducerManager::Instance(), SIGNAL(TransducerOnOff(bool)), this, SLOT(OnTransducerOnOff(bool)));
    connect(this, SIGNAL(StartTransducer()), TransducerManager::Instance(), SLOT(OnStartTransducer()));
    connect(this, SIGNAL(StopTransducer()), TransducerManager::Instance(), SLOT(OnStopTransducer()));
    connect(this, SIGNAL(IncreaseTransducerFreq()), TransducerManager::Instance(), SLOT());

    t_thread_.start();
}

void MainWindow::on_welcome_next_clicked()
{
    ui->stackedWidget->setCurrentIndex(PAGE_REALTIME);
}

void MainWindow::on_realtime_prev_clicked()
{
    ui->stackedWidget->setCurrentIndex(PAGE_WELCOME);
}

void MainWindow::on_realtime_next_clicked()
{
    ui->stackedWidget->setCurrentIndex(PAGE_HISTORY);
}

void MainWindow::on_history_prev_clicked()
{
    ui->stackedWidget->setCurrentIndex(PAGE_REALTIME);
}

void MainWindow::on_history_next_clicked()
{
    ui->stackedWidget->setCurrentIndex(PAGE_PARAMETER);
}

void MainWindow::on_param_prev_clicked()
{
    ui->stackedWidget->setCurrentIndex(PAGE_HISTORY);
}

void MainWindow::onTimeOut()
{
    if (ui->plot->graph() == 0)
    {
        ui->plot->addGraph();
        ui->plot->graph()->setLineStyle(QCPGraph::lsLine);
    }

    double x = QDateTime::currentDateTime().toTime_t();
    int y = qrand() % 5;
    ui->plot->graph()->addData(x, y);

    bool autoScroll = ui->checkBox_auto->isChecked();
    if (autoScroll)
    {
        ui->plot->xAxis->setRange(x - displaySeconds, x);
        ui->plot->replot();
    }

    bool foundRange = false;
    QCPRange range = ui->plot->graph()->getKeyRange(foundRange);
    if (foundRange)
    {
        if (autoScroll)
        {
            double lower = range.lower;
            double upper = range.upper;
            ui->horizontalScrollBar->setRange(lower, upper);
            ui->horizontalScrollBar->setSliderPosition(upper);
        }
    }
}

void MainWindow::onShijianTimeout()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString strTime = currentTime.toString("yyyy-MM-dd hh:mm:ss");
    ui->welcom_time->setText(strTime);
    ui->realtime_time->setText(strTime);
    ui->history_time->setText(strTime);
}

void MainWindow::horzScrollBarPressed()
{
    ui->checkBox_auto->setChecked(false);
}

void MainWindow::horzScrollBarMoved(int pos)
{
    if (!ui->checkBox_auto->isChecked())
    {
        ui->plot->xAxis->setRange(pos - displaySeconds, pos);
        ui->plot->replot();
    }
}

void MainWindow::horzScrollBarReleased()
{
    if (!ui->checkBox_auto->isChecked())
    {
        int pos = ui->horizontalScrollBar->sliderPosition();
        int max = ui->horizontalScrollBar->maximum();
        if (pos == max)
        {
            ui->checkBox_auto->setChecked(true);
        }
    }
}

void MainWindow::xAxisChanged(QCPRange)
{
}

void MainWindow::plotMouseMove(QMouseEvent *)
{
    /*
    // get mouse position, relative to main window
    int x_pos = event->pos().x();
    int y_pos = event->pos().y();

    // translate to QCustomPlot coordinate
    float x_val = ui->plot->xAxis->pixelToCoord(x_pos); // time stamp
    float y_val = ui->plot->yAxis->pixelToCoord(y_pos); // value

    tracer_->setVisible(false);
    tracer_->setGraphKey(x_val);
    tracer_->updatePosition();
    tracer_->setVisible(true);
    tracer_->setGraph(ui->plot->graph());
    ui->plot->replot();
    */
}

void MainWindow::on_welcom_on_off_clicked()
{
    LINFO << "on_off button click: " << system_on_;
    system_on_ = !system_on_;
    ChangeOnOffBtnIcon();

    if (system_on_) {
        emit StartTransducer();
    } else {
        emit StopTransducer();
    }
}

void MainWindow::OnTransducerOnOff(bool off) {
    LINFO << "on transducer state off: " << off;
    system_on_ = off;
    ChangeOnOffBtnIcon();
}

void MainWindow::ChangeOnOffBtnIcon() {
    QIcon icon;
    icon.addFile(QString::fromUtf8(system_on_ ? ":/png/assets/png/kai.png" : ":/png/assets/png/guan.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->welcom_on_off->setIcon(icon);
}

void MainWindow::on_btn_freq_up_clicked()
{
    LINFO << "freq up button click";
    emit IncreaseTransducerFreq();
}

void MainWindow::on_btn_freq_down_clicked()
{
    LINFO << "freq down button click";
    emit DecreaseTransducerFreq();
}
