/*                         M A I N W I N D O W . H
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
/** @file MainWindow.h
 *
 *  BRL-CAD Qt GUI:
 *      the main window class declaration
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <brlcad/ConstDatabase.h>

#include "ObjectsTreeView.h"
#include "CameraView.h"
#include "GraphicView.h"
#include "LogView.h"


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(const char* file,
               QWidget*    parent = 0);

private:
    BRLCAD::ConstDatabase m_database;
    GraphicView*          m_graphicView;
    ObjectsTreeView*      m_objectsTreeView;
    CameraView*           m_cameraView;
    LogView*              m_logView;

    void LoadDatabase(const char* fileName);

private slots:
    void OpenFile(void);
};


#endif // MAINWINDOW_H
