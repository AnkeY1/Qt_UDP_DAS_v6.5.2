#include "demowave_widget2.h"
#include "ui_demowave_widget2.h"

QSharedPointer<QCPAxisTickerText> xTicker(new QCPAxisTickerText);
QSharedPointer<QCPAxisTickerText> yTicker(new QCPAxisTickerText);

demowave_widget2::demowave_widget2(Demodulation *demodulation):
    ui(new Ui::demowave_widget2),
    demodu(demodulation),
    RegionIndex(0),
    peakNum(demodu->peakNum)
{
    ui->setupUi(this);

    initWidget();

    initComboBox_Region();

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
    DemodataArray[10]=Demodata_11;//region11
    DemodataArray[11]=Demodata_12;
    DemodataArray[12]=Demodata_13;
    DemodataArray[13]=Demodata_14;
    DemodataArray[14]=Demodata_15;
    DemodataArray[15]=Demodata_16;
    DemodataArray[16]=Demodata_17;
    DemodataArray[17]=Demodata_18;
    DemodataArray[18]=Demodata_19;
    DemodataArray[19]=Demodata_20;
    DemodataArray[20]=Demodata_21;//region21
    DemodataArray[21]=Demodata_22;
    DemodataArray[22]=Demodata_23;
    DemodataArray[23]=Demodata_24;
    DemodataArray[24]=Demodata_25;
    DemodataArray[25]=Demodata_26;
    DemodataArray[26]=Demodata_27;
    DemodataArray[27]=Demodata_28;
    DemodataArray[28]=Demodata_29;
    DemodataArray[29]=Demodata_30;
    DemodataArray[30]=Demodata_31;//region31
    DemodataArray[31]=Demodata_32;
    DemodataArray[32]=Demodata_33;
    DemodataArray[33]=Demodata_34;
    DemodataArray[34]=Demodata_35;
    DemodataArray[35]=Demodata_36;
    DemodataArray[36]=Demodata_37;
    DemodataArray[37]=Demodata_38;
    DemodataArray[38]=Demodata_39;
    DemodataArray[39]=Demodata_40;
    DemodataArray[40]=Demodata_41;//region41
    DemodataArray[41]=Demodata_42;
    DemodataArray[42]=Demodata_43;
    DemodataArray[43]=Demodata_44;
    DemodataArray[44]=Demodata_45;
    DemodataArray[45]=Demodata_46;
    DemodataArray[46]=Demodata_47;
    DemodataArray[47]=Demodata_48;
    DemodataArray[48]=Demodata_49;
    DemodataArray[49]=Demodata_50;
    DemodataArray[50]=Demodata_51;//region51
    DemodataArray[51]=Demodata_52;
    DemodataArray[52]=Demodata_53;
    DemodataArray[53]=Demodata_54;
    DemodataArray[54]=Demodata_55;
    DemodataArray[55]=Demodata_56;
    DemodataArray[56]=Demodata_57;
    DemodataArray[57]=Demodata_58;
    DemodataArray[58]=Demodata_59;
    DemodataArray[59]=Demodata_60;
    DemodataArray[60]=Demodata_61;//region61
    DemodataArray[61]=Demodata_62;
    DemodataArray[62]=Demodata_63;
    DemodataArray[63]=Demodata_64;
    DemodataArray[64]=Demodata_65;
    DemodataArray[65]=Demodata_66;
    DemodataArray[66]=Demodata_67;
    DemodataArray[67]=Demodata_68;
    DemodataArray[68]=Demodata_69;
    DemodataArray[69]=Demodata_70;
    DemodataArray[70]=Demodata_71;//region71
    DemodataArray[71]=Demodata_72;
    DemodataArray[72]=Demodata_73;
    DemodataArray[73]=Demodata_74;
    DemodataArray[74]=Demodata_75;
    DemodataArray[75]=Demodata_76;
    DemodataArray[76]=Demodata_77;
    DemodataArray[77]=Demodata_78;
    DemodataArray[78]=Demodata_79;
    DemodataArray[79]=Demodata_80;
    DemodataArray[80]=Demodata_81;//region81
    DemodataArray[81]=Demodata_82;
    DemodataArray[82]=Demodata_83;
    DemodataArray[83]=Demodata_84;
    DemodataArray[84]=Demodata_85;
    DemodataArray[85]=Demodata_86;
    DemodataArray[86]=Demodata_87;
    DemodataArray[87]=Demodata_88;
    DemodataArray[88]=Demodata_89;
    DemodataArray[89]=Demodata_90;
    DemodataArray[90]=Demodata_91;//region91
    DemodataArray[91]=Demodata_92;
    DemodataArray[92]=Demodata_93;
    DemodataArray[93]=Demodata_94;
    DemodataArray[94]=Demodata_95;
    DemodataArray[95]=Demodata_96;
    DemodataArray[96]=Demodata_97;
    DemodataArray[97]=Demodata_98;
    DemodataArray[98]=Demodata_99;
    DemodataArray[99]=Demodata_100;//region100
}

demowave_widget2::~demowave_widget2()
{
    delete ui;
    delete[] DemodataArray;
}

void demowave_widget2::initWidget()
{
    setWindowTitle(QString("Demodulation Wave Display"));


    /*--------------------------���������ʾ-------------------------------*/
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

    /*---------------------------�ٲ�ͼ-----------------------------------*/
    m_heatmapPlot = ui->HeatmapPlot;

    heatmap = new QCPColorMap(m_heatmapPlot->xAxis,m_heatmapPlot->yAxis);

    //���������ʾ������ʹ��������
    xTicker->setSubTickCount(1);
    yTicker->setSubTickCount(1);
    m_heatmapPlot->xAxis->setTicker(xTicker);
    m_heatmapPlot->yAxis->setTicker(yTicker);
    m_heatmapPlot->xAxis->grid()->setPen(Qt::NoPen);
    m_heatmapPlot->yAxis->grid()->setPen(Qt::NoPen);
    m_heatmapPlot->xAxis->grid()->setSubGridVisible(true);
    m_heatmapPlot->yAxis->grid()->setSubGridVisible(true);
    m_heatmapPlot->xAxis->setSubTicks(true);
    m_heatmapPlot->yAxis->setSubTicks(true);
    m_heatmapPlot->xAxis->setTickLength(0);
    m_heatmapPlot->yAxis->setTickLength(0);
    m_heatmapPlot->xAxis->setSubTickLength(6);
    m_heatmapPlot->yAxis->setSubTickLength(6);

    QCPColorScale *colorScale = new QCPColorScale(m_heatmapPlot);  // ����һ��ɫ��
    colorScale->setType(QCPAxis::atBottom);   // ˮƽ��ʾ
    m_heatmapPlot->plotLayout()->addElement(1, 0, colorScale); // ����ɫͼ������ʾ
    heatmap->setColorScale(colorScale);
    QCPColorGradient gradient;  // ɫ��ʹ�õ���ɫ����
    gradient.loadPreset(QCPColorGradient::gpJet); //������ɫ�����Ԥ��
//    gradient.setColorStopAt(0.0, QColor("#f6efa6"));   // ����ɫ����ʼʱ����ɫ
//    gradient.setColorStopAt(1.0, QColor("#bf444c"));  // ����ɫ������ʱ����ɫ
    heatmap->setGradient(gradient);
    heatmap->rescaleDataRange();        // �Զ��������ݷ�Χ�����ݷ�Χ��������Щ����ֵӳ�䵽QCPColorGradient����ɫ���䵱��
//    heatmap->setDataRange(QCPRange(0, 10));     // Ϊ�˱�����echart������һ�£����������ֶ��������ݷ�Χ
    heatmap->setInterpolate(false);         // Ϊ����ʾС���飬���ǽ��ò�ֵ

    // ����ɫ��������α߾�һ��
    QCPMarginGroup *marginGroup = new QCPMarginGroup(m_heatmapPlot);
    m_heatmapPlot->axisRect()->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup);
    colorScale->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup);


}

void demowave_widget2::initComboBox_Region()
{
    ui->comboBox_Region->clear();

    for(int i=1; i<= peakNum; i++ ){

        ui->comboBox_Region->addItem(QString::asprintf("Region %d",i));

        Region.push_back(QString::asprintf("Region %d",i)); //�����labels
    }
}

//����labels��Ӧ��λ��
QVector<double> demowave_widget2::labelPositions(const QVector<QString> &labels, double offset)
{
    QVector<double> result(labels.size());
        for (int i = 0; i < labels.size(); ++i)
            result[i] = i + offset;
        return result;
}

void demowave_widget2::FreeMemory()
{
    delete[] DemodataArray;
}

//ˢ�²�����ʾ
void demowave_widget2::FlashWave()
{
    qDebug() <<"Flash Demodulation Wave Slot responsed !"<<endl;

    QThread::msleep(50);

    //clear plot data
    m_customPlot->graph(0)->data()->clear();

    int regionNum = peakNum;

    int sizeoDemoData = demodu->DEMOdata_flash->size();

    //����sizeoDemoData�ĳ��ȣ�ʹ��ΪregionNum�ı���
    int N1 = sizeoDemoData/regionNum;
    sizeoDemoData = N1*regionNum;

//     int sizeoDemoData = regionNum*256; //�̶���ʾ�ĳ���

    //DEMOdata[] split regions
    for(int k = 0; k<sizeoDemoData; k+=regionNum){
        int p = k/regionNum;
        for(int q=0; q<regionNum;q++){
//            if(demodu->DEMOdata_flash->isEmpty()) QThread::msleep(50);
            DemodataArray[q][p] = demodu->DEMOdata_flash->pop();
        }
    }

    //region select
    RegionIndex = ui->comboBox_Region->currentIndex();
//    qDebug()<<"RegionIndex:"<<RegionIndex<<endl;

    //dispaly wave
    double yMax = DemodataArray[RegionIndex][0], yMin = DemodataArray[RegionIndex][0];

    for(int i = 0; i<N1; i++){
        QVector<double> x(1),y(1);
        x[0] = i;
        y[0]= DemodataArray[RegionIndex][i];
        m_customPlot->graph(0)->addData(x, y); //��ͼ

        //get the Max of y, the Min of y
        if (y[0]>yMax) yMax = y[0];
        if (y[0]<yMin) yMin = y[0];
    }

    m_customPlot->graph(0)->rescaleAxes(true);
    m_customPlot->yAxis->setRangeLower(yMin-2);
    m_customPlot->yAxis->setRangeUpper(yMax+2);
    m_customPlot->replot(); //ˢ��

//    //display heatmap���ٲ�ͼ��
//    for(int p=0; p<peakNum; p++){
//        for(int q=0; q<N1; q++){
//            QVector<double> x(1),y(1),z(1);
//            x[0]=p;
//            y[0]=q;
//            z[0]=DemodataArray[RegionIndex][q];
//            if(z[0]!=0) heatmap->data()->setCell(x[0],y[0],z[0]); //���z��Ϊ0����ͼ
////            else heatmap->data()->setAlpha(x[0],y[0],0); //zΪ0������Ϊ͸��

//            Sampledots.push_back(QString::asprintf("%d",q)); //�������labels
//        }
//    }

//    xTicker->setTicks(labelPositions(Region, 0.5), Region); //�������������label
//    yTicker->setTicks(labelPositions(Sampledots,0.5),Sampledots); //�������ò������label

//    m_heatmapPlot->xAxis->setRange(0, peakNum);
//    m_heatmapPlot->yAxis->setRange(0, N1);
//    m_heatmapPlot->replot(); //ˢ��

}

void demowave_widget2::on_pushButton_reset_clicked()
{
    m_customPlot->graph(0)->rescaleAxes(true);
    ui->customPlot->replot();
}

