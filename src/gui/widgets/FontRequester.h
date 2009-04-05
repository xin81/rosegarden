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

#ifndef _RG_FONTREQUESTER_H_
#define _RG_FONTREQUESTER_H_

#include <QWidget>
#include <QFont>

class QLabel;

namespace Rosegarden
{

/**
 * A widget including a label with a font name in it, written in that
 * font, and a push button calling up a font selection dialog.  A
 * replacement for KFontRequester
 */
class FontRequester : public QWidget
{
    Q_OBJECT

public:
    FontRequester(QWidget *parent = 0);
    virtual ~FontRequester();

    void setFont(QFont);
    QFont getFont() const;

signals:
    void fontChanged(QFont);

protected slots:
    void slotChoose();
    
protected:
    QLabel *m_label;
};

}

#endif