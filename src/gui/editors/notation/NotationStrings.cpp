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


#include "NotationStrings.h"
#include <QApplication>

#include <klocale.h>
#include "misc/Strings.h"
#include "document/ConfigGroups.h"
#include "base/Exception.h"
#include "base/NotationTypes.h"
#include "gui/configuration/GeneralConfigurationPage.h"
#include <QSettings>
#include <QString>


namespace Rosegarden
{

QString
NotationStrings::addDots(QString s, int dots,
                         bool hyphenate, bool internationalize)
{
    if (!dots)
        return s;

    if (internationalize) {
        if (dots > 1) {
            if (hyphenate)
                return QObject::tr("%1-dotted-%2").arg(dots).arg(s);
            else
                return QObject::tr("%1-dotted %2").arg(dots).arg(s);
        } else {
            if (hyphenate)
                return QObject::tr("dotted-%1").arg(s);
            else
                return QObject::tr("dotted %1").arg(s);
        }
    } else {
        if (dots > 1) {
            if (hyphenate)
                return QString("%1-dotted-%2").arg(dots).arg(s);
            else
                return QString("%1-dotted %2").arg(dots).arg(s);
        } else {
            if (hyphenate)
                return QString("dotted-%1").arg(s);
            else
                return QString("dotted %1").arg(s);
        }
    }
}

QString
NotationStrings::getNoteName(Note note, bool plural, bool triplet)
{
    Note::Type type = note.getNoteType();
    int dots = note.getDots();

    static const QString names[] = {
                                       QObject::tr("sixty-fourth note"), QObject::tr("thirty-second note"),
                                       QObject::tr("sixteenth note"), QObject::tr("eighth note"),
                                       QObject::tr("quarter note"), QObject::tr("half note"),
                                       QObject::tr("whole note"), QObject::tr("double whole note")
                                   };
    static const QString pluralnames[] = {
                                             QObject::tr("sixty-fourth notes"), QObject::tr("thirty-second notes"),
                                             QObject::tr("sixteenth notes"), QObject::tr("eighth notes"),
                                             QObject::tr("quarter notes"), QObject::tr("half notes"),
                                             QObject::tr("whole notes"), QObject::tr("double whole notes")
                                         };

    if (plural && triplet) {
        return addDots(QObject::tr("%1 triplets", names[type]), dots, false, true); // TODO PLURAL - this is broken because it assumes there's only 1 plural form
    } else if (plural) {
        return addDots(pluralnames[type], dots, false, true);
    } else if (triplet) {
        return addDots(QObject::tr("%1 triplet").arg(names[type]), dots, false, true);
    } else {
        return addDots(names[type], dots, false, true);
    }
}

QString
NotationStrings::getAmericanName(Note note, bool plural, bool triplet)
{
    Note::Type type = note.getNoteType();
    int dots = note.getDots();

    static const QString names[] = {
                                       "sixty-fourth note", "thirty-second note",
                                       "sixteenth note", "eighth note",
                                       "quarter note", "half note",
                                       "whole note", "double whole note"
                                   };
    static const QString pluralnames[] = {
                                             "sixty-fourth notes", "thirty-second notes",
                                             "sixteenth notes", "eighth notes",
                                             "quarter notes", "half notes",
                                             "whole notes", "double whole notes"
                                         };

    if (plural && triplet) {
        return addDots(QString("%1 triplets").arg(names[type]), dots, false, false);
    } else if (plural) {
        return addDots(pluralnames[type], dots, false, false);
    } else if (triplet) {
        return addDots(QString("%1 triplet").arg(names[type]), dots, false, false);
    } else {
        return addDots(names[type], dots, false, false);
    }
}

QString
NotationStrings::getShortNoteName(Note note, bool plural, bool triplet)
{
    Note::Type type = note.getNoteType();
    int dots = note.getDots();

    static const QString names[] = {
                                       QObject::tr("64th"), QObject::tr("32nd"), QObject::tr("16th"), QObject::tr("8th"),
                                       QObject::tr("quarter"), QObject::tr("half"), QObject::tr("whole"),
                                       QObject::tr("double whole")
                                   };
    static const QString pluralnames[] = {
                                             QObject::tr("64ths"), QObject::tr("32nds"), QObject::tr("16ths"), QObject::tr("8ths"),
                                             QObject::tr("quarters"), QObject::tr("halves"), QObject::tr("wholes"),
                                             QObject::tr("double wholes")
                                         };

    if (plural && triplet) {
        return addDots(QObject::tr("%1 triplets").arg(names[type]), dots, false, true); // TODO - this is broken because it assumes there's only 1 plural form
    } else if (plural) {
        return addDots(pluralnames[type], dots, false, true);
    } else if (triplet) {
        return addDots(QObject::tr("%1 triplet").arg(names[type]), dots, false, true);
    } else {
        return addDots(names[type], dots, false, true);
    }
}

QString
NotationStrings::getReferenceName(Note note, bool isRest)
{
    Note::Type type = note.getNoteType();
    int dots = note.getDots();

    static const QString names[] = {
                                       "hemidemisemi", "demisemi", "semiquaver",
                                       "quaver", "crotchet", "minim", "semibreve", "breve"
                                   };

    QString name(names[type]);
    if (isRest)
        name = "rest-" + name;
    return addDots(name, dots, true, false);
}

Note
NotationStrings::getNoteForName(QString name)
{
    std::string origName(qstrtostr(name));
    int pos = name.find('-');
    int dots = 0;

    if (pos > 0 && pos < 6 && pos < int(name.length()) - 1) {
        dots = name.left(pos).toInt();
        name = name.right(name.length() - pos - 1);
        if (dots < 2) {
            throw MalformedNoteName("Non-numeric or invalid dot count in \"" +
                                    origName + "\"");
        }
    }

    if (name.length() > 7 &&
            (name.left(7) == "dotted " || name.left(7) == "dotted-")) {
        if (dots == 0)
            dots = 1;
        name = name.right(name.length() - 7);
    } else {
        if (dots > 1) {
            throw MalformedNoteName("Dot count without dotted tag in \"" +
                                    origName + "\"");
        }
    }

    if (name.length() > 5 && name.right(5) == " note") {
        name = name.left(name.length() - 5);
    }

    Note::Type type;

    static const char *names[][4] = {
                                        { "64th", "sixty-fourth", "hemidemisemi", "hemidemisemiquaver"
                                        },
                                        { "32nd", "thirty-second", "demisemi", "demisemiquaver"	},
                                        { "16th", "sixteenth", "semi", "semiquaver"	},
                                        { "8th", "eighth", 0, "quaver"	},
                                        { "quarter", 0, 0, "crotchet", },
                                        { "half", 0, 0, "minim"	},
                                        { "whole", 0, 0, "semibreve"	},
                                        { "double whole", 0, 0, "breve"	}
                                    };

    for (type = Note::Shortest; type <= Note::Longest; ++type) {
        for (int i = 0; i < 4; ++i) {
            if (!names[type][i])
                continue;
            if (name == names[type][i])
                return Note(type, dots);
        }
    }

    throw MalformedNoteName("Can't parse note name \"" + origName + "\"");
}

QString
NotationStrings::makeNoteMenuLabel(timeT duration,
                                   bool brief,
                                   timeT &errorReturn,
                                   bool plural)
{
    Note nearestNote = Note::getNearestNote(duration);
    bool triplet = false;
    errorReturn = 0;

    if (duration == 0)
        return "0";

    if (nearestNote.getDuration() != duration) {
        Note tripletNote = Note::getNearestNote(duration * 3 / 2);
        if (tripletNote.getDuration() == duration * 3 / 2) {
            nearestNote = tripletNote;
            triplet = true;
        } else {
            errorReturn = duration - nearestNote.getDuration();
            duration = nearestNote.getDuration();
        }
    }

    QSettings settings;
    settings.beginGroup( GeneralOptionsConfigGroup );

    GeneralConfigurationPage::NoteNameStyle noteNameStyle =
            (GeneralConfigurationPage::NoteNameStyle) settings.value
            ("notenamestyle", GeneralConfigurationPage::Local).toUInt();

    settings.endGroup();

    if (brief) {

        timeT wholeNote = Note(Note::Semibreve).getDuration();
        if ((wholeNote / duration) * duration == wholeNote) {
            return QString("1/%1").arg(wholeNote / duration);
        } else if ((duration / wholeNote) * wholeNote == duration) {
            return QString("%1/1").arg(duration / wholeNote);
        } else if ((wholeNote /(duration*2/3)) * (duration*2/3) == wholeNote) {
            return QString("3/%1").arg(wholeNote / (duration*1/3));
        } else {
            return QObject::tr("%1 ticks").arg(duration);
            plural = false;
        }

    } else {
        QString noteName;

        switch (noteNameStyle) {

        case GeneralConfigurationPage::American:
            noteName = getAmericanName(nearestNote, plural, triplet);
            break;

        case GeneralConfigurationPage::Local:
            noteName = getNoteName(nearestNote, plural, triplet);
            break;
        }

        // Already internationalised, if appropriate
        return noteName;
    }
}

}
