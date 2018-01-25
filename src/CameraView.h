/*                           C A M E R A V I E W . H
 * BRL-CAD
 *
 * Copyright (c) 2018 United States Government as represented by
 * the U.S. Army Research Laboratory.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/** @file CameraView.h
 *
 *  BRL-CAD Qt GUI:
 *      declaration projection control view
 */

#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QDial>
#include <QMatrix4x4>
#include <QTimer>

#include <brlcad/ConstDatabase.h>


class CameraView : public QWidget {
    Q_OBJECT
public:
    CameraView(BRLCAD::ConstDatabase& database,
               QWidget*               graphicView,
               QWidget*               parent = 0);

public slots:
    void Reset(void);

signals:
    void Changed(const QMatrix4x4& transformation);

private:
    BRLCAD::ConstDatabase& m_database;
    QWidget*               m_graphicView;
    QTimer*                m_timer;
    QLineEdit*             m_x;
    QLineEdit*             m_y;
    QLineEdit*             m_sX;
    QLineEdit*             m_sY;
    QLineEdit*             m_z;
    QLineEdit*             m_scale;
    QLineEdit*             m_distance;
    QDial*                 m_xDial;
    QDial*                 m_yDial;
    QDial*                 m_zDial;
    QLineEdit*             m_xRot;
    QLineEdit*             m_yRot;
    QLineEdit*             m_zRot;

private slots:
    void TimeOut(void);
    void RotationFieldChanged();
    void RotationDialChanged();
};


#endif // CAMERAVIEW_H
