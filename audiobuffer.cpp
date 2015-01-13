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
#include "audiobuffer.h"

#include <QDebug>

namespace QJack {

AudioBuffer::AudioBuffer()
    : Buffer() {
}

AudioBuffer::AudioBuffer(const AudioBuffer& other)
    : Buffer(other) {
}

AudioBuffer::AudioBuffer(int size, void *buffer)
    : Buffer(size, buffer) {
}

AudioBuffer::~AudioBuffer() {
}

bool AudioBuffer::clear() {
    if(!isValid()) {
        return false;
    }
    for(int i = 0; i < _size; i++) {
        ((AudioSample*)_jackBuffer)[i] = 0.0;
    }
    return true;
}

AudioSample AudioBuffer::read(int i, bool *ok) const {
    if(!isValid()) {
        if(ok) {
            (*ok) = false;
        }
        return 0.0;
    }

    if(ok) {
        (*ok) = true;
    }
    return (double)((i >= 0 && i < _size) ? ((AudioSample*)(_jackBuffer))[i] : 0.0);
}

bool AudioBuffer::write(int i, AudioSample value) {
    if(!isValid()) {
        return false;
    }
    if(i >= 0 && i < _size) {
        ((AudioSample*)_jackBuffer)[i] = (AudioSample)value;
    }
    return true;
}

bool AudioBuffer::copyTo(AudioBuffer targetBuffer) const {
    if(!isValid()) {
        return false;
    }

    int size = _size < targetBuffer.size() ? _size : targetBuffer.size();
    for(int i = 0; i < size; i++) {
        ((AudioSample*)targetBuffer._jackBuffer)[i] = ((AudioSample*)_jackBuffer)[i];
    }

    return true;
}

bool AudioBuffer::addTo(AudioBuffer targetBuffer) const {
    if(!isValid()) {
        return false;
    }

    int size = _size < targetBuffer.size() ? _size : targetBuffer.size();
    for(int i = 0; i < size; i++) {
        ((AudioSample*)targetBuffer._jackBuffer)[i] += ((AudioSample*)_jackBuffer)[i];
    }

    return true;
}

bool AudioBuffer::addTo(AudioBuffer targetBuffer, double attenuation) const {
    if(!isValid()) {
        return false;
    }

    int size = _size < targetBuffer.size() ? _size : targetBuffer.size();
    for(int i = 0; i < size; i++) {
        ((AudioSample*)targetBuffer._jackBuffer)[i] += (((AudioSample*)_jackBuffer)[i] * attenuation);
    }

    return true;
}

void AudioBuffer::multiply(double attenuation) {
    if(!isValid()) {
        return;
    }

    for(int i = 0; i < _size; i++) {
        ((AudioSample*)_jackBuffer)[i] *= attenuation;
    }
}

bool AudioBuffer::push(AudioRingBuffer &ringBuffer) {
    if(_size <= ringBuffer.numberOfElementsCanBeWritten()) {
        ringBuffer.write((AudioSample*)_jackBuffer, _size);
        return true;
    }
    return false;
}

bool AudioBuffer::pop(AudioRingBuffer &ringBuffer) {
    if(ringBuffer.numberOfElementsAvailableForRead() >= _size) {
        ringBuffer.read((AudioSample*)_jackBuffer, _size);
        return true;
    }
    return false;
}

}
