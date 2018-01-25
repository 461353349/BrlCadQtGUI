/*                       M A I N W I N D O W . C P P
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
/** @file MainWindow.cpp
 *
 *  BRL-CAD Qt GUI:
 *      the main window class implementation
 */

#include <QAction>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QDockWidget>

#include "MainWindow.h"


MainWindow::MainWindow
(
    const char* file,
    QWidget*    parent
) : QMainWindow(parent),
    m_database() {
    // create menus for menuBar
    QAction* fileOpenAction = new QAction(tr("&Open File"));
    fileOpenAction->setShortcuts(QKeySequence::Open);
    fileOpenAction->setStatusTip(tr("Open an Existing File"));
    connect(fileOpenAction, &QAction::triggered, this, &MainWindow::OpenFile);

    // adding actions to QMenu
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(fileOpenAction);

    // the drawing area
    m_graphicView = new GraphicView(m_database);
    setCentralWidget(m_graphicView);

    // the objects tree
    QDockWidget* objectsWidget = new QDockWidget(tr("Object Selection"));

    m_objectsTreeView = new ObjectsTreeView(m_database);
    objectsWidget->setWidget(m_objectsTreeView);
    addDockWidget(Qt::LeftDockWidgetArea, objectsWidget);

    // the projection control
    QDockWidget* cameraWidget = new QDockWidget(tr("Projection Settings"));

    m_cameraView = new CameraView(m_database, m_graphicView);
    cameraWidget->setWidget(m_cameraView);
    addDockWidget(Qt::RightDockWidgetArea, cameraWidget);

    // display the logged information
    QDockWidget* logWidget = new QDockWidget(tr("Logging"));

    m_logView = new LogView();
    logWidget->setWidget(m_logView);
    addDockWidget(Qt::BottomDockWidgetArea, logWidget);

    connect(m_objectsTreeView, &ObjectsTreeView::SelectionChanged, m_graphicView, &GraphicView::Update);
    connect(m_cameraView, &CameraView::Changed, m_graphicView, &GraphicView::UpdateTrafo);

    setWindowTitle(tr("BRL-CAD Qt GUI"));

    if (file != 0)
        LoadDatabase(file);
}


void MainWindow::OpenFile(void) {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open BRL-CAD database"), QString(), "BRL-CAD Database (*.g)");

    LoadDatabase(filePath.toUtf8().data());
}


void MainWindow::LoadDatabase
(
    const char* fileName
) {
    if (m_database.Load(fileName))  {
        QString title = m_database.Title();

        title += " [";
        title += fileName;
        title += "]";

        setWindowTitle(title);
        m_objectsTreeView->Rebuild();
        m_cameraView->Reset();
    }
}
