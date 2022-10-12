#include "writetofiles.h"

WriteToFiles::WriteToFiles(UDP_Recv* udp_Recv, int peaknum):
    udp_recv(udp_Recv),
    saveFlag(false),
    peakNum(peaknum)
{

}

/*----------------����ģʽ���е���ͨ��ԭʼ����----------------*/
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
                     msleep(10);
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


