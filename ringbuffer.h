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
#include <QByteArray>

namespace QJack {

/** Lock-free ringbuffer. */
class RingBufferPrivate;
class RingBuffer {
public:
    RingBuffer(int size = 4096);
    RingBuffer(const RingBuffer& other);
    virtual ~RingBuffer();

    /** Locks ring buffer to memory. Not a RT operation. */
    bool memoryLock();

    /** Empties this buffer. @attention Not threadsafe. */
    void reset();
    /** Empties and resizes this buffer. @attention Not threadsafe. */
    void resetSize(int size);

    /** @returns how many bytes are available for reading. */
    int readSpace() const;

    /** @returns how many bytes are available for writing. */
    int writeSpace() const ;

    /** Read @a size bytes from the ringbuffer. */
    QByteArray read(int size);

    /** Write @a data to the ringbuffer. */
    int write(QByteArray data);

private:
    QSharedPointer<RingBufferPrivate> _sharedRingBuffer;
};

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

}
