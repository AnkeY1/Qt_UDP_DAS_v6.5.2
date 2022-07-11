#include "writetofiles.h"

WriteToFiles::WriteToFiles(UDP_Recv* udp_Recv)
{
    udp_recv = udp_Recv;

    CHdata = make_shared<CirQueue<unsigned char>>(udp_recv->LenoUDP);

    readConfigFile();
}

void WriteToFiles::readConfigFile()
{
    QString filePath = QString("C:/Qt_UDP_DAS/peak.txt"); //build����Ŀ¼��
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the Configration file!"<<endl;
    }
    QByteArray configData = file.readAll(); //��ȡ��������
    char peakNumChar_3 = configData[3]; //peakNum�İ�λ����ڵ��ĸ�λ��
    char peakNumChar_4 = configData[4]; //peakNum��ʮλ����ڵ����λ��
    char peakNumChar_5 = configData[5]; //peakNum�ĸ�λ����ڵ�����λ��
    char peakNumChar_6 = configData[6]; //���з�
//    peakNum = 48;


    //peakNum�Ǹ�λ��
    if(peakNumChar_4 == '\n'){
        bool ok;
        peakNum = QString(peakNumChar_3).toInt(&ok,16)*1;
    }
    //peakNum��ʮλ��
    else if(peakNumChar_5 == '\n'){
        bool ok;
        peakNum = QString(peakNumChar_3).toInt(&ok,16)*10 + QString(peakNumChar_4).toInt(&ok,16)*1;
    }
    //peakNum�ǰ�λ��
    else if(peakNumChar_6 == '\n'){
        bool ok;
        peakNum = QString(peakNumChar_3).toInt(&ok,16)*100 + QString(peakNumChar_4).toInt(&ok,16)*10 + QString(peakNumChar_5).toInt(&ok,16)*1;
    }
}

void WriteToFiles::run()
{
    qDebug() << "TimeUpdate" << endl;

    dateTime = QDateTime::currentDateTime();

     //�ļ�Ŀ¼+[4CH]+[peakNum]+ʱ��.bin
    saveFilenameAll = QString("D:/Qt_UDP_DAS/data/")+QString("[4CH][")+QString::number(peakNum)+QString("]")+dateTime.toString("yyyyMMddhhmmss")+ QString(".bin");

    outfileAll.open(saveFilenameAll.toStdString().data(), ofstream::binary);

    if (!outfileAll.is_open()) return;

    for(int i = 0; i< SaveNumber; i++){

        if(udp_recv->CHdataArray[i]->isEmpty())
            break;
        else
        {
            unsigned int sizeoCHdata = udp_recv->CHdataArray[i]->size();

            for(unsigned int j=0; j<sizeoCHdata; ++j){
                outfileAll.write((const char*)udp_recv->CHdataArray[i]->begin(),sizeof(unsigned char));

                //�������Ϊ�գ��ӳ�һ�ᣬ����ȻΪ�գ�˵��û��������
                 if(udp_recv->CHdataArray[i]->isEmpty()){
                     msleep(50);
                     if(udp_recv->CHdataArray[i]->isEmpty())
                         break;
                 }

                udp_recv->CHdataArray[i]->pop();
            }

        }
    }

    outfileAll.close();

    qDebug()<< "WriteToFiles Thread is Finished ! "<<endl;
    qDebug()<<"-----------------------------------"<<endl;
}


