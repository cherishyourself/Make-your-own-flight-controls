#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QMessageBox>
#include <QPushButton>
#include <QByteArray>
#include <QDebug>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QLabel>
#include <QLayout>
#include <QDebug>
#include <QSpinBox>
#include <QSlider>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QAbstractSlider>
#include <QThread>
#include <Adi.hpp>
#include <QHBoxLayout>
#include <LayoutSquare.hpp>
#include "pidchart.h"
#include "ui_pidchart.h"
#include "planemode.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QSerialPort* SerialPort; //定义串口对象
    Ui::Widget *ui;
public slots:
void initUI();//初始化串口
void ClosPort();//关闭串口
void GetPortNameList();
void MyConnect();
void Switch();
void OpenPort();
void ClosePort();
void ClearReceive();
void ClearSend();
void SendData_1();
void SendData_2(QString str);
void ReceiveData();
void ReceiveData_Rudder();
void AileronValueChange();//副翼
void ElevatorValueChange();//升降
void RudderValueChange();//方向
void UpdateSend_D();//更新模块
void UpdateSend_D4();//更新模块
void UpdateSend_D2();//更新模块
void UpdateSend_PID_P();
void UpdateSend_PID_R();
void UpdateSend_Gyro();
void PidValueChange();
void GyroValueChange();
void SetSingleStep();
void CheckGyro();
void AdjustGyro();
void SliderValue();
void SliderLinevalue();
void DifSet();
void AdjustController();
void SureAdjustController();
void RudderDisplayIniter();//舵量显示初始化
void RudderDisplayDriver();
void SPID_Adjust();
void CPID_Adjust();
void planemodechange();
public:
inline void update() {
    m_adi->update();
}

inline void setRoll(const float roll) {
    m_adi->setRoll( roll );
}

inline void setPitch(const float pitch) {
    m_adi->setPitch( pitch );
}
private:
void setupUi();
qfi::Adi* m_adi{};
LayoutSquare* m_layoutSq{};
planemode myplanemode;
public:
QString AileronValueInitstr;//存储副翼调参指令
QString ElevatorValueInitstr;//存储升降调参指令
QString RudderValueInitstr;//存储方向调参指令
QString PidValueInitstr_Pitch;//存储pitch的调参指令
QString PidValueInitstr_Roll;//存储roll的调参指令
QString GyroValueInitstr;
int RudderDisplay_1,RudderDisplay_2,RudderDisplay_3,RudderDisplay_4;
private slots:
void on_PIDChartpushButton_clicked();
};
#endif // WIDGET_H

