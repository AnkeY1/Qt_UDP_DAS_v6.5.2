#ifndef DEMODULATION_H
#define DEMODULATION_H

#include <QThread>
#include <mainwindow.h>

#define READ_LENGTH 1024*32 //��p_echo_pack_HEX���ȡ�����ݳ���
#define CHDATA_ALL_LENGTH 1024*8 //�ĸ�ͨ����ʮ���������ݳ���
#define CHDATA_LENGTH 1024*2 //һ��ͨ����ʮ����������
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
    qint64 LenoDemo = 4096*10000;

    void readAtanTable(float *roundNum);
    float demoduPh(float vi,float vq);

signals:

protected:
    void run();

};

#endif // DEMODULATION_H
