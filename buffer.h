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

// Qt includes
#include <QString>

namespace QJack {

/**
 * @author Jacob Dawid ( jacob.dawid@omg-it.works )
 * Handle to a memory buffer. Buffer handles are lightweight objects
 * that refer to a memory buffer, store some meta-information and provide
 * convenience methods to read and write the buffer.
 */
class Buffer {
    friend class Port;
public:
    Buffer() {
        _buffer = 0;
    }

    /**
     * Create an audio buffer by allocating memory manually.
     * @warning: Do not call this method in RT code.
     */
    static Buffer createMemoryAudioBuffer(int size);

    /**
     * Frees memory if this is a sample buffer create in memory.
     * @warning: Do not call this method in RT code.
     */
    void releaseMemoryBuffer();

    /** Copy constructor. */
    Buffer(const Buffer& other);

    bool isValid() const { return _buffer != 0; }

    /**
     * @returns true, if this buffer's memory has been allocated manually.
     * (ie. the memory is being managed by the user, not JACK)
     */
    bool isMemoryBuffer() const;

    // Single sample operations

    /** @return the buffer size. */
    int size() const;

    /** @returns sample at position i in the audio buffer. */
    double readAudioSample(int i) const;

    /** Writes sample at position i in the audio buffer. */
    void writeAudioSample(int i, double value);

    // Operations targeting all samples in the buffer

    /** Sets all samples to zero. */
    void clear();

    /**
     * Copies all samples from this buffer to the given buffer.
     * If the source buffer is greater than the target buffer, samples
     * will be truncated. If the target buffer is greater than the
     * source buffer, this operation affects the n samples at the
     * beginning of the target buffer.
     */
    bool copyTo(Buffer targetBuffer) const;

    /**
     * Adds all samples from this buffer to the given buffer.
     * If the source buffer is greater than the target buffer, samples
     * will be truncated. If the target buffer is greater than the
     * source buffer, this operation affects the n samples at the
     * beginning of the target buffer.
     */
    bool addTo(Buffer targetBuffer) const;

    /**
     * Multiplies and adds all samples from this buffer to the given buffer.
     * If the source buffer is greater than the target buffer, samples
     * will be truncated. If the target buffer is greater than the
     * source buffer, this operation affects the n samples at the
     * beginning of the target buffer.
     */
    bool addTo(Buffer targetBuffer, double attenuation) const;

    /**
     * Multiplies all samples in this buffer with @attenuation.
     */
    void multiply(double attenuation);

private:
    /** Private constructor. */
    Buffer(int size, void* buffer);

    /** Size of sample buffer. */
    int _size;

    /** Pointer to memory buffer. */
    void *_buffer;

    /**
     * Flag that indicates that this buffer's memory has been allocated manually.
     * (ie. the memory is being managed by the user, not JACK)
     */
    bool _isMemoryBuffer;
};

}
