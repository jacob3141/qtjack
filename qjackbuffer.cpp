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
#include <QJackBuffer>

// JACK includes
#include <jack/jack.h>

QJackBuffer QJackBuffer::createMemoryAudioBuffer(int size)
{
    if(size < 1) {
        size = 1;
    }

    QJackBuffer sampleBuffer(size, (void*)new jack_default_audio_sample_t[size]);
    sampleBuffer._isMemoryBuffer = true;
    return sampleBuffer;
}

QJackBuffer::QJackBuffer(const QJackBuffer& other)
{
    _size = other._size;
    _buffer = other._buffer;
    _isMemoryBuffer = other._isMemoryBuffer;
}

QJackBuffer::QJackBuffer(int bufferSize, void *buffer)
{
    _size = bufferSize;
    _buffer = buffer;
    _isMemoryBuffer = false;
}

bool QJackBuffer::isMemoryBuffer()
{
    return _isMemoryBuffer;
}

int QJackBuffer::size()
{
    return _size;
}

double QJackBuffer::readAudioSample(int i)
{
    return (double)((i >= 0 && i < _size) ? ((jack_default_audio_sample_t*)(_buffer))[i] : 0.0);
}

void QJackBuffer::writeAudioSample(int i, double value)
{
    if(i >= 0 && i < _size) {
        ((jack_default_audio_sample_t*)_buffer)[i] = (jack_default_audio_sample_t)value;
    }
}

QString QJackBuffer::lastError()
{
    return _lastError;
}

void QJackBuffer::clear()
{
    for(int i = 0; i < _size; i++) {
        ((jack_default_audio_sample_t*)_buffer)[i] = 0.0;
    }
}

bool QJackBuffer::copyTo(QJackBuffer sampleBuffer)
{
    if(_size != sampleBuffer._size) {
        _lastError = QString("Trying to copy from a sample buffer with %1 samples to a sample buffer with %2.").arg(_size).arg(sampleBuffer._size);
        return false;
    }

    for(int i = 0; i < _size; i++) {
        ((jack_default_audio_sample_t*)sampleBuffer._buffer)[i] = ((jack_default_audio_sample_t*)_buffer)[i];
    }

    return true;
}

bool QJackBuffer::addTo(QJackBuffer sampleBuffer)
{
    if(_size != sampleBuffer._size) {
        _lastError = QString("Trying to add a sample buffer with %1 samples to a sample buffer with %2.").arg(_size).arg(sampleBuffer._size);
        return false;
    }

    for(int i = 0; i < _size; i++) {
        ((jack_default_audio_sample_t*)sampleBuffer._buffer)[i] += ((jack_default_audio_sample_t*)_buffer)[i];
    }

    return true;
}

bool QJackBuffer::addTo(QJackBuffer sampleBuffer, double attenuation)
{
    if(_size != sampleBuffer._size) {
        _lastError = QString("Trying to add a sample buffer with %1 samples to a sample buffer with %2.").arg(_size).arg(sampleBuffer._size);
        return false;
    }

    for(int i = 0; i < _size; i++) {
        ((jack_default_audio_sample_t*)sampleBuffer._buffer)[i] += (((jack_default_audio_sample_t*)_buffer)[i] * attenuation);
    }

    return true;
}

void QJackBuffer::multiply(double attenuation)
{
    for(int i = 0; i < _size; i++) {
        ((jack_default_audio_sample_t*)_buffer)[i] *= attenuation;
    }
}

//double QJackBuffer::peak()
//{
//    double peak = 0.0;
//    for(int i = 0; i < _size; i++) {
//        double sample = QUnits::peak(((jack_default_audio_sample_t*)_buffer)[i]);
//        peak = sample > peak ? sample : peak;
//    }
//    return peak;
//}

void QJackBuffer::releaseMemoryBuffer()
{
    if(_isMemoryBuffer) {
        delete[] (jack_default_audio_sample_t*)_buffer;
    }
}
