#include "widget.h"
#include "ui_widget.h"
#include "Adi.hpp"
#include "LayoutSquare.hpp"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setupUi();
    m_adi = ui->graphicsADI;
    SerialPort=new QSerialPort;//定义一个新串口SerialPortSerialPort=new QSerialPort;//Define a new serial port SerialPort
    initUI();//初始化QSerialPort* SerialPort; //Define serial port object串口
}

Widget::~Widget()
{
    if ( m_layoutSq ) { delete m_layoutSq; m_layoutSq = nullptr; }
    if ( ui ) { delete ui; ui = nullptr; }
   // delete ui;
}

void Widget::setupUi()
{
    m_layoutSq = new LayoutSquare(this);

    m_layoutSq->setContentsMargins(0, 0, 0, 0);
    m_layoutSq->addWidget(ui->graphicsADI);

    setLayout(m_layoutSq);
}
void Widget::initUI()
{

    ClosPort();//关闭串口
    this->setWindowTitle("串口助手1_1");
    GetPortNameList();//获得串口信息
    MyConnect();
    AileronValueChange();//副翼
    ElevatorValueChange();//升降
    RudderValueChange();//方向
    UpdateSend_D();//更新模块
    UpdateSend_Gyro();
    PidValueChange();//PID
    GyroValueChange();//Gyro
    RudderDisplayIniter();//舵量显示初始化
    RudderDisplay_1=RudderDisplay_2=RudderDisplay_3=RudderDisplay_4=0;
    RudderDisplayDriver();
}

void Widget::ClosPort()
{
    SerialPort->close();
if(!SerialPort->isOpen())
{
    ui->handPortButton->setText("打开串口");
    ui->handPortButton->setStyleSheet("background-color write rgb(255,255,255);color: red rgb(238,0,0);font-size:16px");
}
}

void Widget::GetPortNameList()
{
QStringList GetPortNameList;
foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) {
    GetPortNameList<<info.portName();
    ui->selectPortBox->clear();
    ui->selectPortBox->addItems(GetPortNameList);
}

}
void Widget::MyConnect()
{
    //串口类
    connect(ui->handPortButton,&QPushButton::clicked,this,&Widget::Switch);//打开关闭串口
    connect(ui->clearReceiveButton,&QPushButton::clicked,this,&Widget::ClearReceive);//清除接收
    connect(ui->clearSendButton,&QPushButton::clicked,this,&Widget::ClearSend);//清除发送
    connect(ui->searchPortButton,&QPushButton::clicked,this,&Widget::GetPortNameList);//搜索串口
    connect(ui->senddateButton,&QPushButton::clicked,this,&Widget::SendData_1);//发送数据
    //更新发送和复原命令
    connect(ui->update1Button_1,&QPushButton::clicked,this,&Widget::UpdateSend_D);//舵机更新和复原命令
    connect(ui->Update1pushButton_2,&QPushButton::clicked,this,&Widget::UpdateSend_D2);
    connect(ui->Update1pushButton_4,&QPushButton::clicked,this,&Widget::UpdateSend_D4);
    connect(ui->Update2pushButton_P,&QPushButton::clicked,this,&Widget::UpdateSend_PID_P);//PID更新和复原命令
    connect(ui->Update2pushButton_R,&QPushButton::clicked,this,&Widget::UpdateSend_PID_R);
    connect(ui->update3Button,&QPushButton::clicked,this,&Widget::UpdateSend_Gyro);//Gyro更新查看
    connect(ui->GyroZeroButton,&QPushButton::clicked,this,&Widget::AdjustGyro);//Gyro校准
    //副翼
    connect(ui->ch1MinspinBox,SIGNAL(valueChanged(int)),this,SLOT(AileronValueChange()));
    connect(ui->ch1MaxspinBox,SIGNAL(valueChanged(int)),this,SLOT(AileronValueChange()));
    connect(ui->ch1Slider,SIGNAL(sliderReleased()),this,SLOT(AileronValueChange()));
    connect(ui->ch1radioButton,SIGNAL(clicked(bool)),this,SLOT(AileronValueChange()));
    connect(ui->ch1Slider,SIGNAL(sliderMoved(int)),this,SLOT(SliderValue()));
    //升降
    connect(ui->ch2MinspinBox,SIGNAL(valueChanged(int)),this,SLOT(ElevatorValueChange()));
    connect(ui->ch2MaxspinBox,SIGNAL(valueChanged(int)),this,SLOT(ElevatorValueChange()));
    connect(ui->ch2Slider,SIGNAL(sliderReleased()),this,SLOT(ElevatorValueChange()));
    connect(ui->ch2radioButton,SIGNAL(clicked(bool)),this,SLOT(ElevatorValueChange()));
    connect(ui->ch2Slider,SIGNAL(sliderMoved(int)),this,SLOT(SliderValue()));
    //方向
    connect(ui->ch4MinspinBox,SIGNAL(valueChanged(int)),this,SLOT(RudderValueChange()));
    connect(ui->ch4MaxspinBox,SIGNAL(valueChanged(int)),this,SLOT(RudderValueChange()));
    connect(ui->ch4Slider,SIGNAL(sliderReleased()),this,SLOT(RudderValueChange()));
    connect(ui->ch4radioButton,SIGNAL(clicked(bool)),this,SLOT(RudderValueChange()));
    connect(ui->ch4Slider,SIGNAL(sliderMoved(int)),this,SLOT(SliderValue()));
   //PID
    connect(ui->P_PdoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(PidValueChange()));
    connect(ui->P_IdoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(PidValueChange()));
    connect(ui->P_DdoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(PidValueChange()));
    connect(ui->R_PdoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(PidValueChange()));
    connect(ui->P_IdoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(PidValueChange()));
    connect(ui->P_DdoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(PidValueChange()));
    connect(ui->P_anglespinBox,SIGNAL(valueChanged(int)),this,SLOT(PidValueChange()));
    connect(ui->R_anglespinBox,SIGNAL(valueChanged(int)),this,SLOT(PidValueChange()));
    connect(ui->SPIDpushButton,SIGNAL(clicked(bool)),this,SLOT(SPID_Adjust()));
    connect(ui->CPIDpushButton,SIGNAL(clicked(bool)),this,SLOT(CPID_Adjust()));
    //步长
    connect(ui->StepdoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(SetSingleStep()));
    //差速
    connect(ui->difspinBox,SIGNAL(valueChanged(int)),this,SLOT(DifSet()));
    //校准遥控器
    connect(ui->AdjustcontrollerpushButton,SIGNAL(clicked(bool)),this,SLOT(AdjustController()));
    connect(ui->SurepushButton,SIGNAL(clicked(bool)),this,SLOT(SureAdjustController()));
    //陀螺仪
    connect(ui->pitchspinBox,SIGNAL(valueChanged(int)),this,SLOT(GyroValueChange()));
    connect(ui->rollspinBox,SIGNAL(valueChanged(int)),this,SLOT(GyroValueChange()));
    connect(ui->yawspinBox,SIGNAL(valueChanged(int)),this,SLOT(GyroValueChange()));
    connect(ui->CheckGyropushButton,&QPushButton::clicked,this,&Widget::CheckGyro);//查看Gyro数据
    //滑块显示
    connect(ui->ch1lineEdit,SIGNAL(editingFinished()),this,SLOT(SliderLinevalue()));
    //翼型的选择
    connect(ui->planecomboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(planemodechange()));
}

void Widget::Switch()
{
    if(ui->handPortButton->text()=="打开串口")
        OpenPort();
    else if(ui->handPortButton->text() == "关闭串口")
        ClosePort();
}
void Widget::OpenPort()
{
    if(SerialPort->isOpen())//如果打开了串口就先将其关闭设置属性
    {
SerialPort->clear();
SerialPort->close();
    }
    //设置串口名称
    SerialPort->setPortName(ui->selectPortBox->currentText());//选择当前串口的名字
    SerialPort->setBaudRate(ui->bitBox->currentText().toInt());//因为currentText获取的是String类型的值，而参数是int类型，所以这里要进行类型转换
    SerialPort->setDataBits(QSerialPort::DataBits(ui->dataBox->currentText().toInt()));//数据位
    SerialPort->setStopBits(QSerialPort::StopBits(ui->stopBox->currentText().toInt()));//停止位
    //校验位
    switch (ui->checkBox->currentIndex()) {
    case 0:
        SerialPort->setParity(QSerialPort::NoParity);
        break;
    case 1:
        SerialPort->setParity(QSerialPort::OddParity);
        break;
    case 2:
        SerialPort->setParity(QSerialPort::EvenParity);
        break;
    default:
        break;
    }
    //connect(SerialPort,SIGNAL(readyRead()),this,SLOT(ReceiveData()));
    connect(SerialPort,SIGNAL(readyRead()),this,SLOT(ReceiveData_Rudder()));
    SerialPort->setFlowControl(QSerialPort::NoFlowControl);
    if(!SerialPort->open(QIODevice::ReadWrite))//以ReadWrite模式打开串口，如果打开失败弹出消息框以示警告
    {
       QMessageBox::about(this,"错误","可能串口被占用");
       return;
    }
    ui->handPortButton->setText("关闭串口");
}
void Widget::ClosePort()
{
SerialPort->close();
if(!SerialPort->isOpen())
{
    ui->handPortButton->setText("打开串口");
}
}

void Widget::ClearReceive()
{
    ui->replainTextEdit->clear();
}
void Widget::ClearSend()
{
    ui->sendtextEdit->clear();
}
void Widget::SendData_1()
{

//    //未打开串口则不准发送
//    if(ui->handPortButton->text() == "打开串口")
//    {
//        QMessageBox::warning(NULL, "警告", "未打开可用串口，无法发送数据！\r\n\r\n");
//        return;
//    }
QByteArray Data_out;//最终输出数据（字符串）
QString Data;//要发送的数据（发送区内的数据）
QString tempstr,Hex;//转为字符串,16进制
int temp[10];
Data=ui->sendtextEdit->toPlainText();
if(ui->HexcheckBox->checkState()==false) Data_out=Data.toUtf8();
else
{
    for(int i=0;i<=Data.count(",",Qt::CaseSensitive);i++)
    {
     tempstr=Data.section(",",i,i);//将以逗号隔开的数据分别取出来
        temp[i]=tempstr.toInt();//将取出来的数据转为整数型
        Hex.append(QString("%1").arg(temp[i],4,16,QLatin1Char('0')));//将整数型10进制转为4位16进制
    }
    Data_out=QByteArray::fromHex(Hex.toLatin1().data()); //将整型16进制转为16进制字符串
}
SerialPort->write(Data_out);
}

void Widget::SendData_2(QString str)
{
    QByteArray Data_out;//最终输出数据（字符串）
    QString Data=str;//要发送的数据（发送区内的数据）
    QString tempstr,Hex;//转为字符串,16进制
    int temp[10];
    if(ui->HexcheckBox->checkState()==false) Data_out=Data.toUtf8();
    else
    {
        for(int i=0;i<=Data.count(",",Qt::CaseSensitive);i++)
        {
         tempstr=Data.section(",",i,i);//将以逗号隔开的数据分别取出来
            temp[i]=tempstr.toInt();//将取出来的数据转为整数型
            Hex.append(QString("%1").arg(temp[i],4,16,QLatin1Char('0')));//将整数型10进制转为4位16进制
        }

        Data_out=QByteArray::fromHex(Hex.toLatin1().data()); //将整型16进制转为16进制字符串
    }
//    Data_out.append("\r\n");
    SerialPort->write(Data_out);
//    qDebug()<<"长度"<<SerialPort->write(Data_out);
//    qDebug()<<"数据"<<Data_out;
}
void Widget::ReceiveData()
{
QString Receive;
QByteArray Buffer;
Buffer=SerialPort->readAll();//读取所有的数据
if(!Buffer.isEmpty())//如果读取到数据
{
    //转为大写的16进制显示，并且每个4位16进制数直接用空格隔开
    QString tempstr=Buffer.toHex().data();

    tempstr=tempstr.toUpper();
    for(int i=0;i<tempstr.length();i+=2)//实现每4位16进制数接收一个空格
    {
        QString Str;
        Str=tempstr.mid(i,2);
        Receive+=Str;
        Receive+=' ';
    }
    QString OldData;
    OldData=ui->replainTextEdit->toPlainText();//储存之前的数据
    ui->replainTextEdit->clear();//清除数据
    //重新显示
    if(ui->RHexcheckBox->isChecked()==false)
    {
        OldData+=QString(Buffer);
        ui->replainTextEdit->appendPlainText(OldData);
    }
    else
    {
        OldData+=QString(Receive);
        ui->replainTextEdit->appendPlainText(OldData);
    }
Buffer.clear();
    }
}
void Widget::ReceiveData_Rudder()
{
    QString BufferString;
    QString tempstr;
    QByteArray Buffer;
    float roll_,pitch_;
    int temp[10];
    if(SerialPort->bytesAvailable()>=24)
    {BufferString=SerialPort->read(24);//读取数据
    Buffer=SerialPort->read(24);}
    if(!BufferString.isEmpty())
    {
        if(Buffer[0]='@')
        { 
        for(int i=0;i<=4;i++)
        {
         tempstr=BufferString.section(",",i,i);//将以逗号隔开的数据分别取出来
            temp[i]=tempstr.toInt();//将取出来的数据转为整数型
        }
        if(temp[1]!=0&&temp[2]!=0&&temp[3]!=0&&temp[4]!=0)
        {
        RudderDisplay_1=temp[1]; RudderDisplay_2=temp[2];
        RudderDisplay_3=temp[3]; RudderDisplay_4=temp[4];
        }
RudderDisplayDriver();
        }
        if(Buffer[0]='*')
        {
        for(int i=0;i<=4;i++)
        {
         tempstr=BufferString.section(",",i,i);//将以逗号隔开的数据分别取出来
            temp[i]=tempstr.toInt();//将取出来的数据转为整数型
        }
        if(temp[1]!=0&&temp[2]!=0&&temp[3]!=0&&temp[4]!=0)
        {
        roll_=temp[1]; pitch_=temp[2];
        }
        qDebug()<<roll_;
        qDebug()<<pitch_;
setRoll(roll_);
setRoll(pitch_);
update();
        }
    }

}
void Widget::AileronValueChange()
{
ui->ch1Slider->setMaximum(ui->ch1MaxspinBox->value());//设置最大最小值
ui->ch1Slider->setMinimum(ui->ch1MinspinBox->value());
ui->ch1MaxspinBox->setSingleStep(10);//设置步长
ui->ch1MinspinBox->setSingleStep(10);
ui->ch1radioButton->raise();//小部件将在视觉上位于任何重叠的同级小部件的前面

}

void Widget::ElevatorValueChange()
{
    ui->ch2Slider->setMaximum(ui->ch2MaxspinBox->value());//设置最大最小值
    ui->ch2Slider->setMinimum(ui->ch2MinspinBox->value());
    ui->ch2MaxspinBox->setSingleStep(10);//设置步长
    ui->ch2MinspinBox->setSingleStep(10);
    ui->ch2radioButton->raise();//小部件将在视觉上位于任何重叠的同级小部件的前面

}

void Widget:: RudderValueChange()
{
    ui->ch4Slider->setMaximum(ui->ch4MaxspinBox->value());//设置最大最小值
    ui->ch4Slider->setMinimum(ui->ch4MinspinBox->value());
    ui->ch4radioButton->raise();//小部件将在视觉上位于任何重叠的同级小部件的前面

}
void Widget::UpdateSend_D()//舵机量程
{
    AileronValueInitstr +="$,1,";
    AileronValueInitstr +=QString::number(ui->ch1MinspinBox->value(),10);
    AileronValueInitstr +=",";
    AileronValueInitstr +=QString::number(ui->ch1Slider->value(),10);
    AileronValueInitstr +=",";
    AileronValueInitstr +=QString::number(ui->ch1MaxspinBox->value(),10);
    AileronValueInitstr +=",";
    AileronValueInitstr +=QString::number(ui->ch1radioButton->isChecked(),10);
    AileronValueInitstr +=",#";
    qDebug()<<AileronValueInitstr;
    SendData_2(AileronValueInitstr);
    AileronValueInitstr.clear();
}
void Widget::UpdateSend_D2()//舵机量程
{

    ElevatorValueInitstr +="$,2,";
    ElevatorValueInitstr +=QString::number(ui->ch2MinspinBox->value(),10);
    ElevatorValueInitstr +=",";
    ElevatorValueInitstr +=QString::number(ui->ch2Slider->value(),10);
    ElevatorValueInitstr +=",";
    ElevatorValueInitstr +=QString::number(ui->ch2MaxspinBox->value(),10);
    ElevatorValueInitstr +=",";
    ElevatorValueInitstr +=QString::number(ui->ch2radioButton->isChecked(),10);
    ElevatorValueInitstr +=",#";
    qDebug()<<ElevatorValueInitstr;
    SendData_2(ElevatorValueInitstr);
    ElevatorValueInitstr.clear();
}
void Widget::UpdateSend_D4()//舵机量程
{
    RudderValueInitstr +="$,4,";
    RudderValueInitstr +=QString::number(ui->ch4MinspinBox->value(),10);
    RudderValueInitstr +=",";
    RudderValueInitstr +=QString::number(ui->ch4Slider->value(),10);
    RudderValueInitstr +=",";
    RudderValueInitstr +=QString::number(ui->ch4MaxspinBox->value(),10);
    RudderValueInitstr +=",";
    RudderValueInitstr +=QString::number(ui->ch4radioButton->isChecked(),10);
    RudderValueInitstr +=",#";
    qDebug()<<RudderValueInitstr;
    SendData_2(RudderValueInitstr);
    RudderValueInitstr.clear();

}

void Widget::UpdateSend_PID_P()//PID_Pitch调参
{
//pitch
PidValueInitstr_Pitch +="$,P,";
PidValueInitstr_Pitch +=QString::number(ui->P_PdoubleSpinBox->value()*100,10,0);//转为10进制数并保留0位小数
PidValueInitstr_Pitch +=",";
PidValueInitstr_Pitch +=QString::number(ui->P_IdoubleSpinBox->value()*100,10,0);
PidValueInitstr_Pitch +=",";
PidValueInitstr_Pitch +=QString::number(ui->P_DdoubleSpinBox->value()*100,10,0);
PidValueInitstr_Pitch +=",";
PidValueInitstr_Pitch +=QString::number(ui->P_anglespinBox->value(),10);
PidValueInitstr_Pitch +=",";
PidValueInitstr_Pitch +=QString::number(ui->PradioButton->isChecked(),10);
PidValueInitstr_Pitch +=",#";
 qDebug()<<PidValueInitstr_Pitch;
SendData_2(PidValueInitstr_Pitch);
PidValueInitstr_Pitch.clear();

}
void Widget::UpdateSend_PID_R()//PID_Roll调参
{
    //roll
    PidValueInitstr_Roll +="$,R,";
    PidValueInitstr_Roll +=QString::number(ui->R_PdoubleSpinBox->value()*100,10,0);
    PidValueInitstr_Roll +=",";
    PidValueInitstr_Roll +=QString::number(ui->R_IdoubleSpinBox->value()*100,10,0);
    PidValueInitstr_Roll +=",";
    PidValueInitstr_Roll +=QString::number(ui->R_DdoubleSpinBox->value()*100,10,0);
    PidValueInitstr_Roll +=",";
    PidValueInitstr_Roll +=QString::number(ui->R_anglespinBox->value(),10);
    PidValueInitstr_Roll +=",";
    PidValueInitstr_Roll +=QString::number(ui->RradioButton->isChecked(),10);
    PidValueInitstr_Roll +=",#";
    qDebug()<<"pidpitch:"<<PidValueInitstr_Pitch;
    qDebug()<<"pidroll:"<<PidValueInitstr_Roll;
    SendData_2(PidValueInitstr_Roll);
    PidValueInitstr_Roll.clear();
}
void Widget::SPID_Adjust()
{
    QString SPID;
    SPID+="$,SP,#";
    SendData_2(SPID);
    SPID.clear();
}
void Widget::CPID_Adjust()
{
    QString CPID;
    CPID+="$,CP,#";
    SendData_2(CPID);
    CPID.clear();
}
void Widget::UpdateSend_Gyro()//Gyro调参
{
GyroValueInitstr +="&,R,";
GyroValueInitstr +=QString::number(ui->rollspinBox->value(),10);
GyroValueInitstr +=",P,";
GyroValueInitstr +=QString::number(ui->pitchspinBox->value(),10);
GyroValueInitstr +=",#";
SendData_2(GyroValueInitstr);
GyroValueInitstr.clear();
}

void Widget::CheckGyro()
{
    QString str;
    str="$,DD,#";
    SendData_2(str);
    qDebug()<<str;
}
void Widget::AdjustGyro()
{
    QString str;
    str="$,JZ,#";
    SendData_2(str);
    qDebug()<<str;
}
void Widget::PidValueChange()
{
QString PidValueInitstr;
}
void Widget::GyroValueChange()
{
QString GyroValueInitstr;

}
void Widget::SetSingleStep()
{
    ui->P_PdoubleSpinBox->setSingleStep(ui->StepdoubleSpinBox->value());
    ui->P_IdoubleSpinBox->setSingleStep(ui->StepdoubleSpinBox->value());
    ui->P_DdoubleSpinBox->setSingleStep(ui->StepdoubleSpinBox->value());
    ui->R_PdoubleSpinBox->setSingleStep(ui->StepdoubleSpinBox->value());
    ui->R_IdoubleSpinBox->setSingleStep(ui->StepdoubleSpinBox->value());
    ui->R_DdoubleSpinBox->setSingleStep(ui->StepdoubleSpinBox->value());
}
void Widget::SliderValue()
{
    ui->ch1lineEdit->setText(QString::number(ui->ch1Slider->value(),10));
    ui->ch2lineEdit->setText(QString::number(ui->ch2Slider->value(),10));
    ui->ch4lineEdit->setText(QString::number(ui->ch4Slider->value(),10));
}
void Widget::SliderLinevalue()
{

    ui->ch1Slider->setValue(ui->ch1lineEdit->text().toInt());
    ui->ch1Slider->setValue(ui->ch1lineEdit->text().toInt());
    ui->ch1Slider->setValue(ui->ch1lineEdit->text().toInt());

}
void Widget::DifSet()
{
    QString DifSetInitstr;
    DifSetInitstr +="$,DF,";
    DifSetInitstr +=QString::number(ui->difspinBox->value(),10);
    DifSetInitstr +=",#";
    SendData_2(DifSetInitstr);
    DifSetInitstr.clear();
}
void Widget::AdjustController()
{
    QString AControllerInitstr;
    AControllerInitstr +="$,AC,#";
    SendData_2(AControllerInitstr);
    AControllerInitstr.clear();
}
void Widget::SureAdjustController()
{
    QString SureAdjustCInitstr;
    SureAdjustCInitstr +="$,AS,#";
    SendData_2(SureAdjustCInitstr);
    SureAdjustCInitstr.clear();
}
void Widget::RudderDisplayIniter()
{
ui->lineEdit->setStyleSheet("background:transparent;border-width:0;border-style:outset");
ui->lineEdit_2->setStyleSheet("background:transparent;border-width:0;border-style:outset");
ui->lineEdit_3->setStyleSheet("background:transparent;border-width:0;border-style:outset");
ui->lineEdit_4->setStyleSheet("background:transparent;border-width:0;border-style:outset");
}
void Widget::RudderDisplayDriver()
{
    ui->lineEdit->setText(QString::number(RudderDisplay_1,10));
    ui->lineEdit_2->setText(QString::number(RudderDisplay_2,10));
    ui->lineEdit_3->setText(QString::number(RudderDisplay_3,10));
    ui->lineEdit_4->setText(QString::number(RudderDisplay_4,10));
    ui->progressBar_1->setMaximum(ui->ch1MaxspinBox->value());
    ui->progressBar_1->setMinimum(ui->ch1MinspinBox->value());
    ui->progressBar_1->setValue(RudderDisplay_1);
    ui->progressBar_2->setMaximum(ui->ch2MaxspinBox->value());
    ui->progressBar_2->setMinimum(ui->ch2MinspinBox->value());
    ui->progressBar_2->setValue(RudderDisplay_2);
    ui->progressBar_3->setMaximum(2000);
    ui->progressBar_3->setMinimum(1000);
    ui->progressBar_3->setValue(RudderDisplay_3);
    ui->progressBar_4->setMaximum(ui->ch4MaxspinBox->value());
    ui->progressBar_4->setMinimum(ui->ch4MinspinBox->value());
    ui->progressBar_4->setValue(RudderDisplay_4);
}
void Widget::planemodechange()
{
   if(ui->planecomboBox->currentText()=="飞翼") myplanemode.plane_fly();
       else if(ui->planecomboBox->currentText()=="固定翼") myplanemode.plane_fixed();

}
void Widget::on_PIDChartpushButton_clicked()
{
    //this->close();
    PIDChart *pidchar=new PIDChart;
    pidchar->show();
}

