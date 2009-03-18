/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*- vi:set ts=8 sts=4 sw=4: */

/*
    Rosegarden
    A MIDI and audio sequencer and musical notation editor.
    Copyright 2000-2009 the Rosegarden development team.
 
    Other copyrights also apply to some parts of this work.  Please
    see the AUTHORS file and individual file headers for details.
 
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.  See the file
    COPYING included with this distribution for more information.
*/


#include "MatrixElement.h"
#include "MatrixScene.h"
#include "misc/Debug.h"
#include "base/RulerScale.h"

#include <QGraphicsRectItem>
#include <QGraphicsPolygonItem>
#include <QBrush>
#include <QColor>

#include "base/Event.h"
#include "base/NotationTypes.h"
#include "base/BaseProperties.h"
#include "gui/general/GUIPalette.h"
#include "gui/rulers/DefaultVelocityColour.h"


namespace Rosegarden
{

static const int MatrixElementData = 2;

MatrixElement::MatrixElement(MatrixScene *scene, Event *event, bool drum) :
    ViewElement(event),
    m_scene(scene),
    m_drum(drum),
    m_item(0)
{
    reconfigure();
}

MatrixElement::~MatrixElement()
{
    delete m_item;
}

void
MatrixElement::reconfigure()
{
    timeT time = event()->getAbsoluteTime();
    timeT duration = event()->getDuration();

    reconfigure(time, duration);
}

void
MatrixElement::reconfigure(timeT time, timeT duration)
{
    long pitch = 60;
    event()->get<Int>(BaseProperties::PITCH, pitch);

    reconfigure(time, duration, pitch);
}

void
MatrixElement::reconfigure(timeT time, timeT duration, int pitch)
{
    long velocity = 100;
    event()->get<Int>(BaseProperties::VELOCITY, velocity);

    reconfigure(time, duration, pitch, velocity);
}

void
MatrixElement::reconfigure(timeT time, timeT duration, int pitch, int velocity)
{
    const RulerScale *scale = m_scene->getRulerScale();
    int resolution = m_scene->getYResolution();

    double x0 = scale->getXForTime(time);
    double x1 = scale->getXForTime(time + duration);
    m_width = x1 - x0;

    QColor colour;
    if (event()->has(BaseProperties::TRIGGER_SEGMENT_ID)) {
        colour = Qt::gray;
    } else {
        colour = DefaultVelocityColour::getInstance()->getColour(velocity);
    }
    colour.setAlpha(160);

    double fres(resolution);

    if (m_drum) {
        QGraphicsPolygonItem *item = dynamic_cast<QGraphicsPolygonItem *>(m_item);
        if (!item) {
            delete m_item;
            item = new QGraphicsPolygonItem;
            m_item = item;
            m_scene->addItem(m_item);
        }
        QPolygonF polygon;
        polygon << QPointF(0.5, 0.5)
                << QPointF(fres/2, fres/2)
                << QPointF(0.5, fres)
                << QPointF(-fres/2, fres/2)
                << QPointF(0.5, 0.5);
        item->setPolygon(polygon);
        item->setPen
            (QPen(GUIPalette::getColour(GUIPalette::MatrixElementBorder), 0));
        item->setBrush(colour);
    } else {
        QGraphicsRectItem *item = dynamic_cast<QGraphicsRectItem *>(m_item);
        if (!item) {
            delete m_item;
            item = new QGraphicsRectItem;
            m_item = item;
            m_scene->addItem(m_item);
        }
        float width = m_width;
        if (width < 1) width = 1;
        QRectF rect(0.5, 0.5, width, fres + 1);
        item->setRect(rect);
        item->setPen
            (QPen(GUIPalette::getColour(GUIPalette::MatrixElementBorder), 0));
        item->setBrush(colour);
    }

    setLayoutX(x0);

    m_item->setData(MatrixElementData, QVariant::fromValue((void *)this));
    m_item->setPos(x0, (127 - pitch) * (resolution + 1));
}

bool
MatrixElement::isNote() const
{
    return event()->isa(Note::EventType);
}

void
MatrixElement::setSelected(bool selected)
{
    QAbstractGraphicsShapeItem *item =
        dynamic_cast<QAbstractGraphicsShapeItem *>(m_item);
    if (!item) return;

    QColor colour;

    if (selected) {

        colour = GUIPalette::getColour(GUIPalette::SelectedElement);

    } else if (event()->has(BaseProperties::TRIGGER_SEGMENT_ID)) {

        colour = Qt::gray;

    } else {

        long velocity = 100;
        event()->get<Int>(BaseProperties::VELOCITY, velocity);
        colour = DefaultVelocityColour::getInstance()->getColour(velocity);
    }

    colour.setAlpha(160);

    item->setBrush(colour);
}

MatrixElement *
MatrixElement::getMatrixElement(QGraphicsItem *item)
{
    QVariant v = item->data(MatrixElementData);
    if (v.isNull()) return 0;
    return (MatrixElement *)v.value<void *>();
}


}
