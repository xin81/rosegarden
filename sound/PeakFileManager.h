// -*- c-basic-offset: 4 -*-

/*
    Rosegarden-4
    A sequencer and musical notation editor.

    This program is Copyright 2000-2004
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


// Accepts an AudioFIle and turns the sample data into peak data for
// storage in a peak file or a BWF format peak chunk.  Pixmaps or
// sample data is returned to callers on demand using these cached
// values.
//
//

#ifndef _PEAKFILEMANAGER_H_
#define _PEAKFILEMANAGER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <qobject.h>


#include "PeakFile.h"

namespace Rosegarden
{

class AudioFile;
class RealTime;

class PeakFileManager : public QObject
{
    Q_OBJECT
public:
    // updatePercentage tells this object how often to throw a
    // percentage complete message - active between 0-100 only
    // if it's set to 5 then we send an update exception every
    // five percent.  The percentage complete is sent with 
    // each exception.
    //
    PeakFileManager();
    virtual ~PeakFileManager();

private:
    PeakFileManager(const PeakFileManager &pFM);
    PeakFileManager& operator=(const PeakFileManager &);

public:
    // Check that a given audio file has a valid and up to date
    // peak file or peak chunk.
    //
    //
    bool hasValidPeaks(AudioFile *audioFile);

    // Generate a peak file from file details - if the peak file already
    // exists _and_ it's up to date then we don't do anything.  For BWF
    // files we generate an internal peak chunk.
    //
    //
    void generatePeaks(AudioFile *audioFile,
                       unsigned short updatePercentage);

    // Get a vector of floats as the preview
    //
    std::vector<float> getPreview(AudioFile *audioFile,
                                  const RealTime &startTime,
                                  const RealTime &endTime,
                                  int   width,
                                  bool  showMinima);
    
    // Remove cache for a single audio file (if audio file to be deleted etc)
    // 
    bool removeAudioFile(AudioFile *audioFile);

    // Clear down
    //
    void clear();
                    
    // Get split points for a peak file
    //
    std::vector<SplitPointPair> 
        getSplitPoints(AudioFile *audioFile,
                       const RealTime &startTime,
                       const RealTime &endTime,
                       int threshold,
                       const RealTime &minTime);

    std::vector<PeakFile*>::const_iterator begin() const
                { return m_peakFiles.begin(); }

    std::vector<PeakFile*>::const_iterator end() const
                { return m_peakFiles.end(); }

    // Stop a preview during its build
    //
    void stopPreview();

signals:
    void setProgress(int);

protected:

    // Add and remove from our PeakFile cache
    //
    bool insertAudioFile(AudioFile *audioFile);
    PeakFile* getPeakFile(AudioFile *audioFile);

    std::vector<PeakFile*> m_peakFiles;
    unsigned short m_updatePercentage;  // how often we send updates 

    // Whilst processing - the current PeakFile
    //
    PeakFile              *m_currentPeakFile;


};


}


#endif // _PEAKFILEMANAGER_H_


