#include "widget.h"
#include "ui_widget.h"
#include<QBuffer>
#include<QDebug>
#include<QDataStream>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
#if 0
    QBuffer memFile;//创建内存文件
    memFile.open(QIODevice::WriteOnly);
    memFile.write("11111111");
    memFile.write("22222222");

    memFile.close();

    qDebug()<<memFile.buffer();
#endif

    QByteArray array;
    QBuffer memFile(&array);//创建内存文件
    memFile.open(QIODevice::WriteOnly);
    memFile.write("11111111");
    memFile.write("22222222");

    memFile.close();

    qDebug()<<memFile.buffer();
    qDebug()<<"array"<<array;

    QBuffer memFile1;
    memFile1.open(QIODevice::WriteOnly);
    QDataStream stream(&memFile1);
    stream<<QString("来呀来呀")<<250;
    memFile1.close();

    qDebug()<<memFile1.buffer();

    memFile1.open(QIODevice::ReadOnly);
    QDataStream stream1;
    stream1.setDevice(&memFile1);
    QString str;
    int a;
    stream1>>str>>a;
    memFile1.close();

    qDebug()<<str<<a;


}

Widget::~Widget()
{
    delete ui;
}
