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
#include "global.h"
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

    bool isValid() const REALTIME_SAFE { return _jackBuffer != 0; }

    /** @return the buffer size. */
    int size() const;

protected:
    Buffer();
    Buffer(const Buffer& other);

    Buffer(int size, void* buffer);

    /** Size of sample buffer. */
    int _size;

    /** Pointer to memory buffer. */
    void *_jackBuffer;
};

}
