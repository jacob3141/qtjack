///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    This file is part of QJackAudio.                                       //
//    Copyright (C) 2014 Jacob Dawid, jacob@omg-it.works                     //
//                                                                           //
//    QJackAudio is free software: you can redistribute it and/or modify     //
//    it under the terms of the GNU General Public License as published by   //
//    the Free Software Foundation, either version 3 of the License, or      //
//    (at your option) any later version.                                    //
//                                                                           //
//    QJackAudio is distributed in the hope that it will be useful,          //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of         //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
//    GNU General Public License for more details.                           //
//                                                                           //
//    You should have received a copy of the GNU General Public License      //
//    along with QJackAudio. If not, see <http://www.gnu.org/licenses/>.     //
//                                                                           //
//    It is possible to obtain a closed-source license of QJackAudio.        //
//    If you're interested, contact me at: jacob@omg-it.works                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#pragma once

// Qt includes
#include <QString>

/**
 * @author Jacob Dawid ( jacob.dawid@omg-it.works )
 * Handle to a JACK sample buffer. Buffer handles are lightweight objects
 * that refer to a memory buffer, store some meta-information and provide
 * convenience methods to read and write the buffer.
 *
 * Buffers cannot be created as standalone objects, instead they are bound to
 * another object, for example a QJackPort.
 */
class QJackBuffer
{
    friend class QJackPort;
public:
    /** Create an audio buffer by allocating memory manually. */
    static QJackBuffer createMemoryAudioBuffer(int size);

    /** Frees memory if this is a sample buffer create in memory. */
    void releaseMemoryBuffer();

    /** Copy constructor. */
    QJackBuffer(const QJackBuffer& other);

    /** @returns true, if this buffer's memory has been allocated manually. */
    bool isMemoryBuffer();

    /** @returns a human-readable string containing the last error occurred. */
    QString lastError();

    // Single sample operations

    /** @return the buffer size. */
    int size();

    /** @returns sample at position i in the audio buffer. */
    double readAudioSample(int i);

    /** Writes sample at position i in the audio buffer. */
    void writeAudioSample(int i, double value);

    // Operations targeting all samples in the buffer

    /** Sets all samples to zero. */
    void clear();

    /** Copies all samples from this buffer to the given sampleBuffer. */
    bool copyTo(QJackBuffer sampleBuffer);

    /** Adds all sample from this buffer to the goven sampleBuffer. */
    bool addTo(QJackBuffer sampleBuffer);

    /** Adds all sample from this buffer to the goven sampleBuffer. */
    bool addTo(QJackBuffer sampleBuffer, double attenuation);

    /** Multiplies all samples by the given attenuation value. */
    void multiply(double attenuation);

    /** @returns the absolute value of the highest sample value in this buffer. */
    //double peak();

private:
    /** Private constructor. */
    QJackBuffer(int size, void* buffer);

    /** The last error that occurred during an operation. */
    QString _lastError;

    /** Size of sample buffer. */
    int _size;

    /** Pointer to memory buffer. */
    void *_buffer;

    /** Flag that indicates that this buffer's memory has been allocated manually. */
    bool _isMemoryBuffer;
};
