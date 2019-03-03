#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPropertyAnimation>
#include <QPen>
#include <QColor>
#include <QRgb>
#include <QDateTime>
#include <QElapsedTimer>
#include<QThread>

#include "qcustomplot.h"
#include"mythread.h"


/********************************************************************************\
 类的名称： MainWindow
 功能描述: 使用QCustomSlot范例
 主要函数：
         init - 初始化
         realtimeDataSlot - qt实时刷新
 使用方法：
         直接作为主界面程序运行
 作者： 杨炯    QQ:21497936   邮件: 21497936@qq.com
   版本    修改者      日期            备注
   V1.0    杨炯    2017年9月5日      基本功能

\********************************************************************************/

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    void init();

signals:
    void startThread();

protected slots:
    void on_checkBox_female_clicked(bool checked);
    void on_checkBox_male_clicked(bool checked);

    void deal();

private slots:
    void realtimeDataSlot();
    void on_radioButton_mode1_clicked(bool checked);
    void on_radioButton_mode2_clicked(bool checked);
    void on_pushButton_start_clicked();
    void on_pushButton_pause_clicked();
    void on_pushButton_stop_clicked();
    void on_pushButton_close_clicked();
    void on_comboBox_select_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    double _minX;
    double _maxX;
    QTimer *_pTimerUpdate;
    QCPGraph * _pCPGraph;
    QCPGraph * _pCPGraph2;
    double _minX2;
    double _maxX2;
    int _fps;
    int _interval;
    bool _start;
    bool _pause;
    QElapsedTimer _elapsedTimer;
    qint64 _begin;
    double _lastPointKey;


    myThread *myT;
    QThread *thread;
};

#endif // MAINWINDOW_H
