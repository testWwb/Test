#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _fps(0),
    _interval(20),
    _start(false),
    _pause(false),
    _begin(0),
    _lastPointKey(0)
{
    ui->setupUi(this);
    // 初始化界面
    init();
    // 选择模式一
    on_radioButton_mode1_clicked(true);
    // 更新按钮状态
    ui->radioButton_mode1->setChecked(true);
    ui->comboBox_select->setCurrentIndex(2);
    ui->pushButton_pause->setEnabled(false);
    ui->pushButton_stop->setEnabled(false);

     qDebug()<<"主线程号："<<QThread::currentThread();

     for(int i = 0;i<100;i++){
    myT = new myThread;
    thread = new QThread(this);
    myT->moveToThread(thread);
    thread->start();
    connect(myT,&myThread::mySignal,this,&MainWindow::realtimeDataSlot);

    connect(this,&MainWindow::startThread,myT,&myThread::myTimeout);
     }
    connect(this,&MainWindow::destroyed,this,&MainWindow::deal);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    // 增加图层
    ui->customPlot->addGraph(); // blue line
    // 设置图层画笔颜色
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    // 设置图层画刷颜色
//    ui->customPlot->graph(0)->setBrush(QBrush(Qt::yellow));
    // 设置图层反锯齿：关闭
    ui->customPlot->graph(0)->setAntialiasedFill(false);
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));
    // 只设置画刷默认是从该图的电画到0点，若设置画刷会则也会画
//    ui->customPlot->graph(1)->setBrush(QBrush(QColor(240, 255, 0)));
    // 填充的颜色是从用0的画刷画图0到图2之间的点
    ui->customPlot->graph(0)->setChannelFillGraph(ui->customPlot->graph(1));
    // 蓝点所在的图层
    ui->customPlot->addGraph();
    // 设置笔的颜色
    ui->customPlot->graph(2)->setPen(QPen(Qt::blue));
    // 不画线条
    ui->customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
    // 设置点的形状
    ui->customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
    // 红点所在图层
    ui->customPlot->addGraph();
    ui->customPlot->graph(3)->setPen(QPen(Qt::red));
    ui->customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);
    // 下标的格式：数字 或者 时钟
    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltNumber);
    // 设置时钟下标格式
//    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setRange(0, 8);
    // 是否允许自动下标
    ui->customPlot->xAxis->setAutoTickStep(false);
    // 手动设置下标，要先禁止自动下标，隔多少间距显示下标
    ui->customPlot->xAxis->setTickStep(1);
    // 设置steps之间的间隔杠
    ui->customPlot->xAxis->setAutoSubTicks(false);
    ui->customPlot->xAxis->setSubTickCount(9); // 9+1
//    ui->customPlot->axisRect()->setupFullAxesBox();
    // 设置横轴标签
    ui->customPlot->xAxis->setLabel("时间(单位s)");
    // 设置纵轴范围
    ui->customPlot->yAxis->setRange(-1, 1);
    // 设置纵轴标签 仅使用1个字会报换行符错误 后面加个空格
    ui->customPlot->yAxis->setLabel("大小");
    ui->customPlot->yAxis->setAutoTickStep(false);
    ui->customPlot->yAxis->setTickStep(0.2);
    ui->customPlot->yAxis->setAutoSubTicks(false);
    ui->customPlot->yAxis->setSubTickCount(1); // 1+1
    // Qt刷新图标界面的定时器
    _pTimerUpdate = new QTimer(this);
    // Qt定时器，定时刷新qt的程序界面
    connect(_pTimerUpdate, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
}

void MainWindow::realtimeDataSlot()
{
    // 每次刷新点的坐标，并且刷新qt图标的界面
    double key = (double)(_elapsedTimer.elapsed() + _begin) / 1000;
    // 10ms更新一个点
    if (key-_lastPointKey > 0.01)
    {
        double value0 = qSin(key); //sin(key*1.6+cos(key*1.7)*2)*10 + sin(key*1.2+0.56)*20 + 26;
        double value1 = qCos(key); //sin(key*1.3+cos(key*1.2)*1.2)*7 + sin(key*0.9+0.26)*24 + 26;
        if(_start && !_pause)
        {
            if(ui->customPlot->graph(0)->visible() && ui->customPlot->graph(1)->visible())
                ui->customPlot->graph(0)->setBrush(QBrush(Qt::yellow));
            else
                ui->customPlot->graph(0)->setBrush(QBrush(Qt::transparent));
            // 画线
            ui->customPlot->graph(0)->addData(key, value0);
            ui->customPlot->graph(1)->addData(key, value1);
            // 画点
            ui->customPlot->graph(2)->clearData();
            ui->customPlot->graph(2)->addData(key, value0);
            // 蓝色图层是否可见
            if(ui->customPlot->graph(0)->visible())
            {
                // 统一是x的格式，末尾为0也显示
                QString zzz = QString("%1").arg(key);
                for(int index = zzz.size() - zzz.lastIndexOf("."); index < 4; index++)
                {
                    zzz += "0";
                }
                ui->label_maleX->setText(QString("%1").arg(zzz));
                // 在msvc2010编译上会出现错误：9个QString::arg匹配 和  mid右侧不是联合
//                QString strValue0 = QString("%1").arg(value0, 4, 10, QChar('0')).mid(0, 6);
                QString strValue0 = QString("%1").arg(value0).mid(0, 6);
                strValue0 = strValue0.startsWith("-")?strValue0.mid(0,6):strValue0.mid(0,5);
                ui->label_maleY->setText(strValue0);
            }else
            {
                ui->label_maleX->setText("");
                ui->label_maleY->setText("");
            }
            ui->customPlot->graph(3)->clearData();
            ui->customPlot->graph(3)->addData(key, value1);
            // 红色图层是否可见
            if(ui->customPlot->graph(1)->visible())
            {
                // 统一是x的格式，末尾为0也显示
                QString zzz = QString("%1").arg(key);
                for(int index = zzz.size() - zzz.lastIndexOf("."); index < 4; index++)
                {
                    zzz += "0";
                }
                ui->label_femaleX->setText(QString("%1").arg(zzz));
                // 在msvc2010编译上会出现错误：9个QString::arg匹配 和  mid右侧不是联合
//                QString strValue1 = QString("%1").arg(value1, 4, 10, '0').mid(0, 6);
                QString strValue1 = QString("%1").arg(value1).mid(0, 6);
                strValue1 = strValue1.startsWith("-")?strValue1.mid(0,6):strValue1.mid(0,5);
                ui->label_femaleY->setText(strValue1);
            }else
            {
                ui->label_femaleX->setText("");
                ui->label_femaleY->setText("");
            }
            // 移除图标已逝去的点，避免其占用内存
            ui->customPlot->graph(0)->removeDataBefore(key-8);
            ui->customPlot->graph(1)->removeDataBefore(key-8);
            // rescale value (vertical) axis to fit the current data:
            // 根据该图像最高点和最低点，缩放Y周
//            ui->customPlot->graph(0)->rescaleValueAxis();
            // 根据该图像最高点和最低点，缩放Y周 以最后一个为主，所以这里只加入边界，用true
//            ui->customPlot->graph(1)->rescaleValueAxis(true);
            _lastPointKey = key;
        }
    }
    // 当显示的X轴超过8的时候，X坐标范围开始移动
    if(key > 8.0)
    {
        ui->customPlot->xAxis->setRange(key+0.01, 8, Qt::AlignRight);
    }
    ui->customPlot->replot();

    // 计算每秒的帧数
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    // 取2秒的平衡均值
    if (key-lastFpsKey > 2)
    {
        // 设置状态栏显示FPS
        ui->statusBar->showMessage(
              QString("%1 FPS")
              .arg(frameCount/(key-lastFpsKey), 0, 'f', 0));
        lastFpsKey = key;
        frameCount = 0;
    }
}

void MainWindow::on_checkBox_male_clicked(bool checked)
{
    ui->customPlot->graph(0)->setVisible(checked);
    ui->customPlot->graph(2)->setVisible(checked);
}

void MainWindow::deal()
{

    delete myT;
}

void MainWindow::on_checkBox_female_clicked(bool checked)
{
    ui->customPlot->graph(1)->setVisible(checked);
    ui->customPlot->graph(3)->setVisible(checked);
}

void MainWindow::on_radioButton_mode1_clicked(bool checked)
{
    ui->groupBox_comboBox->setEnabled(checked);
    ui->groupBox_checkBox->setEnabled(!checked);
    // 根据选择项，设置图层可见非可见
    switch(ui->comboBox_select->currentIndex())
    {
    case 0:
        ui->customPlot->graph(0)->setVisible(true);
        ui->customPlot->graph(2)->setVisible(true);
        ui->customPlot->graph(1)->setVisible(false);
        ui->customPlot->graph(3)->setVisible(false);
        break;
    case 1:
        ui->customPlot->graph(0)->setVisible(false);
        ui->customPlot->graph(2)->setVisible(false);
        ui->customPlot->graph(1)->setVisible(true);
        ui->customPlot->graph(3)->setVisible(true);
        break;
    case 2:
        ui->customPlot->graph(0)->setVisible(true);
        ui->customPlot->graph(2)->setVisible(true);
        ui->customPlot->graph(1)->setVisible(true);
        ui->customPlot->graph(3)->setVisible(true);
    default:
        ;
    }

}

void MainWindow::on_radioButton_mode2_clicked(bool checked)
{
    ui->groupBox_comboBox->setEnabled(!checked);
    ui->groupBox_checkBox->setEnabled(checked);
    ui->customPlot->graph(0)->setVisible(ui->checkBox_male->isChecked());
    ui->customPlot->graph(2)->setVisible(ui->checkBox_male->isChecked());
    ui->customPlot->graph(1)->setVisible(ui->checkBox_female->isChecked());
    ui->customPlot->graph(3)->setVisible(ui->checkBox_female->isChecked());
}

void MainWindow::on_pushButton_start_clicked()
{
    _start = true;
    _pause = false;
    _pTimerUpdate->start(0);
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_pause->setEnabled(true);
    ui->pushButton_stop->setEnabled(true);
    ui->pushButton_pause->setText("暂停");
    _elapsedTimer.restart();



    myT->setFlag(false);
    emit startThread();
}

void MainWindow::on_pushButton_pause_clicked()
{
    _pause = !_pause;
    if(_pause)
    {
        ui->pushButton_pause->setText("继续");
        _pTimerUpdate->stop();
        _begin += _elapsedTimer.elapsed();
    }else
    {
        ui->pushButton_pause->setText("暂停");
        _pTimerUpdate->start(0);
        _elapsedTimer.restart();
    }
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_pause->setEnabled(true);
    ui->pushButton_stop->setEnabled(true);
}

void MainWindow::on_pushButton_stop_clicked()
{
    ui->customPlot->graph(0)->clearData();
    ui->customPlot->graph(1)->clearData();
    ui->customPlot->graph(2)->clearData();
    ui->customPlot->graph(3)->clearData();
    ui->customPlot->replot();
    ui->label_maleX->setText("");
    ui->label_maleY->setText("");
    ui->label_femaleX->setText("");
    ui->label_femaleY->setText("");
    ui->customPlot->xAxis->setRange(0, 8);
    _begin = 0;
    _start = false;
    _pause = true;
    _lastPointKey = 0;
    _pTimerUpdate->stop();
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_pause->setEnabled(false);
    ui->pushButton_stop->setEnabled(false);




    myT->setFlag(true);
    thread->quit();
    thread->wait();
}

void MainWindow::on_pushButton_close_clicked()
{
    exit(0);
}

void MainWindow::on_comboBox_select_currentIndexChanged(int index)
{
    // 根据选择项，设置图层可见非可见
    switch(index)
    {
    case 0:
        ui->customPlot->graph(0)->setVisible(true);
        ui->customPlot->graph(2)->setVisible(true);
        ui->customPlot->graph(1)->setVisible(false);
        ui->customPlot->graph(3)->setVisible(false);
        break;
    case 1:
        ui->customPlot->graph(0)->setVisible(false);
        ui->customPlot->graph(2)->setVisible(false);
        ui->customPlot->graph(1)->setVisible(true);
        ui->customPlot->graph(3)->setVisible(true);
        break;
    case 2:
        ui->customPlot->graph(0)->setVisible(true);
        ui->customPlot->graph(2)->setVisible(true);
        ui->customPlot->graph(1)->setVisible(true);
        ui->customPlot->graph(3)->setVisible(true);
    default:
        ;
    }
}
