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
#include "buffer.h"

// JACK includes
#include <jack/jack.h>

// Qt includes
#include <QString>

namespace QJack {

/**
 * @author Jacob Dawid ( jacob.dawid@omg-it.works )
 */
class Port
{
    friend class Client;
public:
    Port(const Port& other);

    bool isValid() { return _jackPort != 0; }
    QString fullName();
    QString clientName();
    QString portName();
    Buffer buffer(int samples);

    /** @returns true when this port is an audio port. */
    bool isAudioPort();

    /** @returns true when this port is a midi port. */
    bool isMidiPort();

    /** @returns true, when this port can receive data. */
    bool isInput();

    /** @returns true, when data can be read from this port. */
    bool isOutput();

    /** @returns true, when this port corresponds to a physical I/O connector. */
    bool isPhysical();

    bool canMonitor();
    bool isTerminal();

private:
    Port(jack_port_t *port);
    Port();

    jack_port_t *_jackPort;
};

}
