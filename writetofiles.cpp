#include "writetofiles.h"

WriteToFiles::WriteToFiles(UDP_Recv* udp_Recv)
{
    udp_recv = udp_Recv;

    CHdata = make_shared<CirQueue<unsigned char>>(udp_recv->LenoUDP);

    readConfigFile();
}

void WriteToFiles::readConfigFile()
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

void WriteToFiles::run()
{
    qDebug() << "TimeUpdate" << endl;

    dateTime = QDateTime::currentDateTime();

     //�ļ�Ŀ¼+[4CH]+[peakNum]+ʱ��.bin
    saveFilenameAll = QString("F:/Desktop/UDPConnect/data/")+QString("[4CH][")+QString::number(peakNum)+QString("]")+dateTime.toString("yyyyMMddhhmmss")+ QString(".bin");

    outfileAll.open(saveFilenameAll.toStdString().data(), ofstream::binary);

    if (!outfileAll.is_open()) return;

    for(int i = 0; i< SaveNumber; i++){

        if(udp_recv->CHdataArray[i]->isEmpty())
            continue;
        else
        {
            unsigned int sizeoCHdata = udp_recv->CHdataArray[i]->size();

            for(unsigned int j=0; j<sizeoCHdata; ++j){
                outfileAll.write((const char*)udp_recv->CHdataArray[i]->begin(),sizeof(unsigned char));
                udp_recv->CHdataArray[i]->pop();
            }
            udp_recv->CHdataArray[i]->clear();
        }
    }

    outfileAll.close();

    qDebug()<< "WriteToFiles Thread is Finished ! "<<endl;
    qDebug()<<"-----------------------------------"<<endl;
}


