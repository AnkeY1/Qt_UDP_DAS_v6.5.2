#include "wave_widget.h"
#include "ui_wave_widget.h"

wave_widget::wave_widget(shared_ptr<CirQueue<unsigned char>> CHdataX)
{
    ui = new Ui::wave_widget;
    ui->setupUi(this);

    CHdataQueue = make_shared<CirQueue<unsigned char>>(LenoUDP);

    CHdataQueue = CHdataX;

    //��ʼ��widget
    initWidget();

}

wave_widget::~wave_widget()
{
    delete ui;
}

void wave_widget::initWidget()
{
    // �������������Ტ������ʾ��Χ
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();
    m_axisX->setTitleText("X-label");
    m_axisY->setTitleText("Y-label");
    m_axisX->setMin(0);
    m_axisY->setMin(-AXIS_MAX_Y);
    m_axisX->setMax(AXIS_MAX_X);
    m_axisY->setMax(AXIS_MAX_Y);

    m_lineSeries = new QSplineSeries();                             // �������߻��ƶ���
    m_lineSeries->setPointsVisible(true);                         // �������ݵ�ɼ�
    m_lineSeries->setName("Wave Plot");                            // ͼ������
    m_lineSeries->setPen(QPen(Qt::blue,2,Qt::SolidLine));
    m_lineSeries->setUseOpenGL(true);                              //openGL����
    m_lineSeries->clear();

    m_chart = new QChart();                                        // ����ͼ�����
    m_chart->addAxis(m_axisY, Qt::AlignLeft);                      // ��X����ӵ�ͼ����
    m_chart->addAxis(m_axisX, Qt::AlignBottom);                    // ��Y����ӵ�ͼ����
    m_chart->addSeries(m_lineSeries);                              // �����߶�����ӵ�ͼ����
    m_chart->setAnimationOptions(QChart::SeriesAnimations);        // ��������ʹ���߻�����ʾ�ĸ�ƽ��������Ч�����ÿ�

    m_lineSeries->attachAxis(m_axisX);                             // ���߶��������X�ᣬ�˲��������m_chart->addSeries֮��
    m_lineSeries->attachAxis(m_axisY);                             // ���߶��������Y�ᣬ�˲��������m_chart->addSeries֮��

    ui->graphicsView->setChart(m_chart);                           // ��ͼ��������õ�graphicsView�Ͻ�����ʾ
//    ui->graphicsView->setRenderHint(QPainter::Antialiasing);       // ������Ⱦ������ݣ������������ô���߾��Եò�ƽ��
    ui->graphicsView->setRubberBand(QChartView::RectangleRubberBand);   //XY����ͬʱ�Ŵ���껭���ľ��δ�С
}

//��ȡ���ز����ļ�
void wave_widget::ReadFromFiles()
{
    //1. ��ȡ�����ļ�
    saveFolder =  QString("F:/Desktop/UDPConnect/data/");

    saveFilenameAll = saveFolder+QString("[All][31]")+QString("20220502140910")+ QString(".bin");

    infileAll.open(saveFilenameAll.toStdString().data(), ifstream::binary);

    if(!infileAll.is_open()) return;

    infileAll.read(CHdata,READ_LENGTH);

    //2. HEX to DEC >> CHdata_DEC_all[256]

    for(int i = 0; i<READ_LENGTH; i+=4){
        bool ok;
        int number_DEC = QString(CHdata[i]).toInt(&ok,16)*0 + QString(CHdata[i+1]).toInt(&ok,16)*256 + QString(CHdata[i+2]).toInt(&ok,16)*16 +QString(CHdata[i+3]).toInt(&ok,16)*1;
        if(number_DEC>2047)
            number_DEC = number_DEC-4096;
        int j = i/4;
        CHdata_DEC_all[j] = number_DEC;
    }

    //3. CHdata_DEC_all[256] split into 4 channels
    for(int k = 0; k<CHDATA_ALL_LENGTH; k+=4){
        int p = k/4;
        CHdata_DEC_1[p] = CHdata_DEC_all[k];
        CHdata_DEC_2[p] = CHdata_DEC_all[k+1];
        CHdata_DEC_3[p] = CHdata_DEC_all[k+2];
        CHdata_DEC_4[p] = CHdata_DEC_all[k+3];
    }
}

//��ȡCHdataQueue
void wave_widget::ReadFromQueue()
{
    //1. CHdataQueue >> CHdata
    sizeoCHdata = CHdataQueue->size();

    for( int j=0; j<sizeoCHdata; ++j){
        unsigned char usCHdata = *CHdataQueue->begin();
        CHdataQueue->pop();
        CHdata[j] = (char)usCHdata;
    }

    //2. CHdata >> CHdata_DEC_all[]
    for(int i = 0; i<sizeoCHdata; i+=4){
        bool ok;
        int number_DEC = QString(CHdata[i]).toInt(&ok,16)*0 + QString(CHdata[i+1]).toInt(&ok,16)*256 + QString(CHdata[i+2]).toInt(&ok,16)*16 +QString(CHdata[i+3]).toInt(&ok,16)*1;
        if(number_DEC>2047)
            number_DEC = number_DEC-4096;
        int j = i/4;
        CHdata_DEC_all[j] = number_DEC;
    }

    sizeoCHdataDec = sizeoCHdata/4;

    //3. CHdata_DEC_all[256] split into 4 channels
    for(int k = 0; k<sizeoCHdataDec; k+=4){
        int p = k/4;
        CHdata_DEC_1[p] = CHdata_DEC_all[k];
        CHdata_DEC_2[p] = CHdata_DEC_all[k+1];
        CHdata_DEC_3[p] = CHdata_DEC_all[k+2];
        CHdata_DEC_4[p] = CHdata_DEC_all[k+3];
    }
}

//ASCII����
void wave_widget::FlashWave(char datagram[])
{
    qDebug() <<"Slot responsed !"<<endl;

    m_lineSeries->clear();

    //1. datagram[] >> CHdata[]
    memcpy(CHdata,datagram,4096);

    sizeoCHdata = 4096;

    //2. CHdata >> CHdata_DEC_all[]
    for(int i = 0; i<sizeoCHdata; i+=4){
        bool ok;
        int number_DEC = QString(CHdata[i]).toInt(&ok,16)*0 + QString(CHdata[i+1]).toInt(&ok,16)*256 + QString(CHdata[i+2]).toInt(&ok,16)*16 +QString(CHdata[i+3]).toInt(&ok,16)*1;
        if(number_DEC>2047)
            number_DEC = number_DEC-4096;
        int j = i/4;
        CHdata_DEC_all[j] = number_DEC;
    }

    sizeoCHdataDec = sizeoCHdata/4;

    //3. CHdata_DEC_all[256] split into 4 channels
    for(int k = 0; k<sizeoCHdataDec; k+=4){
        int p = k/4;
        CHdata_DEC_1[p] = CHdata_DEC_all[k];
        CHdata_DEC_2[p] = CHdata_DEC_all[k+1];
        CHdata_DEC_3[p] = CHdata_DEC_all[k+2];
        CHdata_DEC_4[p] = CHdata_DEC_all[k+3];
    }

    //4. ��ʾ����
    for(int i = 0;i<DISPLAY_LENGTH;i++)
        m_lineSeries->append(QPointF(i,CHdata_DEC_1[i+index]));
}

//HEX����
void wave_widget::FlashWave2(char datagramHEX[])
{
    qDebug() <<"Slot responsed !"<<endl;

    m_lineSeries->clear();

    sizeoCHdata = 2048*32;

    //1. datagramHEX[] >> CHdataHEX[]
    memcpy(CHdataHEX,datagramHEX,sizeoCHdata);

    //2. CHdataHEX[] >> CHdata_DEC_all[]
    for(int i = 0; i<sizeoCHdata; i+=4){
        bool ok;
        int number_DEC = QString(CHdataHEX[i]).toInt(&ok,16)*0 + QString(CHdataHEX[i+1]).toInt(&ok,16)*256 + QString(CHdataHEX[i+2]).toInt(&ok,16)*16 +QString(CHdataHEX[i+3]).toInt(&ok,16)*1;
        if(number_DEC>2047)
            number_DEC = number_DEC-4096;
        int j = i/4;
        CHdata_DEC_all[j] = number_DEC;
    }

    sizeoCHdataDec = sizeoCHdata/4;

    //3. CHdata_DEC_all[256] split into 4 channels
    for(int k = 0; k<sizeoCHdataDec; k+=4){
        int p = k/4;
        CHdata_DEC_1[p] = CHdata_DEC_all[k];
        CHdata_DEC_2[p] = CHdata_DEC_all[k+1];
        CHdata_DEC_3[p] = CHdata_DEC_all[k+2];
        CHdata_DEC_4[p] = CHdata_DEC_all[k+3];
    }

    //4. ��ʾ����
    for(int i = 0;i<DISPLAY_LENGTH;i++)
        m_lineSeries->append(QPointF(i,CHdata_DEC_1[i+index]));
}


void wave_widget::on_btnReset_clicked()
{
    m_axisX->setMin(0);
    m_axisY->setMin(-AXIS_MAX_Y);
    m_axisX->setMax(AXIS_MAX_X);
    m_axisY->setMax(AXIS_MAX_Y);
}
