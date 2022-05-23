#include "demodata_save.h"

DemoData_Save::DemoData_Save(Demodulation* demodulation)
{
    m_demodulation = demodulation;

    readConfigFile();
}

void DemoData_Save::readConfigFile()
{
    QString filePath = QDir::currentPath()+QString("/peak.txt"); //build����Ŀ¼��
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the Configration file!"<<endl;
    }
    QByteArray configData = file.readAll(); //��ȡ��������
    char peakNumChar = configData[3]; //peakNum����ڵ��ĸ�λ��
    bool ok;
    peakNum =  QString(peakNumChar).toInt(&ok,16);
}

void DemoData_Save::run()
{
    /*------------�洢�������DEMOdata������---------------*/
    dateTime = QDateTime::currentDateTime();

    //�ļ�Ŀ¼+[Demo]+[peakNum]+ʱ��.bin
    saveFileDemo = QString("F:/Desktop/Qt_UDP_DAS/data/")+QString("[Demo][")+QString::number(peakNum)+QString("]")+dateTime.toString("yyyyMMddhhmmss")+ QString(".bin");

    outfileDemo.open(saveFileDemo.toStdString().data(), ofstream::binary);

    if (!outfileDemo.is_open()) return;

    int sizeofDEMOdata = m_demodulation->DEMOdata_save->size();

    qDebug()<<"sizeofDEMOdata = "<<sizeofDEMOdata<<endl;

    for(unsigned int i = 0; i<sizeofDEMOdata/sizeof(float); i++){ //����һ��д���ĸ��ֽڣ����д�����Ҫ/4
        outfileDemo.write((const char*)m_demodulation->DEMOdata_save->begin(),sizeof(float));
        m_demodulation->DEMOdata_save->pop();
    }

    outfileDemo.close();

    //clear DEMOdata_save
    m_demodulation->DEMOdata_save->clear();
}
