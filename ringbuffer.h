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

// JACK includes
#include <jack/ringbuffer.h>

// Qt includes
#include <QSharedPointer>
#include <QVector>

// Own includes
#include "global.h"

namespace QJack {

/** Lock-free ringbuffer. */
class RingBufferPrivate {
public:
    RingBufferPrivate(int size) {
        _jackRingBuffer = jack_ringbuffer_create(size);
    }

    virtual ~RingBufferPrivate() {
        jack_ringbuffer_free(_jackRingBuffer);
    }

    jack_ringbuffer_t *_jackRingBuffer;
};

template<typename Type>
class RingBuffer {
public:
    RingBuffer(int numberOfElements = 4096) {
        _p = QSharedPointer<RingBufferPrivate>(new RingBufferPrivate(numberOfElements * bytesPerElement()));
    }

    RingBuffer(const RingBuffer& other) {
        _p = other._p;
    }

    bool isValid() REALTIME_SAFE {
        return _p->_jackRingBuffer != 0;
    }

    virtual ~RingBuffer() {
    }

    /** Locks ring buffer to memory. Not a RT operation. */
    bool memoryLock()  {
        return (bool)jack_ringbuffer_mlock(_p->_jackRingBuffer);
    }

    /** Empties this buffer. @attention Not threadsafe. */
    void reset() {
        jack_ringbuffer_reset(_p->_jackRingBuffer);
    }

    /** Empties and resizes this buffer. @attention Not threadsafe. */
    void resetSize(int size)  {
        jack_ringbuffer_reset_size(_p->_jackRingBuffer, size * bytesPerElement());
    }

    /** @returns how many elements are available for reading. */
    int numberOfElementsAvailableForRead() const REALTIME_SAFE {
        return jack_ringbuffer_read_space(_p->_jackRingBuffer) / bytesPerElement();
    }

    /** @returns how many elements are available for writing. */
    int numberOfElementsCanBeWritten() const REALTIME_SAFE {
        return jack_ringbuffer_write_space(_p->_jackRingBuffer) / bytesPerElement();
    }

    /** Read @a numberOfElements of elements from the ringbuffer. */
    int read(Type *data, int numberOfElements) REALTIME_SAFE {
        int bytesRead = jack_ringbuffer_read(_p->_jackRingBuffer,
                                             (char*)data,
                                             numberOfElements * bytesPerElement());
        return bytesRead / bytesPerElement();
    }

    /** Write @a data to the ringbuffer. */
    int write(Type *data, int numberOfElements) REALTIME_SAFE {
        int bytesWritten = jack_ringbuffer_write(_p->_jackRingBuffer,
                                                 (char*)data,
                                                 numberOfElements * bytesPerElement());
        return bytesWritten / bytesPerElement();
    }

    int bytesPerElement() const REALTIME_SAFE {
        return sizeof(Type);
    }

private:
    QSharedPointer<RingBufferPrivate> _p;
};

typedef RingBuffer<jack_default_audio_sample_t> AudioRingBuffer;

}
