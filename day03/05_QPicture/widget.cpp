#include "widget.h"
#include "ui_widget.h"
#include<QPicture>
#include<QPainter>
#include<QPixmap>
#include<QImage>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QPicture picture;
    QPainter p;
    p.begin(&picture);

    p.drawPixmap(0,0,80,80,QPixmap("../Image/buffer.jpg"));
    p.drawLine(50,50,150,50);

    p.end();
    //保存的是二进制文件
    picture.save("../picture.png");

}

Widget::~Widget()
{
    delete ui;
}
void Widget::paintEvent(QPaintEvent *event)
{
#if 0
    QPicture pic;
    pic.load("../picture.png");//加载文件
    QPainter p(this);
    p.drawPicture(0,0,pic);
#endif//注释掉了
    QPainter p(this);

    QPixmap pixmap;
    pixmap.load("../Image/buffer.jpg");
    //pixmap转换为QImage
    QImage tempImage=pixmap.toImage();
    p.drawImage(0,0,tempImage);

    QImage image;
    image.load("../Image/buffer.jpg");
    //QImage转换为QPixmap
    QPixmap tempPixmap = QPixmap::fromImage(image);
    p.drawPixmap(200,0,tempPixmap);


}
