#include "widget.h"
#include "ui_widget.h"
#include<QPainter>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QPixmap pixmap(400,300);//绘图设备，400*300

    QPainter p(&pixmap);

    //填充白色背景
//    p.fillRect(0,0,400,300,Qt::white);//通过画笔填充

    pixmap.fill(Qt::white);//通过设备填充

    p.drawPixmap(0,0,80,80,QPixmap("../Image/buffer1.png"));

    //保存图片
    pixmap.save("../pixmap.png");
}

Widget::~Widget()
{
    delete ui;
}
