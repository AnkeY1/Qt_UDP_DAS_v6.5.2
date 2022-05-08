#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <winsock2.h>
#include <QQueue>
#include "CirQueue.h"
#include <memory>
#include <fstream>
#include <sstream>
#include <QDateTime>
#include <QTime>
#include <QTimer>
#include <QtNetwork>
#include <recvdata.h>
#include <QtDebug>
#include <vector>
#include <windows.h>
#include "writetofiles.h"
#include "udp_recv.h"
#include "wave_widget.h"
#include "demodulation.h"
#include "com_send.h"
#include "ui_mainwindow.h"


typedef unsigned char BYTE;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

 //ʹ��ǰ������
class WriteToFiles;
class UDP_Recv;
class wave_widget;
class Demodulation;
class COM_Send;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow *ui;

    QTimer* udpTimer;
    UDP_Recv* udp_recv;
    WriteToFiles* writeToFiles;
    wave_widget *wave_Widget;
    Demodulation *demodu;
    COM_Send *com_send;
    bool isSave;
    bool isStart;
    bool isASCII;
    bool isHEX;
    int AcqMode; //�ɼ���ģʽѡ��

    void setLocalStyleSheet();
    void setLocalMsg();
    void OpenWriteToFilesThread();
    void stopThread();


private slots:

    void FinishUDP_RecvThread();
    void FinishWriteToFilesThread();
    void on_pushButton_Start_clicked();
    void on_pushButton_Stop_clicked();

    void on_pushButton_Clear_clicked();

    void on_checkBox_Save_clicked();

    void on_checkBox_ASCII_clicked();

    void on_checkBox_Hex_clicked();

    void on_pushButton_Display_clicked();

    void on_checkBox_Demo_clicked();

    void on_comboBox_Mode_currentIndexChangedSlot();

    void on_pushButton_Send_clicked();

private:
    bool isTimeUpdate;

};
#endif // MAINWINDOW_H
