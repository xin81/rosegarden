
/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*- vi:set ts=8 sts=4 sw=4: */

/*
    Rosegarden
    A MIDI and audio sequencer and musical notation editor.

    This program is Copyright 2000-2007
        Guillaume Laurent   <glaurent@telegraph-road.org>,
        Chris Cannam        <cannam@all-day-breakfast.com>,
        Richard Bown        <richard.bown@ferventsoftware.com>

    The moral rights of Guillaume Laurent, Chris Cannam, and Richard
    Bown to claim authorship of this work have been asserted.

    Other copyrights also apply to some parts of this work.  Please
    see the AUTHORS file and individual file headers for details.

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.  See the file
    COPYING included with this distribution for more information.
*/

#ifndef _RG_MATRIXELEMENT_H_
#define _RG_MATRIXELEMENT_H_

#include "viewelement/ViewElement.h"
#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsRectItem>

class QColor;


namespace Rosegarden
{

class Event;

class MatrixElement : public ViewElement, public QGraphicsRectItem
{

public:
    MatrixElement(Event *event);

    virtual ~MatrixElement();

    void setCanvas(QGraphicsScene* c);

    /**
     * Returns the actual x coordinate of the element on the canvas
     */
    double getCanvasX() const { return x(); }

    /**
     * Returns the actual y coordinate of the element on the canvas
     */
    double getCanvasY() const { return y(); }

    double getCanvasZ() const { return zValue(); }

    /**
     * Sets the x coordinate of the element on the canvas
     */
    void setCanvasPos(double x, double y) { setPos(x, y); }


    /**
     * Sets the width of the rectangle on the canvas
     */
    void setWidth(qreal w)   { QRectF r = rect(); r.setWidth(w); setRect(r); }
    qreal getWidth() { return rect().width(); }

    /**
     * Sets the height of the rectangle on the canvas
     */
    void setHeight(qreal h)   { QRectF r = rect(); r.setHeight(h); setRect(r); }
    qreal getHeight() { return rect().height(); }

    /// Returns true if the wrapped event is a note
    bool isNote() const;

    /*
     * Set the colour of the element
     */
    void setColour(const QColor &colour)
        { setBrush(QBrush(colour)); }

    /**
     * If element rectangle is currently visible gets its size and returns true.
     * Returns false if element rectangle is undefined or not visible.
     */
    bool getVisibleRectangle(QRectF &rectangle);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    
    
protected:

};


typedef ViewElementList MatrixElementList;


}

#endif
