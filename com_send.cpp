#include "com_send.h"

COM_Send::COM_Send(MainWindow* Mainwindow)
{
    mainWindow = Mainwindow;

    myCom = new Win_QextSerialPort("COM1",QextSerialBase::EventDriven);

    myCom->setParity(PAR_NONE); //��żУ�����ã���������Ϊ��У��
    myCom->setDataBits(DATA_8);//����λ���ã���������Ϊ8λ����λ
    myCom->setStopBits(STOP_1);//ֹͣλ���ã���������Ϊ1λֹͣλ
    myCom->setPortName("COM3"); //���ö˿ں�
    myCom->setBaudRate(BAUD115200); //���ò�����

    filePath = QDir::currentPath()+QString("/peak.txt"); //build����Ŀ¼��

    if (filePath.isEmpty())
    {
        mainWindow->ui->textEdit_Msg->insertPlainText(QString("Warning! Empty File! ")+"\n");
        return;
    }

    //�ļ�·����ʾ�����Ϳ�
    mainWindow->ui->textEdit_Msg->insertPlainText(QString("Configeration File Directory: ")+filePath+"\n");
}

void COM_Send::run()
{
    //1. ��peak.txt
    configFile = new QFile(filePath);

    if (!configFile->exists())
    {
        mainWindow->ui->textEdit_Msg->insertPlainText(QString("Warning! File is Not Exist! ")+"\n");
        return;
    }

    if (!configFile->open(QIODevice::ReadOnly))
    {
        mainWindow->ui->textEdit_Msg->insertPlainText(QString("Warning! File Open Failed! ")+"\n");
        return;
    }

    //2. ��ȡpeak.txt��peakData
    peakData = configFile->readAll();

    configFile->close();

    if (peakData.isEmpty())
    {
        mainWindow->ui->textEdit_Msg->insertPlainText(QString("Warning! File Content is Empty! ")+"\n");
        return;
    }

    //3. �򿪴���
    myCom->open(QIODevice::ReadWrite);

    if(myCom->isOpen()){
        mainWindow->ui->textEdit_Msg->insertPlainText(QString("Open COM Successful! ")+"\n");

        //4. peakData >> myCom
        myCom->write(peakData);
    }

    else
        mainWindow->ui->textEdit_Msg->insertPlainText(QString("Open COM Failed! ")+"\n");

    qDebug() << "com_send Thread is over " << endl;
}
