#include "planemode.h"
#include "QGraphicsView"
#include "QGraphicsSvgItem"
#include "QDebug"
#include "QRect"
planemode::planemode(QWidget *parent) : QGraphicsView(parent)
{
m_planemode=new QGraphicsScene (this);

}

void planemode::plane_fly()
{

}

void planemode::plane_fixed()
{

}
