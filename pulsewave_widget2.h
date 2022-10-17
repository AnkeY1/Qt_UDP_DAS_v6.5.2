#ifndef PULSEWAVE_WIDGET2_H
#define PULSEWAVE_WIDGET2_H

#include <mainwindow.h>
#include <QWidget>
#include "qcustomplot.h"
#include <QMainWindow>
#include <qmainwindow.h>
#include <QTimer>
#include <fstream>
#include <sstream>
#include <mainwindow.h>
#include "udp_recv.h"
#include <ctime>

#define READ_PULSE_LENGTH 1024*32 //��pack_HEX_Display���ȡ�����ݳ���
#define PULSEDATA_ALL_LENGTH 1024*8 //�ĸ�ͨ����ʮ���������ݳ���
#define PULSEDATA_LENGTH 1024*2 //һ��ͨ����ʮ����������
#define DISPLAY_LENGTH_PULSE 256*2 //չʾ��widget�ϵ����ݳ���
#define SaveNumber 65 //�洢�����ĸ���

namespace Ui {
class pulsewave_widget2;
}

using namespace std;

class UDP_Recv;

class pulsewave_widget2 : public QWidget
{
    Q_OBJECT
public:
    pulsewave_widget2(UDP_Recv *udp_Recv);
    ~pulsewave_widget2();

    void initWidget();
    void FlashWave();
    void FreeMemory();

    Ui::pulsewave_widget2 *ui;
    UDP_Recv *udp_recv;
    QCustomPlot *m_customPlot;
    QCPItemText *textLabel;
    QCPItemLine *arrow;
    QCPItemTracer *m_sameTimeTracer;
    QCPItemText *m_sameTimeTextTip;
    int sizeoPulsedata ;
    int sizeoPulsedataDec;
    int sizeoDisplaydata;
    int ChannelIndex=0;
    //HEX����
    char* PulsedataHEX;
    int Pulsedata_DEC_all_HEX[PULSEDATA_ALL_LENGTH*2] =  {0};
    int Pulsedata_DEC_1_HEX[PULSEDATA_LENGTH*2]= {0};
    int Pulsedata_DEC_2_HEX[PULSEDATA_LENGTH*2] =  {0};
    int Pulsedata_DEC_3_HEX[PULSEDATA_LENGTH*2] =  {0};
    int Pulsedata_DEC_4_HEX[PULSEDATA_LENGTH*2] =  {0};
    int Pulsedata_DEC_disp[PULSEDATA_LENGTH*2] = {0};

signals:
    void PauseWave();
    void RestartWave();

private slots:
    void on_btnReset_clicked();
    void on_pushButton_pause_clicked();
    void on_pushButton_restart_clicked();
    void slot_MouseRelease(QMouseEvent*);
};

#endif // PULSEWAVE_WIDGET2_H
