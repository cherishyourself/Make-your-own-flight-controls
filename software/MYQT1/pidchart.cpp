#include "pidchart.h"
#include "ui_pidchart.h"
#include "QChartView"
#include "QLineSeries"
#include <QValueAxis>
#include "QtMath"
QT_CHARTS_USE_NAMESPACE
PIDChart::PIDChart(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PIDChart)
{
    ui->setupUi(this);
    mycharts();
}

PIDChart::~PIDChart()
{
    delete ui;
}
void PIDChart::mycharts()
{
    //创建图表
    QChartView *chartview=new QChartView;
    QChart *chart=new QChart;
    chart->setTitle("PID实时曲线显示");
    chartview->setChart(chart);
    setCentralWidget(chartview);

    //创建折线图
    QLineSeries *seriesC0=new QLineSeries;
    QLineSeries *seriesC1=new QLineSeries;
    seriesC0->setName("期望值");
    seriesC1->setName("实际值");
    chart->addSeries(seriesC0);
    chart->addSeries(seriesC1);
    qreal Flag1=50,Ture1=0,count=100,intv=0.1,t=0;
    for(int i=0;i<count;i++)
    {
        seriesC0->append(t,Flag1);
        seriesC1->append(t,Ture1);
        t+=intv;
    }
    QValueAxis *axisX=new QValueAxis;
    axisX->setRange(0,100);
    QValueAxis *axisY=new QValueAxis;
    axisY->setRange(-50,50);

    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    seriesC0->attachAxis(axisX);
    seriesC0->attachAxis(axisY);
    seriesC1->attachAxis(axisX);
    seriesC1->attachAxis(axisY);
}
