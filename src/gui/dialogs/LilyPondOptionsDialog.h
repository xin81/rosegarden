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

#ifndef _RG_LILYPONDOPTIONSDIALOG_H_
#define _RG_LILYPONDOPTIONSDIALOG_H_

#include <QDialog>
#include <QString>

#include "gui/configuration/HeadersConfigurationPage.h"

class QWidget;
class QCheckBox;
class QComboBox;

namespace Rosegarden
{

class RosegardenDocument;
class HeadersConfigurationPage;

class LilyPondOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    LilyPondOptionsDialog(QWidget *parent,
			  RosegardenDocument *doc,
                          QString windowCaption = "",
                          QString heading = "");

    static void setDefaultLilyPondVersion(QString version);

public slots:
    void slotApply();
    void accept();

protected:
    RosegardenDocument *m_doc;
    QComboBox *m_lilyLanguage;
    QComboBox *m_lilyPaperSize;
    QComboBox *m_lilyFontSize;
    QComboBox *m_lilyTempoMarks;
    QComboBox *m_lilyExportSelection;
    QComboBox *m_lilyExportLyrics;
    QCheckBox *m_lilyPaperLandscape;
    QCheckBox *m_lilyRaggedBottom;
    QCheckBox *m_lilyChordNamesMode;
    QCheckBox *m_lilyExportBeams;
    QCheckBox *m_lilyExportStaffGroup;
    QComboBox *m_lilyMarkerMode;
    HeadersConfigurationPage *m_headersPage;

    void populateDefaultValues();

    static const unsigned int FONT_OFFSET = 6;
    static const unsigned int FONT_20 = 20 + FONT_OFFSET;

};



}

#endif
