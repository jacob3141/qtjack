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

QSampleBuffer::QSampleBuffer(const QSampleBuffer& other)
{
    _bufferType = other._bufferType;
    _bufferSize = other._bufferSize;
    _buffer = other._buffer;
}

QSampleBuffer::QSampleBuffer(QSampleBuffer::BufferType bufferType, int bufferSize, void *buffer)
{
    _bufferType = bufferType;
    _bufferSize = bufferSize;
    _buffer = buffer;
}

QSampleBuffer::BufferType QSampleBuffer::bufferType()
{
    return _bufferType;
}

int QSampleBuffer::bufferSize()
{
    return _bufferSize;
}

double QSampleBuffer::readAudioSample(int i)
{
    if(_bufferType == AudioBuffer) {
        return (i >= 0 && i < _bufferSize) ? ((double*)(_buffer))[i] : 0.0;
    } else {
        return 0.0;
    }
}

void QSampleBuffer::writeAudioSample(int i, double value)
{
    if(_bufferType == AudioBuffer) {
        if(i >= 0 && i < _bufferSize) {
            ((double*)_buffer)[i] = value;
        }
    }
}

QString QSampleBuffer::lastError() {
    return _lastError;
}

bool QSampleBuffer::copyTo(QSampleBuffer sampleBuffer)
{
    if(_bufferSize != sampleBuffer._bufferSize) {
        _lastError = QString("Trying to copy from a sample buffer with %1 samples to a sample buffer with %2.").arg(_bufferSize).arg(sampleBuffer._bufferSize);
        return false;
    }
    memcpy(sampleBuffer._buffer, _buffer, _bufferSize);
    return true;
}
