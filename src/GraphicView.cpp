/*                            G R A P H I C V I E W . C P P
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
/** @file GraphicView.cpp
 *
 *  BRL-CAD Qt GUI:
 *      implementation of the graphical visualization
 */

#include <cmath>

#include <QPainter>

#include "GraphicView.h"


GraphicView::GraphicView
(
    BRLCAD::ConstDatabase& database,
    QWidget*               parent
) : QWidget(parent),
    m_database(database),
    m_transformation(),
    m_image(),
    m_imageUpTodate(false),
    m_updatingImage(false) {
    setMinimumSize(100, 100);
}


void GraphicView::Update(void) {
    m_imageUpTodate = false;

    update();
}


void GraphicView::UpdateTrafo(const QMatrix4x4& transformation) {
    if (transformation != m_transformation) {
        m_transformation = transformation;

        Update();
    }
}


void GraphicView::paintEvent
(
    QPaintEvent*
) {
    if (!m_imageUpTodate && !m_updatingImage) {
        m_updatingImage = true;
        UpdateImage();
        m_imageUpTodate = true;
        m_updatingImage = false;
    }

    QPainter painter(this);
    painter.drawImage(0, 0, m_image);
}


class RayTraceCallback : public BRLCAD::ConstDatabase::HitCallback {
public:
    RayTraceCallback(const QVector3D& direction) : BRLCAD::ConstDatabase::HitCallback(), m_direction(direction), m_color(Qt::white) {}

    virtual bool operator()(const BRLCAD::ConstDatabase::Hit& hit) throw() {
        double    brightness         = 0;
        double    ambient            = 0.1;
        double    diffuseWeight      = 0.5;
        double    specularWeight     = 0.5;
        int       n                  = 4;
        QVector3D normal             = QVector3D(hit.SurfaceNormalIn().coordinates[0], hit.SurfaceNormalIn().coordinates[1], hit.SurfaceNormalIn().coordinates[2]);
        double    dotProduct         = QVector3D::dotProduct(m_direction, normal); // negative because of opposite directions

        // value from 0 to 1
        double    diffuse            = -dotProduct;

        // refleced = incidence - 2 normal
        QVector3D reflectedDir       = m_direction - 2. * dotProduct * normal;
        reflectedDir.normalize();

        // value from 0 to 1
        double    reflectedDotCamDir = std::max(0.f, QVector3D::dotProduct(reflectedDir, m_direction));
        double    specular           = pow(reflectedDotCamDir, n);

        brightness += ambient + diffuse * diffuseWeight;

        double    red                = std::min(hit.Red() * brightness + specular * specularWeight, 1.0);
        double    green              = std::min(hit.Green() * brightness + specular * specularWeight, 1.0);
        double    blue               = std::min(hit.Blue() * brightness + specular * specularWeight, 1.0);

        m_color.setRgbF(red, green, blue);

        return false;
    }

    const QColor& Color(void) const {
        return m_color;
    }

private:
    QVector3D m_direction;
    QColor    m_color;
};


void GraphicView::UpdateImage(void) {
    int w  = width();
    int h = height();

    m_image = QImage(w, h, QImage::Format_RGB32);

    QVector3D directionStart = m_transformation.map(QVector3D(0., 0., 1.));
    QVector3D directionEnd   = m_transformation.map(QVector3D(0., 0., 0.));
    QVector3D direction      = directionEnd - directionStart;
    direction.normalize();

    for (int column = 0; column < w; column++) {
        for (int row = 0; row < h; row++) {
            QVector3D        imagePoint(column, h - row - 1., 0.);
            QVector3D        modelPoint = m_transformation.map(imagePoint);
            RayTraceCallback callback(direction);
            BRLCAD::Ray3D    ray;

            ray.origin.coordinates[0]    = modelPoint.x();
            ray.origin.coordinates[1]    = modelPoint.y();
            ray.origin.coordinates[2]    = modelPoint.z();
            ray.direction.coordinates[0] = direction.x();
            ray.direction.coordinates[1] = direction.y();
            ray.direction.coordinates[2] = direction.z();

            m_database.ShootRay(ray, callback, BRLCAD::ConstDatabase::StopAfterFirstHit);

            m_image.setPixelColor(column, row, callback.Color());
        }
    }
}
