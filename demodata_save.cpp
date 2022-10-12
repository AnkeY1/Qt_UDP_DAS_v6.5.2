#include "demodata_save.h"

DemoData_Save::DemoData_Save(Demodulation* demodulation)
    :m_demodulation(demodulation),
    peakNum(m_demodulation->peakNum)
{

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
             msleep(10);
             if(m_demodulation->DEMOdata_save->isEmpty())
                 break;
         }

        m_demodulation->DEMOdata_save->pop();
    }

    outfileDemo.close();

    //clear DEMOdata_save
//    m_demodulation->DEMOdata_save->clear();
}
