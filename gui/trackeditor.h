/***************************************************************************
                          trackseditor.h  -  description
                             -------------------
    begin                : Mon May 7 2001
    copyright            : (C) 2001 by Guillaume Laurent, Chris Cannam, Rich Bown
    email                : glaurent@telegraph-road.org, cannam@all-day-breakfast.com, bownie@bownie.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TRACKSEDITOR_H
#define TRACKSEDITOR_H

#include <list>
#include <qwidget.h>
#include <qheader.h>

class TrackPart;
class TracksCanvas;

/**Global widget for track edition.
Shows a global overview of the composition.

  *@author Guillaume Laurent, Chris Cannam, Rich Bown
  */

class TracksEditor : public QWidget  {
   Q_OBJECT
public: 
    TracksEditor(unsigned int nbTracks,
                 unsigned int nbBars,
                 QWidget *parent = 0, const char *name = 0,
                 WFlags f=0);

    void clear();

    bool moveTrack(int section, int fromIdx, int toIdx);
    TrackPart* getTrackAtIdx(int idx);

protected slots:
    void trackOrderChanged(int section, int fromIdx, int toIdx);
    void addTrackPart(TrackPart*);
    void deleteTrackPart(TrackPart*);

signals:
    void needUpdate();

protected:
    TracksCanvas *m_tracksCanvas;
    QHeader *m_hHeader, *m_vHeader;

    list<TrackPart*> m_trackParts;
};

#endif
