#include "demowave_widget.h"
#include "ui_demowave_widget.h"

demowave_widget::demowave_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::demowave_widget)
{
    ui->setupUi(this);

    initWidget();


}

demowave_widget::~demowave_widget()
{
    delete ui;
}

void demowave_widget::initWidget()
{
    // �������������Ტ������ʾ��Χ
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();
    m_axisX->setTitleText("Time Series");
    m_axisY->setTitleText("Phase (rad)");
    m_axisX->setMin(0);
    m_axisY->setMin(-AXIS_MAX_Y);
    m_axisX->setMax(AXIS_MAX_X);
    m_axisY->setMax(AXIS_MAX_Y);
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

//HEX����
void demowave_widget::FlashWave(shared_ptr<CirQueue<float>> DEMOdata)
{
    qDebug() <<"Flash Demodulation Wave Slot responsed !"<<endl;

    m_lineSeries->clear();

    int sizeoDemoData = READ_DEMO_LENGTH;

    //DEMOdata[] split into 4 channels
    for(int k = 0; k<sizeoDemoData; k+=4){
        int p = k/4;
        Demodata_1[p] = DEMOdata->pop();
        Demodata_2[p] = DEMOdata->pop();
        Demodata_3[p] = DEMOdata->pop();
        Demodata_4[p] = DEMOdata->pop();
    }

    //    Channel select
    ChannelIndex = ui->comboBox_Channel->currentIndex();

    switch (ChannelIndex) {
    case 0:{
        for(int i = 0;i<DISPLAY_LENGTH_DEMO; i++)
            m_lineSeries->append(QPointF(i,Demodata_1[i]));
        break;
    }

    case 1:{
        for(int i = 0;i<DISPLAY_LENGTH_DEMO; i++)
            m_lineSeries->append(QPointF(i,Demodata_2[i]));
        break;
    }

    case 2:{
        for(int i = 0;i<DISPLAY_LENGTH_DEMO; i++)
            m_lineSeries->append(QPointF(i,Demodata_3[i]));
        break;
    }

    case 3:{
        for(int i = 0;i<DISPLAY_LENGTH_DEMO; i++)
            m_lineSeries->append(QPointF(i,Demodata_4[i]));
        break;
    }

    default:
        break;
    }

}

void demowave_widget::on_pushButton_reset_clicked()
{
    m_axisX->setMin(0);
    m_axisY->setMin(-AXIS_MAX_Y);
    m_axisX->setMax(AXIS_MAX_X);
    m_axisY->setMax(AXIS_MAX_Y);
}

