#include "demowave_widget.h"
#include "ui_demowave_widget.h"

demowave_widget::demowave_widget(Demodulation *demodulation) :
    ui(new Ui::demowave_widget)
{
    ui->setupUi(this);
    demodu = demodulation;

    initWidget();

    readConfigFile();

    initComboBox_Region();

//    Demodata_1 = new float[REGION_DATA_LENGTH]();
//    Demodata_2 = new float[REGION_DATA_LENGTH]();
//    Demodata_3 = new float[REGION_DATA_LENGTH]();
//    Demodata_4 = new float[REGION_DATA_LENGTH]();
//    Demodata_5 = new float[REGION_DATA_LENGTH]();
//    Demodata_6 = new float[REGION_DATA_LENGTH]();
//    Demodata_7 = new float[REGION_DATA_LENGTH]();
//    Demodata_8 = new float[REGION_DATA_LENGTH]();
//    Demodata_9 = new float[REGION_DATA_LENGTH]();

    DemodataArray = new float*[peakNum]; //��̬������ά����
    DemodataArray[0]=Demodata_1;
    DemodataArray[1]=Demodata_2;
    DemodataArray[2]=Demodata_3;
    DemodataArray[3]=Demodata_4;
    DemodataArray[4]=Demodata_5;
    DemodataArray[5]=Demodata_6;
    DemodataArray[6]=Demodata_7;
    DemodataArray[7]=Demodata_8;
    DemodataArray[8]=Demodata_9;
    DemodataArray[9]=Demodata_10;
}

demowave_widget::~demowave_widget()
{
    delete ui;
}

void demowave_widget::initWidget()
{
    setWindowTitle(QString("Demodulation Wave Display"));

    // �������������Ტ������ʾ��Χ
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();
    m_axisX->setTitleText("Time Series");
    m_axisY->setTitleText("Phase (rad)");
    m_axisX->setMin(0);
//    m_axisY->setMin(-AXIS_MAX_Y);
    m_axisX->setMax(AXIS_MAX_X);
//    m_axisY->setMax(AXIS_MAX_Y);
    m_axisY->setTickCount(10);
    m_axisX->setTickCount(10);

    m_lineSeries = new QSplineSeries();                             // �������߻��ƶ���
    m_lineSeries->setPointsVisible(true);                         // �������ݵ�ɼ�
    m_lineSeries->setName("Demodulation Wave Plot");                            // ͼ������
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

void demowave_widget::readConfigFile()
{
    QString filePath = QString("C:/Qt_UDP_DAS/peak.txt");
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the Configration file!"<<endl;
    }
   QByteArray configData = file.readAll(); //��ȡ��������
   char peakNumChar = configData[3]; //peakNum����ڵ��ĸ�λ��
   bool ok;
   peakNum =  QString(peakNumChar).toInt(&ok,16);
}

void demowave_widget::initComboBox_Region()
{
    ui->comboBox_Region->clear();

    for(int i=1; i<= peakNum; i++ )
        ui->comboBox_Region->addItem(QString::asprintf("Region %d",i));
}

//HEX����ʱˢ�²�����ʾ
void demowave_widget::FlashWave()
{
    qDebug() <<"Flash Demodulation Wave Slot responsed !"<<endl;

    m_lineSeries->clear();

    int regionNum = peakNum;

    int sizeoDemoData = demodu->DEMOdata_flash->size();

    //����sizeoDemoData�ĳ��ȣ�ʹ��ΪregionNum�ı���
    int N1 = sizeoDemoData/regionNum;
    sizeoDemoData = N1*peakNum;

    //DEMOdata[] split regions
    for(int k = 0; k<sizeoDemoData; k+=regionNum){
        int p = k/regionNum;
        for(int q=0; q<regionNum;q++){
            if(demodu->DEMOdata_flash->isEmpty()) QThread::msleep(50);
            DemodataArray[q][p] = demodu->DEMOdata_flash->pop();
        }
    }

    //region select
    RegionIndex = ui->comboBox_Region->currentIndex();

    //dispaly wave
    for(int i = 0;i<DISPLAY_LENGTH_DEMO; i++){
        m_lineSeries->append(QPointF(i,DemodataArray[RegionIndex][i]));
        m_axisY->setRange(DemodataArray[RegionIndex][i]-15,DemodataArray[RegionIndex][i]+15);
    }

}

void demowave_widget::on_pushButton_reset_clicked()
{
    m_axisX->setMin(0);
    m_axisY->setMin(-AXIS_MAX_Y);
    m_axisX->setMax(AXIS_MAX_X);
    m_axisY->setMax(AXIS_MAX_Y);

}

