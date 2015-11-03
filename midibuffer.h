///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    This file is part of QtJack.                                           //
//    Copyright (C) 2014-2015 Jacob Dawid <jacob@omg-it.works>               //
//                                                                           //
//    QtJack is free software: you can redistribute it and/or modify         //
//    it under the terms of the GNU General Public License as published by   //
//    the Free Software Foundation, either version 3 of the License, or      //
//    (at your option) any later version.                                    //
//                                                                           //
//    QtJack is distributed in the hope that it will be useful,              //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of         //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
//    GNU General Public License for more details.                           //
//                                                                           //
//    You should have received a copy of the GNU General Public License      //
//    along with QtJack. If not, see <http://www.gnu.org/licenses/>.         //
//                                                                           //
//    It is possible to obtain a closed-source license of QtJack.            //
//    If you're interested, contact me at: jacob@omg-it.works                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#pragma once

// Own includes
#include "global.h"
#include "buffer.h"
#include "midievent.h"

namespace QtJack {

class MidiBuffer : public Buffer {
    friend class MidiPort;
public:
    MidiBuffer();
    MidiBuffer(const MidiBuffer& other);
    virtual ~MidiBuffer();

    /** Sets all samples to zero. */
    bool clear() REALTIME_SAFE;

    /** @returns sample at position i in the midi buffer. */
    MidiData read(int i, bool *ok = 0) const REALTIME_SAFE;

    /** @returns the number of MIDI events in the buffer. */
    int numberOfEvents();

    /** @returns the MIDI event at the given index. */
    MidiEvent readEvent(int index, bool *ok = 0);

    /** Writes sample at position i in the midi buffer. */
    bool write(int i, MidiData value) REALTIME_SAFE;

    /** Clears the event buffer of an output port. */
    void clearEventBuffer();

    /** Reset the event buffer (for memory buffers). */
    void resetEventBuffer();

    /** Get the size of the largest event fitting into this buffer. */
    size_t maximumEventSize();

    /**
     * Reserves space for an event to be written to the buffer.
     * @returns a valid pointer to a data area or NULL or error.
     */
    MidiData *reserveEvent(int sample, size_t dataSize);

    /**
     * Writes an event to the event buffer.
     * @param sample The sample offset.
     * @param midiData A pointer to the beginning of a MIDI data buffer.
     * @param dataSize The size of the data buffer.
     * @returns true on success, false otherwise.
     */
    bool writeEvent(int sample, MidiData *midiData, size_t dataSize);

    /**
     * Copies all samples from this buffer to the given buffer.
     * If the source buffer is greater than the target buffer, samples
     * will be truncated. If the target buffer is greater than the
     * source buffer, this operation affects the n samples at the
     * beginning of the target buffer.
     */
    bool copyTo(MidiBuffer targetBuffer) const REALTIME_SAFE;

    /**
     * Pushes the contents of this buffer to the specified ring buffer.
     * @param ringBuffer The ring buffer to write to.
     * @returns true on succes, false otherwise.
     */
    bool push(MidiRingBuffer& ringBuffer) REALTIME_SAFE;

    /**
     * Pops the contents of this buffer from the specified ring buffer.
     * @param ringBuffer The ring buffer to read from.
     * @returns true on succes, false otherwise.
     */
    bool pop(MidiRingBuffer& ringBuffer) REALTIME_SAFE;

    /** Get the number of events that could not be written to the buffer. */
    int lostEventCount();

protected:
    MidiBuffer(int size, void *buffer);
};

} // namespace QtJack
