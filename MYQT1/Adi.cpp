/***************************************************************************//**
 * @file qfi_ADI.cpp
 * @author  Marek M. Cel <marekcel@marekcel.pl>
 *
 * @section LICENSE
 *
 * Copyright (C) 2013 Marek M. Cel
 *
 * This file is part of QFlightInstruments. You can redistribute and modify it
 * under the terms of GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Further information about the GNU General Public License can also be found
 * on the world wide web at http://www.gnu.org.
 *
 * ---
 *
 * Copyright (C) 2013 Marek M. Cel
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/

#include "Adi.hpp"
#include <QGraphicsSvgItem>
#include <cmath>
#include <QDebug>
#include <QRect>
namespace qfi {
Adi::Adi(QWidget* parent) : QGraphicsView (parent)
{
    m_scene = new QGraphicsScene( this );//创立一个场景m_scene
    setSceneRect(0,0,300,300);
    setScene( m_scene );//把此场景设置为场景
    m_scene->clear();//清空创立的场景
    init();//初始化
}

Adi::~Adi()
{
    if (m_scene) {
        m_scene->clear();
        delete m_scene;
        m_scene = nullptr;
    }
    reset();
}

void Adi::reinit()
{
    if (m_scene) {
        m_scene->clear();
        init();
    }
}

void Adi::update()//外部调用更新
{
    updateView();
    m_faceDeltaX_old  = m_faceDeltaX_new;
    m_faceDeltaY_old  = m_faceDeltaY_new;
}

void Adi::setRoll(const float roll)//更新roll 需要外部调用
{
    m_roll = roll;
    if ( m_roll < -180.0f ) m_roll = -180.0f;
    if ( m_roll >  180.0f ) m_roll =  180.0f;
}

void Adi::setPitch(const float pitch)//更新pitch 需要外部调用
{
    m_pitch = pitch;

    if ( m_pitch < -90.0f ) m_pitch = -90.0f;
    if ( m_pitch >  90.0f ) m_pitch =  90.0f;
}

void Adi::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent( event );
    reinit();
}
void Adi::init()//初始化
{//            static_cast 强制类型转换
    m_scaleX = static_cast<float>(width()) / static_cast<float>(m_originalWidth);//设置x轴的宽度  这里不明白为什么这样写
    m_scaleY = static_cast<float>(height()) / static_cast<float>(m_originalHeight);//设置y轴的高度

    reset();//格式化

    m_itemBack = new QGraphicsSvgItem( ":/qfi/image/adi_back.svg" );//把创建一个类存储矢量图
    m_itemBack->setCacheMode( QGraphicsItem::NoCache );//设置缓存模式，缓存图形加快渲染速度，此处禁用缓存
    m_itemBack->setZValue( m_backZ );//设置同级项目的堆叠顺序
    m_itemBack->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );//映射矩阵，将项目的坐标系映射到其父项
    m_itemBack->setTransformOriginPoint( m_originalAdiCtr );//在项目坐标中设置转换的原点。
    m_scene->addItem( m_itemBack );//在场景中加入此图项

    m_itemFace = new QGraphicsSvgItem( ":/qfi/image/adi_face.svg" );
    m_itemFace->setCacheMode( QGraphicsItem::NoCache );
    m_itemFace->setZValue( m_faceZ );
    m_itemFace->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemFace->setTransformOriginPoint( m_originalAdiCtr );
    m_scene->addItem( m_itemFace );

    m_itemRing = new QGraphicsSvgItem( ":/qfi/image/adi_ring.svg" );
    m_itemRing->setCacheMode( QGraphicsItem::NoCache );
    m_itemRing->setZValue( m_ringZ );
    m_itemRing->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemRing->setTransformOriginPoint( m_originalAdiCtr );
    m_scene->addItem( m_itemRing );

    m_itemCase = new QGraphicsSvgItem( ":/qfi/image/adi_case.svg" );
    m_itemCase->setCacheMode( QGraphicsItem::NoCache );
    m_itemCase->setZValue( m_caseZ );
    m_itemCase->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemRing->setTransformOriginPoint( m_originalAdiCtr );
    m_scene->addItem( m_itemCase );

    centerOn( width() / 2.0f , height() / 2.0f );//确保场景坐标在视图中居中

    updateView();//更新视图，包含场景的旋转和移动
}

void Adi::reset()
{
    m_itemBack = nullptr;//把四个svg图形清空
    m_itemFace = nullptr;
    m_itemRing = nullptr;
    m_itemCase = nullptr;

    m_roll  = 0.0f;//roll和pitch清空
    m_pitch = 0.0f;

    m_faceDeltaX_new = 0.0f;//x，y轴清空 具体是什么暂时不清楚
    m_faceDeltaX_old = 0.0f;
    m_faceDeltaY_new = 0.0f;
    m_faceDeltaY_old = 0.0f;
}

void Adi::updateView()
{
    m_scaleX = static_cast<float>(width())  / static_cast<float>(m_originalWidth);
    m_scaleY = static_cast<float>(height()) / static_cast<float>(m_originalHeight);

    m_itemBack->setRotation(-m_roll);//设置绕z轴顺时针旋转角度，单位为角度 根据实际情况修改
    m_itemFace->setRotation(-m_roll);
    m_itemRing->setRotation(-m_roll);
qDebug()<<"roll"<<m_roll<<"pitch"<<m_pitch;
    const float roll_rad{static_cast<float>(M_PI * m_roll / 180.0)};//弧度转角度 （弧度*π）/180
    const float delta{static_cast<float>(m_originalPixPerDeg * m_pitch)};//不知道干什么的

    m_faceDeltaX_new = m_scaleX * delta * std::sin( roll_rad );//？？
    m_faceDeltaY_new = m_scaleY * delta * std::cos( roll_rad );//？？

    m_itemFace->moveBy( m_faceDeltaX_new - m_faceDeltaX_old, m_faceDeltaY_new - m_faceDeltaY_old );
    //将项目水平移动 dx 点，垂直移动 dy 点。
    m_scene->update();//更新场景
}

}
