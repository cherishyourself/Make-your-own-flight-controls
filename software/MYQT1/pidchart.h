#ifndef PIDCHART_H
#define PIDCHART_H

#include <QMainWindow>

namespace Ui {
class PIDChart;
}

class PIDChart : public QMainWindow
{
    Q_OBJECT

public:
    explicit PIDChart(QWidget *parent = nullptr);
    void mycharts();
    ~PIDChart();

private:
    Ui::PIDChart *ui;
};

#endif // PIDCHART_H
