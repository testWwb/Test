#include "widget.h"
#include "ui_widget.h"
#include<QPainter>
#include<QMouseEvent>
/*功能：实现不规则窗口，去掉窗口多余部分
 * 时间：2018.09.15
 */
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //去窗口边框
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    //把窗口背景设置为透明
    setAttribute(Qt::WA_TranslucentBackground);

}

Widget::~Widget()
{
    delete ui;
}
void Widget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.drawPixmap(0,0,QPixmap("../Image/buffer1.png"));
}
void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        //如果是右键，关闭窗口
        close();
    }
    else if(event->button() == Qt::LeftButton)
    {
        //求坐标差值
        //当前点击坐标-窗口左上角坐标
        p = event->globalPos()-this->frameGeometry().topLeft();

    }
}
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)//左击
    {
        //移动是窗口左上角原点移动，p是获得点击点处与左上角原点的差
        move(event->globalPos() - p);//实现移动
//        move(event->globalPos());

    }
}
