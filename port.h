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
class Port {
    friend class Client;
public:
    Port();
    Port(const Port& other);

    bool isValid() const { return _jackPort != 0; }

    /** @returns the full name of this port (including the clients name). */
    QString fullName() const;

    /** @returns the name of the client this port belongs to. */
    QString clientName() const;

    /** @returns this ports name. */
    QString portName() const;

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
    Buffer buffer(int samples) const;

    /** @returns the full type of this port. */
    QString portType() const;

    /** @returns true when this port is an audio port. */
    bool isAudioPort() const;

    /** @returns true when this port is a midi port. */
    bool isMidiPort() const;

    /** @returns true, when this port can receive data. */
    bool isInput() const;

    /** @returns true, when data can be read from this port. */
    bool isOutput() const;

    /** @returns true, when this port corresponds to a physical I/O connector. */
    bool isPhysical() const;

    /** @returns whether this port can monitor. */
    bool canMonitor() const;

    /** @returns whether this port is a terminal. */
    bool isTerminal() const;

private:
    Port(jack_port_t *port);

    jack_port_t *_jackPort;
};

}
