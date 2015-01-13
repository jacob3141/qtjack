///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    This file is part of QJack.                                            //
//    Copyright (C) 2014-2015 Jacob Dawid <jacob@omg-it.works>               //
//                                                                           //
//    QJack is free software: you can redistribute it and/or modify          //
//    it under the terms of the GNU General Public License as published by   //
//    the Free Software Foundation, either version 3 of the License, or      //
//    (at your option) any later version.                                    //
//                                                                           //
//    QJack is distributed in the hope that it will be useful,               //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of         //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
//    GNU General Public License for more details.                           //
//                                                                           //
//    You should have received a copy of the GNU General Public License      //
//    along with QJack. If not, see <http://www.gnu.org/licenses/>.          //
//                                                                           //
//    It is possible to obtain a closed-source license of QJack.             //
//    If you're interested, contact me at: jacob@omg-it.works                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#pragma once

// Own includes
#include "global.h"
#include "buffer.h"

namespace QJack {

class AudioBuffer : public Buffer {
    friend class AudioPort;
public:
    AudioBuffer();
    AudioBuffer(const AudioBuffer& other);
    virtual ~AudioBuffer();

    /** Sets all samples to zero. */
    bool clear() REALTIME_SAFE;

    /** @returns sample at position i in the audio buffer. */
    AudioSample read(int i, bool *ok = 0) const REALTIME_SAFE;

    /** Writes sample at position i in the audio buffer. */
    bool write(int i, AudioSample value) REALTIME_SAFE;

    /**
     * Copies all samples from this buffer to the given buffer.
     * If the source buffer is greater than the target buffer, samples
     * will be truncated. If the target buffer is greater than the
     * source buffer, this operation affects the n samples at the
     * beginning of the target buffer.
     */
    bool copyTo(AudioBuffer targetBuffer) const REALTIME_SAFE;


    /**
     * Adds all samples from this buffer to the given buffer.
     * If the source buffer is greater than the target buffer, samples
     * will be truncated. If the target buffer is greater than the
     * source buffer, this operation affects the n samples at the
     * beginning of the target buffer.
     */
    bool addTo(AudioBuffer targetBuffer) const REALTIME_SAFE;

    /**
     * Multiplies and adds all samples from this buffer to the given buffer.
     * If the source buffer is greater than the target buffer, samples
     * will be truncated. If the target buffer is greater than the
     * source buffer, this operation affects the n samples at the
     * beginning of the target buffer.
     */
    bool addTo(AudioBuffer targetBuffer, double attenuation) const REALTIME_SAFE;

    /**
     * Multiplies all samples in this buffer with @attenuation.
     */
    void multiply(double attenuation) REALTIME_SAFE;

    /**
     * Pushes the contents of this buffer to the specified ring buffer.
     * @param ringBuffer The ring buffer to write to.
     * @returns true on succes, false otherwise.
     */
    bool push(AudioRingBuffer& ringBuffer) REALTIME_SAFE;

    /**
     * Pops the contents of this buffer from the specified ring buffer.
     * @param ringBuffer The ring buffer to read from.
     * @returns true on succes, false otherwise.
     */
    bool pop(AudioRingBuffer& ringBuffer) REALTIME_SAFE;

protected:
    void allocateMemory();
    void releaseMemory();

private:
    AudioBuffer(int size, void *buffer);
};

}
