/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*- vi:set ts=8 sts=4 sw=4: */

/*
    Rosegarden
    A MIDI and audio sequencer and musical notation editor.
    Copyright 2000-2014 the Rosegarden development team.
 
    Other copyrights also apply to some parts of this work.  Please
    see the AUTHORS file and individual file headers for details.
 
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.  See the file
    COPYING included with this distribution for more information.
*/

#ifndef RG_AUDIOPREVIEWPAINTER_H
#define RG_AUDIOPREVIEWPAINTER_H

#include "CompositionModelImpl.h"

#include <QPainter>
#include <QColor>

namespace Rosegarden {

class CompositionModelImpl;
class Composition;
class Segment;
class CompositionRect;

class AudioPreviewPainter {
public:
    AudioPreviewPainter(CompositionModelImpl& model,
			CompositionModelImpl::AudioPreviewData* apData,
			const Composition &composition,
			const Segment* segment);

    void paintPreviewImage();
    PixmapArray getPreviewImage();
    const CompositionRect& getSegmentRect() { return m_rect; }

    static int tileWidth();

protected:
    void initializeNewSlice();
    void finalizeCurrentSlice();

    //--------------- Data members ---------------------------------
    CompositionModelImpl& m_model;
    CompositionModelImpl::AudioPreviewData* m_apData;
    const Composition &m_composition;
    const Segment* m_segment;
    CompositionRect m_rect;

    QImage m_image;
    PixmapArray m_previewPixmaps;

    QPainter m_p;
    QPainter m_pb;
    QColor m_defaultCol;
    int m_penWidth;
    int m_height;
    int m_halfRectHeight;
    int m_sliceNb;

};

}

#endif

