// -*- c-basic-offset: 4 -*-

/*
    Rosegarden-4 v0.1
    A sequencer and musical notation editor.

    This program is Copyright 2000-2002
        Guillaume Laurent   <glaurent@telegraph-road.org>,
        Chris Cannam        <cannam@all-day-breakfast.com>,
        Richard Bown        <bownie@bownie.com>

    The moral right of the authors to claim authorship of this work
    has been asserted.

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.  See the file
    COPYING included with this distribution for more information.
*/

#ifndef _STAFF_H_
#define _STAFF_H_

#include "ViewElement.h"
#include "Segment.h"

namespace Rosegarden 
{

/**
 * Staff is the base class for classes which represent a Segment as an
 * on-screen graphic.  It manages the relationship between Segment/Event
 * and specific implementations of ViewElement.
 *
 * The template argument T must be a subclass of ViewElement.
 *
 * Staff was formerly known as ViewElementsManager.
 */

template <class T>
class Staff : public SegmentObserver
{
public: 
    virtual ~Staff();

    /**
     * Create a new ViewElementList wrapping all Events in the
     * segment, or return the previously created one
     */
    ViewElementList<T> *getViewElementList();

    /**
     * Create a new ViewElementList wrapping Events in the
     * [from, to[ interval, or return the previously created one
     * (even if passed new arguments)
     */
    ViewElementList<T> *getViewElementList(Segment::iterator from,
					   Segment::iterator to);

    /**
     * Return the Segment wrapped by this object 
     */
    Segment &getSegment() { return m_segment; }

    /**
     * Return the Segment wrapped by this object 
     */
    const Segment &getSegment() const { return m_segment; }

    /**
     * SegmentObserver method - called after the event has been added to
     * the segment
     */
    virtual void eventAdded(const Segment *, Event *);

    /**
     * SegmentObserver method - called after the event has been removed
     * from the segment, and just before it is deleted
     */
    virtual void eventRemoved(const Segment *, Event *);

    /**
     * SegmentObserver method - called after an event's start time
     * and/or duration change
     */
    virtual void eventQuantizationChanged(const Segment *, Event *);

protected:
    Staff(Segment &);

    /**
     * Return true if the event should be wrapped
     * Useful for piano roll where we only want to wrap notes
     * (always true by default)
     */
    virtual bool wrapEvent(Event *);

    Segment &m_segment;
    ViewElementList<T> *m_viewElementList;
    ViewElementList<T>::iterator findEvent(Rosegarden::Event *);
};



template <class T>
Staff<T>::Staff(Segment &t) :
    m_segment(t),
    m_viewElementList(0)
{
    // empty
}

template <class T>
Staff<T>::~Staff()
{
    if (m_viewElementList) m_segment.removeObserver(this);
}

template <class T>
ViewElementList<T> *
Staff<T>::getViewElementList()
{
    return getViewElementList(m_segment.begin(), m_segment.end());
}

template <class T>
ViewElementList<T> *
Staff<T>::getViewElementList(Segment::iterator from,
			     Segment::iterator to)
{
    if (!m_viewElementList) {

        m_viewElementList = new ViewElementList<T>;

        for (Segment::iterator i = from; i != to; ++i) {

            if (!wrapEvent(*i)) continue;

            T *el = new T(*i);
	    el->setAbsoluteTime(m_segment.getAbsoluteTimeOf(*i));
	    el->setDuration(m_segment.getDurationOf(*i));
            m_viewElementList->insert(el);
        }

        m_segment.addObserver(this);

    }
    
    return m_viewElementList;
}

template <class T>
bool
Staff<T>::wrapEvent(Event*)
{
    return true;
}

template <class T>
ViewElementList<T>::iterator
Staff<T>::findEvent(Event *e)
{
    T dummy(e);

    std::pair<ViewElementList<T>::iterator,
	      ViewElementList<T>::iterator>
        r = m_viewElementList->equal_range(&dummy);

    for (ViewElementList<T>::iterator i = r.first; i != r.second; ++i) {
        if ((*i)->event() == e) {
            return i;
        }
    }

    return m_viewElementList->end();
}


template <class T>
void
Staff<T>::eventAdded(const Segment *t, Event *e)
{
    assert(t == &m_segment);

    if (wrapEvent(e)) {
        T *el = new T(e);
	el->setAbsoluteTime(m_segment.getAbsoluteTimeOf(e));
	el->setDuration(m_segment.getDurationOf(e));
        m_viewElementList->insert(el);
    }
}

template <class T>
void
Staff<T>::eventRemoved(const Segment *t, Event *e)
{
    assert(t == &m_segment);

    // If we have it, lose it

    ViewElementList<T>::iterator i = findEvent(e);
    if (i != m_viewElementList->end()) {
        m_viewElementList->erase(i);
        return;
    }
}

template <class T>
void
Staff<T>::eventQuantizationChanged(const Segment *t, Event *e)
{
    assert(t == &m_segment);

    ViewElementList<T>::iterator i = findEvent(e);
    if (i != m_viewElementList->end()) {
	m_viewElementList->erase(i);
	T *el = new T(e);
	el->setAbsoluteTime(m_segment.getAbsoluteTimeOf(e));
	el->setDuration(m_segment.getDurationOf(e));
	m_viewElementList->insert(el);
    }
}

}

#endif

