#include "widget.h"
#include "ui_widget.h"
#include<QPainter>
#include<QPen>
#include<QBrush>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    x=0;
}

Widget::~Widget()
{
    delete ui;
}
void Widget::paintEvent(QPaintEvent *event)
{
   // QPainter p(this);//一种操作
   //另一种操作
    QPainter p;//创建绘图对象
    p.begin(this);//指定当前窗口为绘图设备

    //绘图操作
    //p.drawXXX

    //画背景图
//    p.drawPixmap(0,0,width(),height(),QPixmap("../Image/cropPhoto.jpg"));//图片名字太长和有数字不会显示？有问题
    p.drawPixmap(rect(),QPixmap("../Image/cropPhoto.jpg"));//rect()获取窗口矩形大小，更简单

    //定义画笔
    QPen pen;
    pen.setWidth(5);//设置线宽
//    pen.setColor(Qt::red);//简单设置颜色
    pen.setColor(QColor(14,9,234));//rgb设置颜色
    pen.setStyle(Qt::DashLine);//设置风格
    //把画笔交给画家
    p.setPen(pen);

    //画直线
    p.drawLine(50,50,150,50);
    p.drawLine(50,50,50,150);

    //创建画刷对象
    QBrush brush;
    brush.setColor(Qt::red);//设置颜色
    brush.setStyle(Qt::Dense1Pattern);//设置样式
    //把画刷给画家
    p.setBrush(brush);

    //画矩形
    p.drawRect(200,200,100,50);

    //画圆
    p.drawEllipse(QPoint(150,150),50,50);

    //画笑脸
    p.drawPixmap(x,180,80,80,QPixmap("../Image/buffer1.png"));

    p.end();
}

void Widget::on_pushButton_clicked()
{
    x+=20;
    if(x>width())
    {
        x=0;
    }
    //刷新窗口，让窗口重绘，整个窗口都刷新
    update();//间接调用paintEvent（）
}
