#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
    ,ui(new Ui::MainWindow)
    ,isDemo(false)
    ,isStart(false)
    ,isHEX(true)
    ,AcqMode(1)
    ,Freq(20*1000) //������
    ,demoFlashTime(2000)//����ź� flashһ�ε�ʱ��
    ,pulseFlashTime(1000)//�����ź� flashһ�ε�ʱ��
//    ,demoFlashFreq(1000/demoFlashTime)  //��flash��Ƶ��
    ,pulseFlashFreq(1000/pulseFlashTime) //��flash��Ƶ��
    //����ĳ�ʼ����Ҫ�ù��캯����ʼ���б�
//    ,udp_recv(new UDP_Recv(this))
//    ,SaveTimer(new QTimer()) // writetofiles Timer
//    ,FlashTimer_Pulse(new QTimer())  //Flash pulsewave_widget2 Timer
//    ,DemoTimer(new QTimer()) //demodualtion Timer
//    ,FlashTimer(new QTimer()) //Flash demowave_widget2 Timer
//    ,writeToFiles(new WriteToFiles(udp_recv))
//    ,pulsewave_Widget2(new pulsewave_widget2(udp_recv))
//    ,demodu(new Demodulation(udp_recv,demoFlashTime,demoFlashFreq))
//    ,demowave_Widget2(new demowave_widget2(demodu))
//    ,com_send(new COM_Send(this))
//    ,demodata_save(new DemoData_Save(demodu))
{

//    setAttribute(Qt::WA_DeleteOnClose);

    ui->setupUi(this);

    setWindowIcon(QIcon("C:/Qt_UDP_DAS/main.png"));

    setWindowTitle(QString("Qt_UDP_DAS"));

    //set HTML pages
    setHtmlPages();

    //read configuration file
    readConfigFile();

    udp_recv = new UDP_Recv(this);
    udp_recv->start();
    SaveTimer = new QTimer();
    FlashTimer_Pulse = new QTimer();
    DemoTimer = new QTimer();
    FlashTimer = new QTimer();
    writeToFiles = new WriteToFiles(udp_recv, peakNum);
    pulsewave_Widget2 = new pulsewave_widget2(udp_recv);
    demodu = new Demodulation(udp_recv,demoFlashTime,Freq,peakNum);
    com_send = new COM_Send(this);
    demodata_save = new DemoData_Save(demodu);
    MsgTimer = new QTimer();
    m_fft = new FFT(demodu);
    demowave_Widget2 = new demowave_widget2(demodu,m_fft);
    fftTimer = new QTimer();


    SaveTimer->setTimerType(Qt::PreciseTimer);
    FlashTimer_Pulse->setTimerType(Qt::PreciseTimer);
    DemoTimer->setTimerType(Qt::PreciseTimer);
    FlashTimer->setTimerType(Qt::PreciseTimer);
    MsgTimer->setTimerType(Qt::PreciseTimer);
    fftTimer->setTimerType(Qt::PreciseTimer);

    connect(webobj,&WebClass::send_mainwindow_btn_start,this,&MainWindow::on_pushButton_Start_clicked);
    connect(webobj,&WebClass::send_mainwindow_btn_stop,this,&MainWindow::on_pushButton_Stop_clicked);
    connect(webobj,&WebClass::send_mainwindow_ckbox_isSave,this,&MainWindow::on_checkBox_Save_clicked);
    connect(webobj,&WebClass::send_mainwindow_ckbox_isDemo,this,&MainWindow::on_checkBox_Demo_clicked);
    connect(webobj,&WebClass::send_mainwindow_btn_display_pulse,this,&MainWindow::on_pushButton_Display_pulse_clicked);
    connect(webobj,&WebClass::send_mainwindow_btn_display_demo,this,&MainWindow::on_pushButton_Display_demo_clicked);
    connect(webobj,&WebClass::send_mainwindow_combobox_mode_indexchanged,this,&MainWindow::on_comboBox_Mode_currentIndexChangedSlot);
    connect(demowave_Widget2->webobj4,&WebClass::send_demowidget_combobox_region_indexchanged,demowave_Widget2,&demowave_widget2::on_comboBox_region_currentIndexChangedSlot);
    connect(demowave_Widget2->webobj4,&WebClass::send_fft_combobox_region_indexchanged,m_fft,&FFT::on_comboBox_region_currentIndexChangedSlot);
    connect(pulsewave_Widget2,&pulsewave_widget2::send_mainwindow_btn_pause,this,&MainWindow::PulseWave_pause_slot);
    connect(pulsewave_Widget2,&pulsewave_widget2::send_mainwindow_btn_restart,this,&MainWindow::PulseWave_restart_slot);

    connect(SaveTimer,&QTimer::timeout,this,&MainWindow::OpenSaveThread);
    connect(DemoTimer,&QTimer::timeout,this,&MainWindow::OpenDemoSaveThread);
    connect(FlashTimer_Pulse,&QTimer::timeout,pulsewave_Widget2,&pulsewave_widget2::FlashWave);
    connect(FlashTimer,&QTimer::timeout,demowave_Widget2,&demowave_widget2::FlashWave);
    connect(MsgTimer,&QTimer::timeout,this,&MainWindow::setLocalMsg);
    connect(fftTimer,&QTimer::timeout,this,&MainWindow::OpenFFTThread);

    connect(udp_recv,&QThread::finished,this,&MainWindow::FinishUDP_RecvThread);
    connect(writeToFiles,&QThread::finished,this,&MainWindow::FinishWriteToFilesThread);
    connect(demodu,&QThread::finished,this,&MainWindow::FinishDemodulationThread);
    connect(demodata_save,&QThread::finished,this,&MainWindow::FinishDemoData_saveThread);
    connect(m_fft,&QThread::finished,this,&MainWindow::FinishFFT_Thread);

    cnt=1;
    MsgTimer->start(100);

}


MainWindow::~MainWindow()
{
    delete ui;
    delete webChannel;
    delete webobj;

    on_pushButton_Stop_clicked();
    FinishUDP_RecvThread();
    FinishDemodulationThread();
    FinishWriteToFilesThread();
    FinishDemoData_saveThread();
    FinishFFT_Thread();


}

//������Ͻ�x��ťʱ��Ӧ
void MainWindow::closeEvent( QCloseEvent * event )
{
    qApp->quit();
//switch( QMessageBox::information( this, tr("exit tip"), tr("Do you really want exit?"), tr("Yes"), tr("No"), 0, 1 ) )
//   {
//     case 0:
//          event->accept();
//          break;
//     case 1:
//     default:
//         event->ignore();
//         break;
//   }
}

void MainWindow::setHtmlPages()
{
    m_mainwindow_widget = ui->mainwindow_widget;
    m_mainwindow_widget->setContextMenuPolicy(Qt::NoContextMenu);
    m_mainwindow_widget->load(QUrl::fromLocalFile(QString("C:/Qt_UDP_DAS/PAGE/tpl03/MainWindow.html")));

    webChannel = new QWebChannel;

    webobj = new WebClass();

    webChannel->registerObject("webobj", webobj);

    m_mainwindow_widget->page()->setWebChannel(webChannel);
}


void MainWindow::setLocalMsg()
{
    QJsonObject LocalMsg_obj;

    //��ȡ�����ļ������
    QString localHostName = QHostInfo:: localHostName();
    qDebug() <<"LocalHostName: "<<localHostName<<endl;

    QJsonArray HostName_arr;
    HostName_arr.push_back(QString("LocalHostName: "+localHostName));
    LocalMsg_obj.insert("HostName",HostName_arr);

    //��ȡ����IP
    QHostInfo info = QHostInfo::fromName(localHostName);
    QList<QHostAddress> strIpAddress  = info.addresses();
    QHostAddress IpAddress =  strIpAddress.back();
    qDebug() << "IpAddress: " << IpAddress<<endl;

    QJsonArray IpAddr_arr;
    IpAddr_arr.push_back(QString("IpAddress: "+IpAddress.toString()+" Port: 7000"));
    LocalMsg_obj.insert("IpAddr",IpAddr_arr);

    QJsonArray ModeI_arr;
    ModeI_arr.push_back(QString("Setting Acquisition Card Mode I !"));
    LocalMsg_obj.insert("ModeI",ModeI_arr);

    //����js��LocalMsg����
    QString LocalMsg_js = QString("LocalMsg(%1)").arg(QString(QJsonDocument(LocalMsg_obj).toJson()));

    m_mainwindow_widget->page()->runJavaScript(LocalMsg_js);

    if(cnt++>=3) MsgTimer->stop();

}

void MainWindow::FinishUDP_RecvThread()
{
    udp_recv->quit();
    udp_recv->wait();

}

void MainWindow::OpenSaveThread()
{
    if(isSave && isStart)
        writeToFiles->start();
}

void MainWindow::OpenDemoSaveThread()
{
    if(isDemo && isStart)
        demodata_save->start(); //��ʼ����洢�߳�
}

void MainWindow::readConfigFile()
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

void MainWindow::OpenFFTThread()
{
    if(!m_fft->isRunning())
        m_fft->start();
}

void MainWindow::FinishWriteToFilesThread()
{
    writeToFiles->quit();
    writeToFiles->wait();

    QJsonObject save_msg_obj;
    QJsonArray saveMsg;
    saveMsg.push_back(QString(" Save Files have been saved in " + writeToFiles->saveFilenameAll));
    save_msg_obj.insert("save",saveMsg);
    QString save_msg_js = QString("save_msg(%1)").arg(QString(QJsonDocument(save_msg_obj).toJson()));
    m_mainwindow_widget->page()->runJavaScript(save_msg_js);
}

void MainWindow::FinishDemodulationThread()
{
    demodu->quit();
    demodu->wait();
}

void MainWindow::FinishDemoData_saveThread()
{
    demodata_save->quit();
    demodata_save->wait();

    QJsonObject demo_msg_obj;
    QJsonArray demoMsg;
    demoMsg.push_back(QString("Demodulation have been saved in " + demodata_save->saveFileDemo));
    demo_msg_obj.insert("demo",demoMsg);
    QString demo_msg_js = QString("demo_msg(%1)").arg(QString(QJsonDocument(demo_msg_obj).toJson()));
    m_mainwindow_widget->page()->runJavaScript(demo_msg_js);
}

void MainWindow::FinishFFT_Thread()
{
    m_fft->quit();
    m_fft->wait();
}

void MainWindow::on_pushButton_Start_clicked()
{
    QJsonObject start_msg_obj;
    QJsonArray startMsg;
    startMsg.push_back(QString("Started ! \n"));
    start_msg_obj.insert("start",startMsg);
    QString Start_Msg_js = QString("Start_Msg(%1)").arg(QString(QJsonDocument(start_msg_obj).toJson()));
    m_mainwindow_widget->page()->runJavaScript(Start_Msg_js);

    isStart = true;

    if(!udp_recv->isRunning())
        udp_recv->start();

}

void MainWindow::on_pushButton_Stop_clicked()
{
    QJsonObject stop_msg_obj;
    QJsonArray stopMsg;
    stopMsg.push_back(QString("Stopped ! \n"));
    stop_msg_obj.insert("stop",stopMsg);
    QString Stop_Msg_js = QString("Stop_Msg(%1)").arg(QString(QJsonDocument(stop_msg_obj).toJson()));
    m_mainwindow_widget->page()->runJavaScript(Stop_Msg_js);

    isStart = false;
    isSave = false;
    isDemo = false;

//    ui->checkBox_Demo->setEnabled(true);
//    ui->checkBox_Hex->setEnabled(true);
//    ui->checkBox_Save->setEnabled(true);
//    ui->pushButton_Display_pulse->setEnabled(true);
//    ui->pushButton_Send->setEnabled(true);

//    ui->checkBox_Hex->setChecked(isStart);
//    ui->checkBox_Save->setChecked(isSave);
//    ui->checkBox_Demo->setChecked(isDemo);

    //clear CHdata
    udp_recv->clearCHdata();
    udp_recv->quit();

    //clear DEMOdata
    demodu->DEMOdata_flash->clear();
    demodu->DEMOdata_save->clear();

    if(SaveTimer->isActive()) SaveTimer->stop();
    if(DemoTimer->isActive()) DemoTimer->stop();
    if(FlashTimer_Pulse->isActive()) FlashTimer_Pulse->stop();
    if(FlashTimer->isActive()) FlashTimer->stop();

}

void MainWindow::on_pushButton_Clear_clicked()
{
//    ui->textEdit_Msg->clear();
}

void MainWindow::on_checkBox_Save_clicked(bool _isSave)
{
    isSave = _isSave;

    if(!SaveTimer->isActive() && isSave) SaveTimer->start(60000); //��60s������
    if(SaveTimer->isActive() && (!isSave)) SaveTimer->stop();
}

void MainWindow::on_checkBox_Demo_clicked(bool _isDemo)
{
    isDemo = _isDemo;

    if(!DemoTimer->isActive() && isDemo) DemoTimer->start(60000); //����洢��ʱ
//    if(DemoTimer->isActive() && (!isDemo)) DemoTimer->stop();

    demodu->start(); //��ʼ����߳�

    if(!fftTimer->isActive()) fftTimer->start(2000); //fft�����ʱ
}

void MainWindow::on_comboBox_Mode_currentIndexChangedSlot(int _AcqMode)
{
    AcqMode = _AcqMode;
    qDebug()<<"AcqMode = "<<_AcqMode<<endl;
}

void MainWindow::on_pushButton_Display_pulse_clicked()
{
    if(!FlashTimer_Pulse->isActive()) FlashTimer_Pulse->start(pulseFlashTime);

    pulsewave_Widget2->show();
}

void MainWindow::on_pushButton_Send_clicked()
{
    com_send->start();

//    ui->textEdit_Msg->insertPlainText("Sendding peak.txt ! \n");
}

void MainWindow::on_pushButton_Display_demo_clicked()
{
    if(!FlashTimer->isActive()) FlashTimer->start(demoFlashTime);

    demowave_Widget2->show();
}

void MainWindow::PulseWave_pause_slot()
{
    if(FlashTimer_Pulse->isActive()) FlashTimer_Pulse->stop();
}

void MainWindow::PulseWave_restart_slot()
{
    if(!FlashTimer_Pulse->isActive()) FlashTimer_Pulse->start(pulseFlashTime);
}


