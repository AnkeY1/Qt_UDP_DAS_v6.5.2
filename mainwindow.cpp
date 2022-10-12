#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
    ,ui(new Ui::MainWindow)
    ,isDemo(false)
    ,isStart(false)
    ,isHEX(false)
    ,AcqMode(1)
    ,Freq(10*1000) //������
    ,demoFlashTime(50)//����ź� flashһ�ε�ʱ��
    ,pulseFlashTime(5)//�����ź� flashһ�ε�ʱ��
    ,demoFlashFreq(1000/demoFlashTime)  //��flash��Ƶ��
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
    ui->setupUi(this);

    setWindowIcon(QIcon("C:/Qt_UDP_DAS/main.png"));

    setWindowTitle(QString("Qt_UDP_DAS"));

    //set Style Sheet
    setLocalStyleSheet();

    //set Local Message
    setLocalMsg();

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
    demodu = new Demodulation(udp_recv,demoFlashTime,demoFlashFreq,Freq,peakNum);
    demowave_Widget2 = new demowave_widget2(demodu);
    com_send = new COM_Send(this);
    demodata_save = new DemoData_Save(demodu);

    SaveTimer->setTimerType(Qt::PreciseTimer);
    FlashTimer_Pulse->setTimerType(Qt::PreciseTimer);
    DemoTimer->setTimerType(Qt::PreciseTimer);
    FlashTimer->setTimerType(Qt::PreciseTimer);

    //clear window
    if(ui->textEdit_Msg->isFullScreen())
        ui->textEdit_Msg->clear();

    connect(ui->comboBox_Mode, QOverload<int>::of(&QComboBox::currentIndexChanged),this,&MainWindow::on_comboBox_Mode_currentIndexChangedSlot);
    connect(udp_recv,&QThread::finished,this,&MainWindow::FinishUDP_RecvThread);
    connect(SaveTimer,&QTimer::timeout,this,&MainWindow::OpenSaveThread);
    connect(DemoTimer,&QTimer::timeout,this,&MainWindow::OpenDemoSaveThread);
    connect(FlashTimer_Pulse,&QTimer::timeout,pulsewave_Widget2,&pulsewave_widget2::FlashWave);
    connect(FlashTimer,&QTimer::timeout,demowave_Widget2,&demowave_widget2::FlashWave);
    connect(writeToFiles,&QThread::finished,this,&MainWindow::FinishWriteToFilesThread);
    connect(demodu,&QThread::finished,this,&MainWindow::FinishDemodulationThread);
    connect(demodata_save,&QThread::finished,this,&MainWindow::FinishDemoData_saveThread);
    connect(pulsewave_Widget2,&pulsewave_widget2::PauseWave,this,&MainWindow::PulseWave_pause_slot);
    connect(pulsewave_Widget2,&pulsewave_widget2::RestartWave,this,&MainWindow::PulseWave_restart_slot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setLocalStyleSheet()
{

    QFile file("C:/Qt_UDP_DAS/qssStyls/Aqua.qss");//set Style Sheet Directory

    file.open(QIODevice::ReadOnly);

    QString styleSheet = QLatin1String(file.readAll());

    qApp->setStyleSheet(styleSheet);

}

void MainWindow::setLocalMsg()
{
    //��ȡ�����ļ������
    QString localHostName = QHostInfo:: localHostName();
    qDebug() <<"LocalHostName: "<<localHostName<<endl;

    ui->textEdit_Msg->insertPlainText("LocalHostName: "+localHostName+"\n");

    //��ȡ����IP
    QHostInfo info = QHostInfo::fromName(localHostName);
    QList<QHostAddress> strIpAddress  = info.addresses();
    QHostAddress IpAddress =  strIpAddress.back();
    qDebug() << "IpAddress: " << IpAddress<<endl;
    qDebug()<<"--------------------------"<<endl;

    ui->textEdit_Msg->insertPlainText("IpAddress: "+IpAddress.toString()+" Port: 7000 \n");

    //���ô��ڵı���
    QString title = QString("Qt_UDP_DAS");
//    setWindowTitle(title);

    ui->textEdit_Msg->insertPlainText("Setting Acquisition Card Mode I ! \n");
}

void MainWindow::FinishUDP_RecvThread()
{
    udp_recv->quit();
    udp_recv->wait();
}

void MainWindow::OpenSaveThread()
{
    isSave = ui->checkBox_Save->isChecked();

    if(isSave && isStart)
        writeToFiles->start();
}

void MainWindow::OpenDemoSaveThread()
{
    isDemo = ui->checkBox_Demo->isChecked();

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

void MainWindow::FinishWriteToFilesThread()
{
    writeToFiles->quit();
    writeToFiles->wait();

    ui->textEdit_Msg->insertPlainText(" Save Files have been saved in " + writeToFiles->saveFilenameAll+"\n");
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

    ui->textEdit_Msg->insertPlainText("Demodulation have been saved in " + demodata_save->saveFileDemo+ "\n");
}

void MainWindow::on_pushButton_Start_clicked()
{
    ui->textEdit_Msg->insertPlainText("Started ! \n");

    if(!udp_recv->isRunning())
        udp_recv->start();

    isStart = true;

    if(isSave) ui->checkBox_Demo->setDisabled(true);
    if(isDemo) ui->checkBox_Save->setDisabled(true);

    if(AcqMode == 1){
        ui->checkBox_Save->setDisabled(true);

        ui->checkBox_Demo->setDisabled(true);
    }

    if(AcqMode == 2){
        ui->pushButton_Display_pulse->setDisabled(true);

        ui->pushButton_Send->setDisabled(true);
    }

}

void MainWindow::on_pushButton_Stop_clicked()
{
    isStart = false;
    isSave = false;
    isHEX = false;
    isDemo = false;

    ui->checkBox_Demo->setEnabled(true);
    ui->checkBox_Hex->setEnabled(true);
    ui->checkBox_Save->setEnabled(true);
    ui->pushButton_Display_pulse->setEnabled(true);
    ui->pushButton_Send->setEnabled(true);

    ui->checkBox_Hex->setChecked(isStart);
    ui->checkBox_Save->setChecked(isSave);
    ui->checkBox_Demo->setChecked(isDemo);

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

    ui->textEdit_Msg->insertPlainText("Stopped ! \n");
}

void MainWindow::on_pushButton_Clear_clicked()
{
    ui->textEdit_Msg->clear();
}

void MainWindow::on_checkBox_Save_clicked()
{
    isSave = true;

    if(!SaveTimer->isActive()) SaveTimer->start(60000); //��60s������
}

void MainWindow::on_checkBox_Hex_clicked()
{
    isHEX = true;
}

void MainWindow::on_checkBox_Demo_clicked()
{
    isDemo = true;

    if(!DemoTimer->isActive()) DemoTimer->start(60000); //����洢��ʱ

    demodu->start(); //��ʼ����߳�
}

void MainWindow::on_comboBox_Mode_currentIndexChangedSlot()
{
    //�ɼ���ģʽѡ��
    if(ui->comboBox_Mode->currentIndex() == 0){
        ui->textEdit_Msg->insertPlainText("Setting Acquisition Card Mode I ! \n");

        AcqMode = 1;
    }

    if(ui->comboBox_Mode->currentIndex() == 1){
        ui->textEdit_Msg->insertPlainText("Setting Acquisition Card Mode II ! \n");

        AcqMode = 2;
    }
}

void MainWindow::on_pushButton_Display_pulse_clicked()
{


    if(!FlashTimer_Pulse->isActive()) FlashTimer_Pulse->start(pulseFlashTime);

    pulsewave_Widget2->show();
}

void MainWindow::on_pushButton_Send_clicked()
{
    com_send->start();

    ui->textEdit_Msg->insertPlainText("Sendding peak.txt ! \n");
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
    if(!FlashTimer->isActive()) FlashTimer_Pulse->start(pulseFlashTime);
}

