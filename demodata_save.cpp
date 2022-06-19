#include "demodata_save.h"

DemoData_Save::DemoData_Save(Demodulation* demodulation)
{
    m_demodulation = demodulation;

    readConfigFile();
}

void DemoData_Save::readConfigFile()
{
    QString filePath = QString("C:/Qt_UDP_DAS/peak.txt");
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the Configration file!"<<endl;
    }
    QByteArray configData = file.readAll(); //��ȡ��������
    char peakNumChar_shiwei = configData[3]; //peakNum��ʮλ����ڵ��ĸ�λ��
    char peakNumChar_gewei = configData[4]; //peakNum�ĸ�λ����ڵ��ĸ�λ��
    bool ok;
    peakNum =  QString(peakNumChar_shiwei).toInt(&ok,16)*10 + QString(peakNumChar_gewei).toInt(&ok,16)*1;
}

void DemoData_Save::run()
{
    /*------------�洢�������DEMOdata������---------------*/
    dateTime = QDateTime::currentDateTime();

    //�ļ�Ŀ¼+[Demo]+[peakNum]+ʱ��.bin
    saveFileDemo = QString("D:/Qt_UDP_DAS/data/")+QString("[Demo][")+QString::number(peakNum)+QString("]")+dateTime.toString("yyyyMMddhhmmss")+ QString(".bin");

    outfileDemo.open(saveFileDemo.toStdString().data(), ofstream::binary);

    if (!outfileDemo.is_open()) return;

    int sizeofDEMOdata = m_demodulation->DEMOdata_save->size();

//    qDebug()<<"sizeofDEMOdata = "<<sizeofDEMOdata<<endl;

    for(int i = 0; i<sizeofDEMOdata; i++){
        outfileDemo.write((const char*)m_demodulation->DEMOdata_save->begin(),sizeof(float));

        //�������Ϊ�գ��ӳ�һ�ᣬ����ȻΪ�գ�˵��û��������
         if(m_demodulation->DEMOdata_save->isEmpty()){
             msleep(50);
             if(m_demodulation->DEMOdata_save->isEmpty())
                 break;
         }

        m_demodulation->DEMOdata_save->pop();
    }

    outfileDemo.close();

    //clear DEMOdata_save
//    m_demodulation->DEMOdata_save->clear();
}
