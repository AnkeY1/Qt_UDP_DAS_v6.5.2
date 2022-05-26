#include "pulsewave_widget.h"
#include "ui_pulsewave_widget.h"

pulsewave_widget::pulsewave_widget()
{
    ui = new Ui::pulsewave_widget;
    ui->setupUi(this);

    //��ʼ��widget
    initWidget();

    ChannelIndex = 0;
}

pulsewave_widget::~pulsewave_widget()
{
    delete ui;
}

void pulsewave_widget::initWidget()
{
    setWindowTitle(QString("Pulse Wave Display"));

    // �������������Ტ������ʾ��Χ
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();
    m_axisX->setTitleText("Time Series");
    m_axisY->setTitleText("Amplitude");
    m_axisX->setMin(0);
    m_axisY->setMin(-200);
    m_axisX->setMax(AXIS_MAX_X);
    m_axisY->setMax(AXIS_MAX_Y);
    m_axisY->setTickCount(15);
    m_axisX->setTickCount(10);

    m_lineSeries = new QSplineSeries();                             // �������߻��ƶ���
    m_lineSeries->setPointsVisible(true);                         // �������ݵ�ɼ�
    m_lineSeries->setName("Pulse Wave Plot");                            // ͼ������
    m_lineSeries->setPen(QPen(Qt::blue,2,Qt::SolidLine));
    m_lineSeries->setUseOpenGL(true);                              //openGL����
    m_lineSeries->clear();

    m_chart = new QChart();                                        // ����ͼ�����
    m_chart->addAxis(m_axisY, Qt::AlignLeft);                      // ��X����ӵ�ͼ����
    m_chart->addAxis(m_axisX, Qt::AlignBottom);                    // ��Y����ӵ�ͼ����
    m_chart->addSeries(m_lineSeries);                              // �����߶�����ӵ�ͼ����
    m_chart->setAnimationOptions(QChart::NoAnimation);        // �ص���������ʹ���߻�����ʾ�ĸ�ƽ��������Ч�����ÿ�

    m_lineSeries->attachAxis(m_axisX);                             // ���߶��������X�ᣬ�˲��������m_chart->addSeries֮��
    m_lineSeries->attachAxis(m_axisY);                             // ���߶��������Y�ᣬ�˲��������m_chart->addSeries֮��

    ui->graphicsView->setChart(m_chart);                           // ��ͼ��������õ�graphicsView�Ͻ�����ʾ
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);       // ������Ⱦ������ݣ������������ô���߾��Եò�ƽ��
    ui->graphicsView->setRubberBand(QChartView::RectangleRubberBand);   //XY����ͬʱ�Ŵ���껭���ľ��δ�С

}

//HEX����
void pulsewave_widget::FlashWave3(char datagramHEX[])
{
    qDebug() <<"Flash Pulse Wave Slot responsed !"<<endl;

    m_lineSeries->clear();

    sizeoPulsedata = READ_PULSE_LENGTH;

    //1. datagramHEX >> PulsedataHEX[]
    memcpy(PulsedataHEX,datagramHEX,sizeoPulsedata);

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

//    4. Channel select
    ChannelIndex = ui->comboBox_Channel->currentIndex();
    switch (ChannelIndex) {
    case 0:
        memcpy(Pulsedata_DEC_disp,Pulsedata_DEC_1_HEX,PULSEDATA_LENGTH*2);
        break;
    case 1:
        memcpy(Pulsedata_DEC_disp,Pulsedata_DEC_2_HEX,PULSEDATA_LENGTH*2);
        break;
    case 2:
        memcpy(Pulsedata_DEC_disp,Pulsedata_DEC_3_HEX,PULSEDATA_LENGTH*2);
        break;
    case 3:
        memcpy(Pulsedata_DEC_disp,Pulsedata_DEC_4_HEX,PULSEDATA_LENGTH*2);
        break;
    default:
        break;
    }

    //5. Wave Display (��8��ʼ ��Ϊ��ʼ֡��ʼ���ְ�������һ֡��8����)
    for(int i = 8;i<DISPLAY_LENGTH_PULSE;i++)
        m_lineSeries->append(QPointF(i-8,Pulsedata_DEC_disp[i]));
}

void pulsewave_widget::on_btnReset_clicked()
{
    m_axisX->setMin(0);
    m_axisY->setMin(-200);
    m_axisX->setMax(AXIS_MAX_X);
    m_axisY->setMax(AXIS_MAX_Y);
}

////ASCII����
//void wave_widget::FlashWave(char datagram[])
//{
//    qDebug() <<"Slot responsed !"<<endl;

//    m_lineSeries->clear();

//    //1. datagram[] >> CHdata[]
//    memcpy(CHdata,datagram,4096);

//    sizeoCHdata = 4096;

//    //2. CHdata >> CHdata_DEC_all[]
//    for(int i = 0; i<sizeoCHdata; i+=4){
//        bool ok;
//        int number_DEC = QString(CHdata[i]).toInt(&ok,16)*0 + QString(CHdata[i+1]).toInt(&ok,16)*256 + QString(CHdata[i+2]).toInt(&ok,16)*16 +QString(CHdata[i+3]).toInt(&ok,16)*1;
//        if(number_DEC>2047)
//            number_DEC = number_DEC-4096;
//        int j = i/4;
//        CHdata_DEC_all[j] = number_DEC;
//    }

//    sizeoCHdataDec = sizeoCHdata/4;

//    //3. CHdata_DEC_all[256] split into 4 channels
//    for(int k = 0; k<sizeoCHdataDec; k+=4){
//        int p = k/4;
//        CHdata_DEC_1[p] = CHdata_DEC_all[k];
//        CHdata_DEC_2[p] = CHdata_DEC_all[k+1];
//        CHdata_DEC_3[p] = CHdata_DEC_all[k+2];
//        CHdata_DEC_4[p] = CHdata_DEC_all[k+3];
//    }

//    //4. ��ʾ����
//    for(int i = 0;i<DISPLAY_LENGTH;i++)
//        m_lineSeries->append(QPointF(i,CHdata_DEC_1[i]));
//}

