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

// Own includes
#include "ringbuffer.h"

namespace QJack {

/**
 * @author Jacob Dawid ( jacob.dawid@omg-it.works )
 * Handle to a memory buffer. Buffer handles are lightweight objects
 * that refer to a memory buffer, store some meta-information and provide
 * convenience methods to read and write the buffer.
 */
class Buffer {
    friend class AudioBuffer;
    friend class MidiBuffer;
public:
    virtual ~Buffer();

    enum BufferType {
        BufferTypeJack,
        BufferTypeMemory
    };

    /**
     * Create an audio buffer by allocating memory manually.
     * @warning: Do not call this method in RT code.
     */
    static Buffer createMemoryAudioBuffer(int size);

    /**
     * Frees memory if this is a sample buffer create in memory.
     * @warning: Do not call this method in RT code.
     */
    void releaseMemoryBuffer();

    bool isValid() const { return _buffer != 0; }

    /** @return the buffer size. */
    int size() const;

protected:
    Buffer(BufferType bufferType = BufferTypeJack);
    Buffer(const Buffer& other);

    Buffer(int size, void* buffer, BufferType bufferType = BufferTypeJack);

    /** Size of sample buffer. */
    int _size;

    /** Pointer to memory buffer. */
    void *_buffer;

    BufferType _bufferType;
};

}
