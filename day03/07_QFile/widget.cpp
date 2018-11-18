#include "widget.h"
#include "ui_widget.h"
#include<QFile>
#include<QFileDialog>
#include<QFileInfo>
#include<QDebug>
#include<QDateTime>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_ButtonRead_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,"open","../","TXT(*.TXT)");

//    if(!path.isEmpty())
    if(path.isEmpty() == false)
    {
        //文件对象
        QFile file(path);
        //打开方式
        bool isOk = file.open(QIODevice::ReadOnly);
        if(isOk == true)
        {
#if 0
            //读文件,默认只识别UTF-8编码
           QByteArray array = file.readAll();
           //显示到编辑区
           ui->textEdit->setText(array);
#endif
           //一行一行的读
           QByteArray array;
           while (file.atEnd() == false) {
               array += file.readLine();
           }
           ui->textEdit->setText(array);
        }

        file.close();


        QFileInfo info(path);
        qDebug()<<"文件名字："<<info.fileName();
        qDebug()<<"文件后缀："<<info.suffix();
        qDebug()<<"文件大小："<<info.size();
        qDebug()<<"文件创建时间："<<info.created().toString("yyyy-MM-dd hh:mm:ss");
    }


}

void Widget::on_ButtonWrite_clicked()
{
    QString path = QFileDialog::getSaveFileName(this,"save","../","TXT(.*TXT)");
    if(path.isEmpty() == false)
    {
        //创建对象
        QFile file;
        //关联文件名字
        file.setFileName(path);

        //打开文件，只写方式
        bool isOk = file.open(QIODevice::WriteOnly);
        if(isOk == true)
        {
            //获得编辑区内容
            QString str = ui->textEdit->toPlainText();//plaintext:纯文件

            //写文件
            //QString 转化为 QByteArray
//            file.write(str.toUtf8());

            //QString->C++ string ->char *
            file.write(str.toStdString().data());
        }
        file.close();
    }
}
