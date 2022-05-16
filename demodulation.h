#ifndef DEMODULATION_H
#define DEMODULATION_H

#include <QThread>
#include <mainwindow.h>

#define READ_LENGTH 4096*1000 //��CHdata���ȡ�����ݳ���
#define CHDATA_ALL_LENGTH 1024*1000 //�ĸ�ͨ����ʮ���������ݳ���
#define CHDATA_LENGTH 256*1000 //һ��ͨ����ʮ����������
#define NUMTABLE 201

class Demodulation : public QThread
{
    Q_OBJECT
public:
    explicit Demodulation(UDP_Recv* udp_Recv);

    UDP_Recv* udp_recv;
    shared_ptr<CirQueue<float>> DEMOdata;
    unsigned char demo_CHdata[READ_LENGTH]= {'\0'};
    float Vi[CHDATA_LENGTH] = {0};
    float Vq[CHDATA_LENGTH]= {0};
    float Ph[CHDATA_LENGTH]= {0};
    float atanTable[NUMTABLE]= {0};
    int sizeoCHdata;
    int demo_CHdata_DEC_all[CHDATA_ALL_LENGTH]= {0};
    int sizeoCHdataDec;
    int demo_CHdata_DEC_1[CHDATA_LENGTH]= {0};
    int demo_CHdata_DEC_2[CHDATA_LENGTH]= {0};
    int demo_CHdata_DEC_3[CHDATA_LENGTH]= {0};
    int demo_CHdata_DEC_4[CHDATA_LENGTH]= {0};
    int sizeoDemoCHdata;
    QDateTime dateTime; //��ǰϵͳʱ��
    QString saveFileDemo;
    ofstream outfileDemo;
    qint64 LenoDemo = READ_LENGTH;

    void readAtanTable(float *roundNum);
    float demoduPh(float vi,float vq);

signals:
    void sendToDemoWave_widget(shared_ptr<CirQueue<float>>);

protected:
    void run();

};

#endif // DEMODULATION_H
