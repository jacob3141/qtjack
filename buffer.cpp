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

Buffer::Buffer() {
    _size = 1024;
    _jackBuffer = 0;
}

Buffer::Buffer(const Buffer& other) {
    _size = other._size;
    _jackBuffer = other._jackBuffer;
}

Buffer::Buffer(int bufferSize, void *buffer) {
    _size = bufferSize;
    _jackBuffer = buffer;
}

Buffer::~Buffer() {
}

int Buffer::size() const {
    return _size;
}

}
