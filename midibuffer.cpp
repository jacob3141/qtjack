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

// Own includes
#include "midibuffer.h"

namespace QtJack {

MidiBuffer::MidiBuffer()
    : Buffer() {
}

MidiBuffer::MidiBuffer(const MidiBuffer& other)
    : Buffer(other) {
}

MidiBuffer::MidiBuffer(int size, void *buffer)
    : Buffer(size, buffer) {
}

MidiBuffer::~MidiBuffer() {
}

bool MidiBuffer::clear() {
    if(!isValid()) {
        return false;
    }
    for(int i = 0; i < _size; i++) {
        ((MidiSample*)_jackBuffer)[i] = 0.0;
    }
    return true;
}

MidiSample MidiBuffer::read(int i, bool *ok) const {
    if(!isValid()) {
        if(ok) {
            (*ok) = false;
        }
        return 0.0;
    }

    if(ok) {
        (*ok) = true;
    }
    return (double)((i >= 0 && i < _size) ? ((MidiSample*)(_jackBuffer))[i] : 0.0);
}

bool MidiBuffer::write(int i, MidiSample value) {
    if(!isValid()) {
        return false;
    }
    if(i >= 0 && i < _size) {
        ((MidiSample*)_jackBuffer)[i] = (MidiSample)value;
    }
    return true;
}

bool MidiBuffer::copyTo(MidiBuffer targetBuffer) const {
    if(!isValid()) {
        return false;
    }

    int size = _size < targetBuffer.size() ? _size : targetBuffer.size();
    for(int i = 0; i < size; i++) {
        ((MidiSample*)targetBuffer._jackBuffer)[i] = ((MidiSample*)_jackBuffer)[i];
    }

    return true;
}

bool MidiBuffer::push(MidiRingBuffer &ringBuffer) {
    if(_size <= ringBuffer.numberOfElementsCanBeWritten()) {
        ringBuffer.write((MidiSample*)_jackBuffer, _size);
        return true;
    }
    return false;
}

bool MidiBuffer::pop(MidiRingBuffer &ringBuffer) {
    if(ringBuffer.numberOfElementsAvailableForRead() >= _size) {
        ringBuffer.read((MidiSample*)_jackBuffer, _size);
        return true;
    }
    return false;
}

unsigned int MidiBuffer::getEventCount() {
    return (int)jack_midi_get_event_count(_jackBuffer);
}

} // namespace QtJack
