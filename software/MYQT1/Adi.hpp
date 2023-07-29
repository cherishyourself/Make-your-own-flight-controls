/***************************************************************************//**
 * @file qfi_ADI.h
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
#ifndef __qfi_Adi_H__
#define __qfi_Adi_H__
#include <QHBoxLayout>
#include <QGraphicsView>
class QWidget;
class QResizeEvent;
class QGraphicsScene;
class QGraphicsSvgItem;

namespace qfi {

//---------------------------------------------------
// Class: Adi
// Description: Attitude Director Indicator
//---------------------------------------------------
class Adi : public QGraphicsView
{
    Q_OBJECT

public:
    Adi(QWidget* parent = nullptr);
    virtual ~Adi();

    // reinitiates widget
    void reinit();

    // refreshes (redraws) widget
    void update();

    void setRoll(const float);
    void setPitch(const float);

protected:
    void resizeEvent(QResizeEvent*);

private:
    void init();
    void reset();
    void updateView();

    QGraphicsScene* m_scene{};

    QGraphicsSvgItem* m_itemBack{};
    QGraphicsSvgItem* m_itemFace{};
    QGraphicsSvgItem* m_itemRing{};
    QGraphicsSvgItem* m_itemCase{};

    float m_roll{};
    float m_pitch{};

    float m_faceDeltaX_new{};
    float m_faceDeltaX_old{};
    float m_faceDeltaY_new{};
    float m_faceDeltaY_old{};

    float m_scaleX{1.0f};
    float m_scaleY{1.0f};

    const int m_originalHeight{240};
    const int m_originalWidth{240};

    const float m_originalPixPerDeg{1.7f};

    QPointF m_originalAdiCtr{122.0f, 122.0f};

    const int m_backZ{-30};
    const int m_faceZ{-20};
    const int m_ringZ{-10};
    const int m_caseZ{10};
};

}

#endif
