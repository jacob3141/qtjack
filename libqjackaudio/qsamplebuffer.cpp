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

// Own includes
#include <QSampleBuffer>

// JACK includes
#include <jack/jack.h>

QSampleBuffer QSampleBuffer::createMemoryAudioBuffer(int size)
{
    if(size < 1) {
        size = 1;
    }

    QSampleBuffer sampleBuffer(AudioBuffer, size, (void*)new jack_default_audio_sample_t[size]);
    sampleBuffer._isMemoryBuffer = true;
    return sampleBuffer;
}

QSampleBuffer::QSampleBuffer(const QSampleBuffer& other)
{
    _bufferType = other._bufferType;
    _bufferSize = other._bufferSize;
    _buffer = other._buffer;
    _isMemoryBuffer = other._isMemoryBuffer;
}

QSampleBuffer::QSampleBuffer(QSampleBuffer::BufferType bufferType, int bufferSize, void *buffer)
{
    _bufferType = bufferType;
    _bufferSize = bufferSize;
    _buffer = buffer;
    _isMemoryBuffer = false;
}

bool QSampleBuffer::isMemoryBuffer()
{
    return _isMemoryBuffer;
}

QSampleBuffer::BufferType QSampleBuffer::bufferType()
{
    return _bufferType;
}

int QSampleBuffer::size()
{
    return _bufferSize;
}

double QSampleBuffer::readAudioSample(int i)
{
    if(_bufferType == AudioBuffer) {
        return (double)((i >= 0 && i < _bufferSize) ? ((jack_default_audio_sample_t*)(_buffer))[i] : 0.0);
    } else {
        return 0.0;
    }
}

void QSampleBuffer::writeAudioSample(int i, double value)
{
    if(_bufferType == AudioBuffer) {
        if(i >= 0 && i < _bufferSize) {
            ((jack_default_audio_sample_t*)_buffer)[i] = (jack_default_audio_sample_t)value;
        }
    }
}

QString QSampleBuffer::lastError()
{
    return _lastError;
}


void QSampleBuffer::clear()
{
    for(int i = 0; i < _bufferSize; i++) {
        ((jack_default_audio_sample_t*)_buffer)[i] = 0.0;
    }
}

bool QSampleBuffer::copyTo(QSampleBuffer sampleBuffer)
{
    if(_bufferSize != sampleBuffer._bufferSize) {
        _lastError = QString("Trying to copy from a sample buffer with %1 samples to a sample buffer with %2.").arg(_bufferSize).arg(sampleBuffer._bufferSize);
        return false;
    }

    for(int i = 0; i < _bufferSize; i++) {
        ((jack_default_audio_sample_t*)sampleBuffer._buffer)[i] = ((jack_default_audio_sample_t*)_buffer)[i];
    }

    return true;
}

bool QSampleBuffer::addTo(QSampleBuffer sampleBuffer)
{
    if(_bufferSize != sampleBuffer._bufferSize) {
        _lastError = QString("Trying to add a sample buffer with %1 samples to a sample buffer with %2.").arg(_bufferSize).arg(sampleBuffer._bufferSize);
        return false;
    }

    for(int i = 0; i < _bufferSize; i++) {
        ((jack_default_audio_sample_t*)sampleBuffer._buffer)[i] += ((jack_default_audio_sample_t*)_buffer)[i];
    }

    return true;
}

bool QSampleBuffer::addTo(QSampleBuffer sampleBuffer, double attenuation)
{
    if(_bufferSize != sampleBuffer._bufferSize) {
        _lastError = QString("Trying to add a sample buffer with %1 samples to a sample buffer with %2.").arg(_bufferSize).arg(sampleBuffer._bufferSize);
        return false;
    }

    for(int i = 0; i < _bufferSize; i++) {
        ((jack_default_audio_sample_t*)sampleBuffer._buffer)[i] += (((jack_default_audio_sample_t*)_buffer)[i] * attenuation);
    }

    return true;
}

void QSampleBuffer::releaseMemoryBuffer()
{
    if(_isMemoryBuffer) {
        if(_bufferType == AudioBuffer) {
            delete[] (jack_default_audio_sample_t*)_buffer;
        }
    }
}
