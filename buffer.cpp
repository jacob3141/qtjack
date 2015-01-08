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

// Own includes
#include "buffer.h"

// JACK includes
#include <jack/jack.h>

namespace QJack {

Buffer Buffer::createMemoryAudioBuffer(int size) {
    // A buffer with less than a single sample does not make any sense.
    if(size < 1) {
        size = 1;
    }

    Buffer sampleBuffer(size, (void*)new jack_default_audio_sample_t[size]);
    sampleBuffer._isMemoryBuffer = true;
    return sampleBuffer;
}

Buffer::Buffer(const Buffer& other) {
    _size = other._size;
    _buffer = other._buffer;
    _isMemoryBuffer = other._isMemoryBuffer;
}

Buffer::Buffer(int bufferSize, void *buffer) {
    _size = bufferSize;
    _buffer = buffer;
    _isMemoryBuffer = false;
}

bool Buffer::isMemoryBuffer() const {
    return _isMemoryBuffer;
}

int Buffer::size() const {
    return _size;
}

double Buffer::readAudioSample(int i) const {
    if(!isValid()) {
        return 0.0;
    }
    return (double)((i >= 0 && i < _size) ? ((jack_default_audio_sample_t*)(_buffer))[i] : 0.0);
}

void Buffer::writeAudioSample(int i, double value) {
    if(!isValid()) {
        return;
    }
    if(i >= 0 && i < _size) {
        ((jack_default_audio_sample_t*)_buffer)[i] = (jack_default_audio_sample_t)value;
    }
}

void Buffer::clear() {
    if(!isValid()) {
        return;
    }
    for(int i = 0; i < _size; i++) {
        ((jack_default_audio_sample_t*)_buffer)[i] = 0.0;
    }
}

bool Buffer::copyTo(Buffer targetBuffer) const {
    if(!isValid()) {
        return false;
    }

    int size = _size < targetBuffer.size() ? _size : targetBuffer.size();
    for(int i = 0; i < size; i++) {
        ((jack_default_audio_sample_t*)targetBuffer._buffer)[i] = ((jack_default_audio_sample_t*)_buffer)[i];
    }

    return true;
}

bool Buffer::addTo(Buffer targetBuffer) const {
    if(!isValid()) {
        return false;
    }

    int size = _size < targetBuffer.size() ? _size : targetBuffer.size();
    for(int i = 0; i < size; i++) {
        ((jack_default_audio_sample_t*)targetBuffer._buffer)[i] += ((jack_default_audio_sample_t*)_buffer)[i];
    }

    return true;
}

bool Buffer::addTo(Buffer targetBuffer, double attenuation) const {
    if(!isValid()) {
        return false;
    }

    int size = _size < targetBuffer.size() ? _size : targetBuffer.size();
    for(int i = 0; i < size; i++) {
        ((jack_default_audio_sample_t*)targetBuffer._buffer)[i] += (((jack_default_audio_sample_t*)_buffer)[i] * attenuation);
    }

    return true;
}

void Buffer::multiply(double attenuation) {
    if(!isValid()) {
        return;
    }

    for(int i = 0; i < _size; i++) {
        ((jack_default_audio_sample_t*)_buffer)[i] *= attenuation;
    }
}

void Buffer::releaseMemoryBuffer() {
    if(!isValid()) {
        return;
    }

    if(_isMemoryBuffer) {
        delete[] (jack_default_audio_sample_t*)_buffer;
        _buffer = 0;
    }
}

}
