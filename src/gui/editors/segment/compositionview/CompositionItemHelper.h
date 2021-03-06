
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

#ifndef RG_COMPOSITIONITEMHELPER_H
#define RG_COMPOSITIONITEMHELPER_H

#include "CompositionModelImpl.h"
#include "base/Event.h"

namespace Rosegarden
{


class SnapGrid;
class Segment;

class CompositionItemHelper {
public:
    static timeT getStartTime(CompositionItemPtr, const SnapGrid&);
    static timeT getEndTime(CompositionItemPtr, const SnapGrid&);
    static int getTrackPos(CompositionItemPtr, const SnapGrid&);
    static void setStartTime(CompositionItemPtr, timeT, const SnapGrid&);
    static void setEndTime(CompositionItemPtr, timeT, const SnapGrid&);
    static Segment* getSegment(CompositionItemPtr);
    static CompositionItemPtr makeCompositionItem(Segment*);
    /**
     * return the CompositionItemPtr in the model which references the same segment as referenceItem
     */
    static CompositionItemPtr findSiblingCompositionItem(const CompositionModelImpl::ItemContainer& items, CompositionItemPtr referenceItem);

};


}

#endif
