/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*- vi:set ts=8 sts=4 sw=4: */

/*
    Rosegarden
    A MIDI and audio sequencer and musical notation editor.

    This program is Copyright 2000-2009
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

#ifndef _RG_COMMAND_H_
#define _RG_COMMAND_H_

#include <QString>

#include <vector>

namespace Rosegarden
{

class Command
{
public:
    virtual ~Command() { }

    virtual void execute() = 0;
    virtual void unexecute() = 0;
    virtual QString getName() const = 0;
};

class NamedCommand : public Command
{
public:
    NamedCommand(QString name) : m_name(name) { }
    virtual ~NamedCommand() { }

    virtual QString getName() const { return m_name; }
    virtual void setName(QString name) { m_name = name; }

protected:
    QString m_name;
};

class MacroCommand : public Command
{
public:
    MacroCommand(QString name);
    virtual ~MacroCommand();

    virtual void addCommand(Command *command);
    virtual void deleteCommand(Command *command);
    virtual bool haveCommands() const;

    virtual void execute();
    virtual void unexecute();

    virtual QString getName() const;
    virtual void setName(QString name);

protected:
    QString m_name;
    std::vector<Command *> m_commands;
};

/**
 * BundleCommand is a MacroCommand whose name includes a note of how
 * many commands it contains
 */
class BundleCommand : public MacroCommand
{
public:
    BundleCommand(QString name);
    virtual ~BundleCommand();

    virtual QString getName() const;
};

}

#endif