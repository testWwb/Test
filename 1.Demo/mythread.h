#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>

class myThread : public QObject
{
    Q_OBJECT
public:
    explicit myThread(QObject *parent = 0);

    void myTimeout();
    void setFlag(bool flag = true);

signals:
    void mySignal();

public slots:

private:
    bool isStop;
};

#endif // MYTHREAD_H
