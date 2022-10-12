#include "pulsewave_widget2.h"
#include "ui_pulsewave_widget2.h"

pulsewave_widget2::pulsewave_widget2(UDP_Recv *udp_Recv):
    ui( new Ui::pulsewave_widget2),
    udp_recv(udp_Recv)
{
    ui->setupUi(this);

    PulsedataHEX = new char[READ_PULSE_LENGTH*2];

    initWidget();
}

pulsewave_widget2::~pulsewave_widget2()
{
    FreeMemory();
}

void pulsewave_widget2::initWidget()
{
    setWindowTitle(QString("Pulse Wave Display"));

    m_customPlot = ui->customPlot;

    //ÿ�����߶����ռһ��graph()
    m_customPlot->addGraph();

    //���û���QPen
    QPen pen;
    pen.setWidth(4);//�߿�
    pen.setStyle(Qt::PenStyle::SolidLine);//ʵ��
    pen.setColor(Qt::blue); //��ɫ

    //��ͼ����ӻ���
    m_customPlot->graph(0)->setPen(pen);
    m_customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // ������X���Χ������ɫ

    m_customPlot->xAxis->setRangeLower(0);
    m_customPlot->xAxis->setRangeUpper(2000);
    m_customPlot->yAxis->setRangeLower(-200);
    m_customPlot->yAxis->setRangeUpper(2048);

    // �߿��Ҳ���ϲ����ʾ�̶��ߣ�������ʾ�̶�ֵ:
    // (�μ� QCPAxisRect::setupFullAxesBox for a quicker method to do this)
    m_customPlot->xAxis2->setVisible(true);
    m_customPlot->xAxis2->setTickLabels(false);
    m_customPlot->yAxis2->setVisible(true);
    m_customPlot->yAxis2->setTickLabels(false);
    // ʹ��������X��ķ�Χ������ȣ�ʹ��������Y��ķ�Χ�������
    //    connect(m_customPlot->xAxis, &QCPAxis::rangeChanged(QCPRange), m_customPlot->xAxis2, &QCPAxis::setRange(QCPRange));
    //    connect(m_customPlot->yAxis, &QCPAxis::rangeChanged(QCPRange), m_customPlot->yAxis2, &QCPAxis::setRange(QCPRange));


    // ֧�������ק��ķ�Χ������������ķ�Χ�������ѡͼ�㣨ÿ�����߶�ռһ��ͼ�㣩
    m_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    //��ѡ�Ŵ�
    m_customPlot->selectionRect()->setPen(QPen(Qt::black,1,Qt::DashLine));//����ѡ�����ʽ������
    m_customPlot->selectionRect()->setBrush(QBrush(QColor(0,0,100,50)));//����ѡ�����ʽ����͸��ǳ��
    m_customPlot->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);

    //ȷ��������ľ���
    ui->customPlot->setSelectionTolerance(1);

    //��ǵ��źŲ�
    connect(ui->customPlot,&QCustomPlot::mouseRelease, this, &pulsewave_widget2::slot_MouseRelease);

}

void pulsewave_widget2::FlashWave()
{
    qDebug() <<"Flash Pulse Wave Slot responsed !"<<endl;

    //clear history data
    m_customPlot->graph(0)->data()->clear();

    sizeoPulsedata = READ_PULSE_LENGTH*2;

    //1. pack_HEX_Display >> PulsedataHEX[]
    memcpy(PulsedataHEX,udp_recv->pack_HEX_Display,sizeof(char)*sizeoPulsedata);

    //2. PulsedataHEX[] >> Pulsedata_DEC_all[]
    for(int i = 0; i<sizeoPulsedata; i+=4){
        bool ok;
        int number_DEC = QString(PulsedataHEX[i]).toInt(&ok,16)*0 + QString(PulsedataHEX[i+1]).toInt(&ok,16)*256 + QString(PulsedataHEX[i+2]).toInt(&ok,16)*16 +QString(PulsedataHEX[i+3]).toInt(&ok,16)*1;
        if(number_DEC>2047)
            number_DEC = number_DEC-4096;
        int j = i/4;
        Pulsedata_DEC_all_HEX[j] = number_DEC;
    }

    sizeoPulsedataDec = sizeoPulsedata/4;

    //3. Pulsedata_DEC_all[] split into 4 channels
    for(int k = 0; k<sizeoPulsedataDec; k+=4){
        int p = k/4;
        Pulsedata_DEC_1_HEX[p] = Pulsedata_DEC_all_HEX[k];
        Pulsedata_DEC_2_HEX[p] = Pulsedata_DEC_all_HEX[k+1];
        Pulsedata_DEC_3_HEX[p] = Pulsedata_DEC_all_HEX[k+2];
        Pulsedata_DEC_4_HEX[p] = Pulsedata_DEC_all_HEX[k+3];
    }

    sizeoDisplaydata = sizeoPulsedataDec/4;

    //4. Channel select
    ChannelIndex = ui->comboBox_Channel->currentIndex();
    switch (ChannelIndex) {
    case 0:
//        copy(begin(Pulsedata_DEC_1_HEX),end(Pulsedata_DEC_1_HEX),begin(Pulsedata_DEC_disp));
        memcpy(Pulsedata_DEC_disp,Pulsedata_DEC_1_HEX,sizeof(int)*sizeoDisplaydata);
        break;
    case 1:
//        copy(begin(Pulsedata_DEC_2_HEX),end(Pulsedata_DEC_2_HEX),begin(Pulsedata_DEC_disp));
        memcpy(Pulsedata_DEC_disp,Pulsedata_DEC_2_HEX,sizeof(int)*sizeoDisplaydata);
        break;
    case 2:
//        copy(begin(Pulsedata_DEC_3_HEX),end(Pulsedata_DEC_3_HEX),begin(Pulsedata_DEC_disp));
        memcpy(Pulsedata_DEC_disp,Pulsedata_DEC_3_HEX,sizeof(int)*sizeoDisplaydata);
        break;
    case 3:
//        copy(begin(Pulsedata_DEC_4_HEX),end(Pulsedata_DEC_4_HEX),begin(Pulsedata_DEC_disp));
        memcpy(Pulsedata_DEC_disp,Pulsedata_DEC_4_HEX,sizeof(int)*sizeoDisplaydata);
        break;
    default:
        break;
    }

    //5. Wave Display /* (��8��ʼ ��Ϊ��ʼ֡��ʼ���ְ�������һ֡��8����)*/
    for(int i = 0;i<sizeoDisplaydata-1;i++){
        QVector<double> x(1),y(1);
        x[0] = i/*-8*/;
        y[0]= Pulsedata_DEC_disp[i];
        if(y[0]<-10) y[0]=0; //ȥ���·��ĵ�������
        if(Pulsedata_DEC_disp[i]>0 && Pulsedata_DEC_disp[i-1]==0 && Pulsedata_DEC_disp[i+1]==0) y[0]=0; //ȥ���Ϸ��ĵ�������
        m_customPlot->graph(0)->addData(x, y);
    }

    ui->customPlot->replot();
}

void pulsewave_widget2::FreeMemory()
{
    delete[] PulsedataHEX;
}

void pulsewave_widget2::on_btnReset_clicked()
{
    //�Զ�����XY��ķ�Χ���Ա���ʾ��graph(0)�����еĵ�
    m_customPlot->graph(0)->rescaleAxes(true);

//    m_customPlot->xAxis->setRangeLower(0);
//    m_customPlot->xAxis->setRangeUpper(2000);
    m_customPlot->yAxis->setRangeLower(-10);
    m_customPlot->yAxis->setRangeUpper(2048);

    // ����ˢ��ͼ��
    ui->customPlot->replot();
}

void pulsewave_widget2::on_pushButton_pause_clicked()
{
    emit PauseWave();
}

void pulsewave_widget2::on_pushButton_restart_clicked()
{
    emit RestartWave();
}

//��ȡ���ѡ�е����ֵ
void pulsewave_widget2::slot_MouseRelease(QMouseEvent *e)
{
    //�ų���������
        if (e->button() != Qt::LeftButton){ return; }

    //��ȡ����ĵ�����
        QPointF ChickedPoint = e->pos();
    //�ų�����������
        if(!ui->customPlot->viewport().contains(e->pos())){return;}
    //����������ת��Ϊ��ֵ
         double currentx = ui->customPlot->xAxis->pixelToCoord(ChickedPoint.x());
         double currenty = ui->customPlot->yAxis->pixelToCoord(ChickedPoint.y());
    //ʹ��QToolTip���ֵ��
        QToolTip::showText(mapToGlobal(e->pos()),QString("x:%1, y:%2").arg(currentx).arg(currenty),this);


}

