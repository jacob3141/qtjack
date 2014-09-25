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

#ifndef QSAMPLEBUFFER_H
#define QSAMPLEBUFFER_H

// Qt includes
#include <QString>

/**
 * @class QSampleBuffer
 * @author Jacob Dawid ( jacob.dawid@omg-it.works )
 * Handle to a JACK sample buffer. Buffer handles are lightweight objects
 * that refer to a memory buffer, store some meta-information and provide
 * convenience methods to read and write the buffer.
 *
 * Buffers cannot be created as standalone objects, instead they are bound to
 * another object, for example a QJackPort.
 */
class QSampleBuffer
{
    friend class QJackPort;
public:
    /** Create an audio buffer by allocating memory manually. */
    static QSampleBuffer createMemoryAudioBuffer(int size);

    /** Frees memory if this is a sample buffer create in memory. */
    void releaseMemoryBuffer();

    /** Copy constructor. */
    QSampleBuffer(const QSampleBuffer& other);

    /**
     * Type of sample buffer.
     */
    enum BufferType {
        AudioBuffer,
        MidiBuffer
    };

    /** @returns true, if this buffer's memory has been allocated manually. */
    bool isMemoryBuffer();

    /** @returns the buffer type. @see BufferType */
    BufferType bufferType();

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
    bool copyTo(QSampleBuffer sampleBuffer);

    /** Adds all sample from this buffer to the goven sampleBuffer. */
    bool addTo(QSampleBuffer sampleBuffer);

    /** Adds all sample from this buffer to the goven sampleBuffer. */
    bool addTo(QSampleBuffer sampleBuffer, double attenuation);

    /** Multiplies all samples by the given attenuation value. */
    void multiply(double attenuation);

    /** @returns the absolute value of the highest sample value in this buffer. */
    double peak();

private:
    /** Private constructor. */
    QSampleBuffer(BufferType bufferType, int size, void* buffer);

    /** The last error that occurred during an operation. */
    QString _lastError;

    /** Sample buffer type. */
    BufferType _bufferType;

    /** Size of sample buffer. */
    int _size;

    /** Pointer to memory buffer. */
    void *_buffer;

    /** Flag that indicates that this buffer's memory has been allocated manually. */
    bool _isMemoryBuffer;
};

#endif // QSAMPLEBUFFER_H
