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
#include "ringbuffer.h"

namespace QJack {

RingBuffer::RingBuffer(int size) {
    _p = QSharedPointer<RingBufferPrivate>(new RingBufferPrivate(size));
}

RingBuffer::RingBuffer(const RingBuffer& other) {
    _p = other._p;
}

RingBuffer::~RingBuffer() {
}

bool RingBuffer::memoryLock() {
    return (bool)jack_ringbuffer_mlock(_p->_jackRingBuffer);
}

void RingBuffer::reset() {
    jack_ringbuffer_reset(_p->_jackRingBuffer);
}

void RingBuffer::resetSize(int size) {
    jack_ringbuffer_reset_size(_p->_jackRingBuffer, size);
}

QByteArray RingBuffer::read(int size) {
    char buf[size];
    int bytesRead = jack_ringbuffer_read(_p->_jackRingBuffer, buf, size);
    return QByteArray(buf, bytesRead);
}

int RingBuffer::write(QByteArray data) {
    return jack_ringbuffer_write(_p->_jackRingBuffer, data.constData(), data.count());
}

int RingBuffer::readSpace() const {
    return jack_ringbuffer_read_space(_p->_jackRingBuffer);
}

int RingBuffer::writeSpace() const {
    return jack_ringbuffer_write_space(_p->_jackRingBuffer);
}

}
