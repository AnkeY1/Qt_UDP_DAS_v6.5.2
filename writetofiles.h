#ifndef WRITETOFILES_H
#define WRITETOFILES_H

#include <QThread>
#include <QDebug>
#include "ui_mainwindow.h"
#include "mainwindow.h"

class  WriteToFiles;
class UDP_Recv;

class WriteToFiles : public QThread
{
public:
    WriteToFiles(UDP_Recv* udp_Recv);
    UDP_Recv* udp_recv;
    bool isTimeUpdate;
    QDateTime dateTime; //��ǰϵͳʱ��
    QString saveFilenameAll;
    QString fileAll;
    ofstream outfileAll;
    bool saveFlag = false;
    qint64 LenoDemo = 4096*10000;
    shared_ptr<CirQueue<unsigned char>> CHdata;
    int peakNum = 0;

    void readConfigFile();


protected:
    //QThread���麯��
    //�̴߳�����
    //����ֱ�ӵ��ã�ͨ��start()��ӵ���
    void run();

signals:

public slots:

};

#endif // WRITETOFILES_H
