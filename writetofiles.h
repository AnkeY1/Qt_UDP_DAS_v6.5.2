#ifndef WRITETOFILES_H
#define WRITETOFILES_H

#include <QThread>
#include <QDebug>
#include "ui_mainwindow.h"
#include "mainwindow.h"

class WriteToFiles;
class UDP_Recv;

class WriteToFiles : public QThread
{
public:
    WriteToFiles(UDP_Recv* udp_Recv, int peaknum);
    ~WriteToFiles();
    QString& SaveFilenameAll() {return saveFilenameAll;}
private:
    UDP_Recv* udp_recv;
    QDateTime dateTime; //��ǰϵͳʱ��
    QString saveFilenameAll;
    QString fileAll;
    ofstream outfileAll;
    bool saveFlag;
    int peakNum;


protected:
    //QThread���麯��
    //�̴߳�����
    //����ֱ�ӵ��ã�ͨ��start()��ӵ���
    void run();

signals:

public slots:

};

#endif // WRITETOFILES_H
