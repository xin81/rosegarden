/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*- vi:set ts=8 sts=4 sw=4: */

/*
    Rosegarden
    A sequencer and musical notation editor.
    Copyright 2000-2009 the Rosegarden development team.
    See the AUTHORS file for more details.

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.  See the file
    COPYING included with this distribution for more information.
*/

#ifndef _INSTRUMENT_H_
#define _INSTRUMENT_H_

#include <string>
#include <vector>

#include "XmlExportable.h"
#include "MidiProgram.h"

#include <QString>
#include <QCoreApplication>

// An Instrument connects a Track (which itself contains
// a list of Segments) to a device that can play that
// Track.  
//
// An Instrument is either MIDI or Audio (or whatever else
// we decide to implement).
//
//

namespace Rosegarden
{

// plugins
class AudioPluginInstance;
typedef std::vector<AudioPluginInstance*>::iterator PluginInstanceIterator;

typedef std::vector<std::pair<MidiByte, MidiByte> > StaticControllers;
typedef std::vector<std::pair<MidiByte, MidiByte> >::iterator StaticControllerIterator;
typedef std::vector<std::pair<MidiByte, MidiByte> >::const_iterator StaticControllerConstIterator;


// Instrument number groups
//
const InstrumentId SystemInstrumentBase     = 0;
const InstrumentId AudioInstrumentBase      = 1000;
const InstrumentId MidiInstrumentBase       = 2000;
const InstrumentId SoftSynthInstrumentBase  = 10000;

const unsigned int AudioInstrumentCount     = 16;
const unsigned int SoftSynthInstrumentCount = 24;

const MidiByte MidiMaxValue = 127;
const MidiByte MidiMidValue = 64;
const MidiByte MidiMinValue = 0;

typedef unsigned int BussId;

// Predeclare Device
//
class Device;

class PluginContainer
{
public:
    static const unsigned int PLUGIN_COUNT; // for non-synth plugins

    PluginInstanceIterator beginPlugins() { return m_audioPlugins.begin(); }
    PluginInstanceIterator endPlugins() { return m_audioPlugins.end(); }

    // Plugin management
    //
    void addPlugin(AudioPluginInstance *instance);
    bool removePlugin(unsigned int position);
    void clearPlugins();
    void emptyPlugins(); // empty the plugins but don't clear them down

    // Get a plugin for this container
    //
    AudioPluginInstance* getPlugin(unsigned int position);

    virtual unsigned int getId() const = 0;
    virtual std::string getName() const = 0;
    virtual std::string getPresentationName() const = 0;

protected:
    PluginContainer(bool havePlugins);
    virtual ~PluginContainer();

    std::vector<AudioPluginInstance*> m_audioPlugins;
};

class Instrument : public XmlExportable, public PluginContainer
{
    Q_DECLARE_TR_FUNCTIONS(Rosegarden::Instrument)
public:
    static const unsigned int SYNTH_PLUGIN_POSITION;

    enum InstrumentType { Midi, Audio, SoftSynth };

    Instrument(InstrumentId id,
               InstrumentType it,
               const std::string &name,
               Device *device);

    Instrument(InstrumentId id,
               InstrumentType it,
               const std::string &name,
               MidiByte channel,
               Device *device);


    // Copy constructor and assignment
    //
    Instrument(const Instrument &);
    Instrument &operator=(const Instrument &);

    ~Instrument();

    virtual std::string getName() const { return m_name; }
    virtual std::string getPresentationName() const;

    /** Returns a translated QString suitable for presentation to the user */
    virtual QString getLocalizedPresentationName() const;

    virtual std::string getAlias() const;

    void setId(InstrumentId id) { m_id = id; }
    InstrumentId getId() const { return m_id; }

    void setName(const std::string &name) { m_name = name; }
    void setAlias(const std::string &alias) { m_alias = alias; }
    InstrumentType getType() const { return m_type; }

    void setType(InstrumentType type) { m_type = type; }
    InstrumentType getInstrumentType() { return m_type; }


    // ---------------- MIDI Controllers -----------------
    //
    void setMidiChannel(MidiByte mC) { m_channel = mC; }
    MidiByte getMidiChannel() const { return m_channel; }

    //void setMidiInputChannel(char ic) { m_input_channel = ic; }
    //char getMidiInputChannel() const { return m_input_channel; }

    void setMidiTranspose(MidiByte mT) { m_transpose = mT; }
    MidiByte getMidiTranspose() const { return m_transpose; }

    // Pan is 0-127 for MIDI instruments, and (for some
    // unfathomable reason) 0-200 for audio instruments.
    //
    void setPan(MidiByte pan) { m_pan = pan; }
    MidiByte getPan() const { return m_pan; }

    // Volume is 0-127 for MIDI instruments.  It's not used for
    // audio instruments -- see "level" instead.
    // 
    void setVolume(MidiByte volume) { m_volume = volume; }
    MidiByte getVolume() const { return m_volume; }

    void setProgram(const MidiProgram &program) { m_program = program; }
    const MidiProgram &getProgram() const { return m_program; }

    void setSendBankSelect(bool value) { m_sendBankSelect = value; }
    bool sendsBankSelect() const { return m_sendBankSelect; }

    void setSendProgramChange(bool value) { m_sendProgramChange = value; }
    bool sendsProgramChange() const { return m_sendProgramChange; }

    void setSendPan(bool value) { m_sendPan = value; }
    bool sendsPan() const { return m_sendPan; }

    void setSendVolume(bool value) { m_sendVolume = value; }
    bool sendsVolume() const { return m_sendVolume; } 

    void setControllerValue(MidiByte controller, MidiByte value);
    MidiByte getControllerValue(MidiByte controller) const;

    // This is retrieved from the reference MidiProgram in the Device
    const MidiKeyMapping *getKeyMapping() const;

    // Convenience functions (strictly redundant with get/setProgram):
    // 
    void setProgramChange(MidiByte program);
    MidiByte getProgramChange() const;

    void setMSB(MidiByte msb);
    MidiByte getMSB() const;

    void setLSB(MidiByte msb);
    MidiByte getLSB() const;

    void setPercussion(bool percussion);
    bool isPercussion() const;

    // --------------- Audio Controllers -----------------
    //
    void setLevel(float dB) { m_level = dB; }
    float getLevel() const { return m_level; }

    void setRecordLevel(float dB) { m_recordLevel = dB; }
    float getRecordLevel() const { return m_recordLevel; }

    void setAudioChannels(unsigned int ch) { m_channel = MidiByte(ch); }
    unsigned int getAudioChannels() const { return (unsigned int)(m_channel); }

    // An audio input can be a buss or a record input. The channel number
    // is required for mono instruments, ignored for stereo ones.
    void setAudioInputToBuss(BussId buss, int channel = 0);
    void setAudioInputToRecord(int recordIn, int channel = 0);
    int getAudioInput(bool &isBuss, int &channel) const;

    void setAudioOutput(BussId buss) { m_audioOutput = buss; }
    BussId getAudioOutput() const { return m_audioOutput; }

    // Implementation of virtual function
    //
    virtual std::string toXmlString();

    // Get and set the parent device
    //
    Device* getDevice() const { return m_device; }
    void setDevice(Device* dev) { m_device = dev; }

    // Return a string describing the current program for
    // this Instrument
    //
    std::string getProgramName() const;

    // MappedId management - should typedef this type once
    // we have the energy to shake this all out.
    //
    int getMappedId() const { return m_mappedId; }
    void setMappedId(int id) { m_mappedId = id; }

    StaticControllers& getStaticControllers() { return m_staticControllers; }

private:
    InstrumentId    m_id;
    std::string     m_name;
    std::string     m_alias;
    InstrumentType  m_type;
    
    // Standard MIDI controllers and parameters
    //
    MidiByte        m_channel;
    //char            m_input_channel;
    MidiProgram     m_program;
    MidiByte        m_transpose;
    MidiByte        m_pan;  // required by audio
    MidiByte        m_volume;

    // Used for Audio volume (dB value)
    //
    float           m_level;

    // Record level for Audio recording (dB value)
    //
    float           m_recordLevel;

    Device         *m_device;

    // Do we send at this intrument or do we leave these
    // things up to the parent device and God?  These are
    // directly relatable to GUI elements
    // 
    bool             m_sendBankSelect;
    bool             m_sendProgramChange;
    bool             m_sendPan;
    bool             m_sendVolume;

    // Instruments are directly related to faders for volume
    // control.  Here we can store the remote fader id.
    //
    int              m_mappedId;

    // Which input terminal we're connected to.  This is a BussId if
    // less than 1000 or a record input number (plus 1000) if >= 1000.
    // The channel number is only used for mono instruments.
    //
    int              m_audioInput;
    int              m_audioInputChannel;

    // Which buss we output to.  Zero is always the master.
    //
    BussId           m_audioOutput;

    // A static controller map that can be saved/loaded and queried along with this instrument.
    // These values are modified from the IPB - if they appear on the IPB then they are sent
    // at playback start time to the sequencer.
    //
    //
    StaticControllers    m_staticControllers;
};


class Buss : public XmlExportable, public PluginContainer
{
public:
    Buss(BussId id);
    ~Buss();

    void setId(BussId id) { m_id = id; }
    BussId getId() const { return m_id; }

    void setLevel(float dB) { m_level = dB; }
    float getLevel() const { return m_level; }
    
    void setPan(MidiByte pan) { m_pan = pan; }
    MidiByte getPan() const { return m_pan; }

    int getMappedId() const { return m_mappedId; }
    void setMappedId(int id) { m_mappedId = id; }

    virtual std::string toXmlString();
    virtual std::string getName() const;
    virtual std::string getPresentationName() const;

private:
    BussId m_id;
    float m_level;
    MidiByte m_pan;
    int m_mappedId;
};
  

// audio record input of a sort that can be connected to

class RecordIn : public XmlExportable
{
public:
    RecordIn();
    ~RecordIn();

    int getMappedId() const { return m_mappedId; }
    void setMappedId(int id) { m_mappedId = id; }

    virtual std::string toXmlString();

private:
    int m_mappedId;
};
    

}

#endif // _INSTRUMENT_H_
