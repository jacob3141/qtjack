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
#include "port.h"
#include "audiobuffer.h"

namespace QJack {

class AudioPort : public Port {
    friend class Client;
public:
    AudioPort();
    AudioPort(const Port& other);
    AudioPort(const AudioPort& other);

    /**
     * @returns a buffer that points to the memory of this port.
     * @warning: Please be aware that if this port is an input port, modifying
     * samples within the buffer can lead to unexpected behaviour for other clients.
     * You have direct access to the input memory buffer and the recommended way
     * is to create a memory buffer, copy all samples over and alter them in the
     * memory buffer. For reasons of RT execution safety, you should create your
     * memory buffers outside of the JACK process thread (for example in the
     * constructor of your processor object).
     */
    AudioBuffer buffer(int samples) const REALTIME_SAFE;


protected:
    AudioPort(jack_port_t *jackPort);
};

}

Q_DECLARE_METATYPE(QJack::AudioPort)

namespace QJack {
    class AudioPortMetaTypeInitializer {
    public:
        AudioPortMetaTypeInitializer() {
            qRegisterMetaType<QJack::AudioPort>();
        }
    };

    static AudioPortMetaTypeInitializer audioPortMetaTypeInitializer;
}
