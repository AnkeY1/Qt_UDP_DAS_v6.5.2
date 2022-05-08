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
    string saveFolder;//�洢Ŀ¼���ļ��У�
    QString saveFilename1; //CH1�洢�ļ���
    QString saveFilename2;
    QString saveFilename3;
    QString saveFilename4;
    QString saveFilenameAll;
    QString saveFileDemo;
    QString file1;
    QString file2;
    QString file3;
    QString file4;
    QString fileAll;
    ofstream outfile1;//CH1
    ofstream outfile2;
    ofstream outfile3;
    ofstream outfile4;
    ofstream outfileAll;
    ofstream outfileDemo;
    bool saveFlag = false;
    bool demoFlag = false;
    int PeakNum=31;
    qint64 LenoDemo = 4096*1000;
    shared_ptr<CirQueue<unsigned char>> CHdata;
    shared_ptr<CirQueue<int>> Phdata;

protected:
    //QThread���麯��
    //�̴߳�����
    //����ֱ�ӵ��ã�ͨ��start()��ӵ���
    void run();

signals:

public slots:
    void recvPhSlot(int Ph[]);

};

#endif // WRITETOFILES_H
