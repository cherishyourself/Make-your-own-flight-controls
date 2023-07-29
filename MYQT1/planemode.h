#ifndef PLANEMODE_H
#define PLANEMODE_H
#include <QHBoxLayout>
#include <QObject>
#include <QWidget>
#include <QGraphicsView>
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSvgItem;
class planemode : public QGraphicsView
{
    Q_OBJECT
public:
    explicit planemode(QWidget *parent = nullptr);
    void plane_fly();
    void plane_fixed();
signals:

private:
    QGraphicsScene *m_planemode{};
};

#endif // PLANEMODE_H
