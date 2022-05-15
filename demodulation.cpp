#include "demodulation.h"

//����udp_recv�µ�CHdata���ɽ��
Demodulation::Demodulation(UDP_Recv* udp_Recv)
{
    //init
    udp_recv = udp_Recv;

    DEMOdata = make_shared<CirQueue<float>>(LenoDemo);

    readAtanTable(atanTable);
}

void Demodulation::run()
{
 /*------------����CHdataArray�����������---------------*/
    for(int i = 0; i< SaveNumber; i++){

        if(udp_recv->CHdataArray[i]->isEmpty())
            continue;
        else
        {
            //1. һ��������CHdata >> demo_CHdata
            unsigned int sizeoCHdata = udp_recv->CHdataArray[i]->size();

            for(unsigned int j=0; j<sizeoCHdata; ++j){
                demo_CHdata[j] = *udp_recv->CHdataArray[i]->begin();
                udp_recv->CHdataArray[i]->pop();
            }

            udp_recv->CHdataArray[i]->clear();

            //2. demo_CHdata[] >> demo_CHdata_DEC_all[]
            for(int i = 0; i<READ_LENGTH; i+=4){
                bool ok;
                int number_DEC = QString(demo_CHdata[i]).toInt(&ok,16)*0 + QString(demo_CHdata[i+1]).toInt(&ok,16)*256 + QString(demo_CHdata[i+2]).toInt(&ok,16)*16 +QString(demo_CHdata[i+3]).toInt(&ok,16)*1;
                if(number_DEC>2047)
                    number_DEC = number_DEC-4096;
                int j = i/4;
                demo_CHdata_DEC_all[j] = number_DEC;
            }

            //3. demo_CHdata_DEC_all[] split into 4 channels
            for(int k = 0; k<CHDATA_ALL_LENGTH; k+=4){
                int p = k/4;
                demo_CHdata_DEC_1[p] = demo_CHdata_DEC_all[k];
                demo_CHdata_DEC_2[p] = demo_CHdata_DEC_all[k+1];
                demo_CHdata_DEC_3[p] = demo_CHdata_DEC_all[k+2];
                demo_CHdata_DEC_4[p] = demo_CHdata_DEC_all[k+3];
            }

            //4. get Vi Vq
            for(int i = 0; i<CHDATA_LENGTH; i++){
                Vi[i] = (float)(demo_CHdata_DEC_1[i] + demo_CHdata_DEC_2[i] - 2 * demo_CHdata_DEC_3[i]);
                Vq[i] = (float)(-sqrt(3) * (demo_CHdata_DEC_1[i] - demo_CHdata_DEC_2[i]));

                //5. ��λ���
                Ph[i]=demoduPh(Vi[i],Vq[i]);

                //6.Ph[] >> DEMOdata
                DEMOdata->push(Ph[i]);
            }

            //7. clear CHdata
             udp_recv->CHdataArray[i]->clear();
        }
    }

/*------------�洢�������DEMOdata������---------------*/
    dateTime = QDateTime::currentDateTime();

    saveFileDemo = QString("F:/Desktop/UDPConnect/data/")+QString("[Demo]")+dateTime.toString("yyyyMMddhhmmss")+ QString(".bin");

    outfileDemo.open(saveFileDemo.toStdString().data(), ofstream::binary);

    if (!outfileDemo.is_open()) return;

    int sizeofDEMOdata = DEMOdata->size();

    for(int i = 0; i<sizeofDEMOdata; i++){
        outfileDemo.write((const char*)DEMOdata->begin(),sizeof(float));
        DEMOdata->pop();
    }

    outfileDemo.close();

/*-------------��ʾ�������--------------*/

    //clear DEMOdata
    DEMOdata->clear();
}

//��ȡ������ֵ����ļ�
void Demodulation::readAtanTable(float *roundNum){
    QString tableFileName;
    tableFileName = QString("atanTable.bin");
    QString path=QString("F:/Desktop/UDPConnect/")+tableFileName;//�ļ���ַ�����Ǳ�ʱ��Ҫ�������á�
    qDebug()<<"atan table file path:"<<path.toStdString().c_str();
    FILE *tableFile=fopen(path.toStdString().c_str(),"rb");
    if(tableFile == NULL)
    {
        char logStr2[64];
        sprintf(logStr2,"Open %s Failed!\n",path.toStdString().c_str());

    }
    fread(roundNum,sizeof(float),201,tableFile);
    fclose(tableFile);

}

//�����������λ���
float Demodulation::demoduPh(float vi,float vq){
    float absVi=abs(vi);
    float absVq=abs(vq);
    float z=0,z0=0,dz=0,ph0=0,ph=0;
    int numZ0=0;


    if(absVi>=absVq){
        if(vi>0){//111 110
            z=-vq/vi;
            z0=(float) (round(z*100.0)/100.0);//0.01�����ķ�����ֵ
            numZ0=(int) (100+z0/0.01);//z0=-1:0.01:1,������Ϊ1��1��201
            dz=(z-z0)/(1+(z*z0));
            ph0=dz+atanTable[numZ0];//���
            ph=ph0;
        }
        else if(vi==0){
            ph=0;
        }
        else {//vi<0  101 100
            z=-vq/vi;
            z0=(float) (round(z*100.0)/100.0);//0.01�����ķ�����ֵ
            numZ0=(int) (100+z0/0.01);//z0=-1:0.01:1,������Ϊ1��1��201
            dz=(z-z0)/(1+(z*z0));
            ph0=dz+atanTable[numZ0];//���
            ph= ( ((-vq)>=0) ? 1: -1 ) *M_PI+ph0;
        }
    }
    else {//absVi<absVq
        if(vq<0){//-vq>0
            z=-vi/vq;
            z0=(float) (round(z*100.0)/100.0);//0.01�����ķ�����ֵ
            numZ0=(int) (100+z0/0.01);//z0=-1:0.01:1,������Ϊ1��1��201
            dz=(z-z0)/(1+(z*z0));
            ph0=dz+atanTable[numZ0];//���
            ph=0.5*M_PI-ph0;
        }
        else {//-vq<0
            z=-vi/vq;
            z0=(float) (round(z*100.0)/100.0);//0.01�����ķ�����ֵ
            numZ0=(int) (100+z0/0.01);//z0=-1:0.01:1,������Ϊ1��1��201
            dz=(z-z0)/(1+(z*z0));
            ph0=dz+atanTable[numZ0];//���
            ph=-0.5*M_PI-ph0;
        }
    }
    return ph;
}
