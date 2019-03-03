#include "mythread.h"
#include<QDebug>
#include<QThread>

myThread::myThread(QObject *parent) : QObject(parent)
{

}

void myThread::myTimeout()
{
    while (isStop == false) {
        QThread::sleep(1);
        emit mySignal();

        qDebug()<<"子线程号："<<QThread::currentThread();
        if(true == isStop)
        {
            break;
        }
    }

}

void myThread::setFlag(bool flag)
{
    isStop = flag;
}
