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
#include <jack/jack.h>

// Qt includes
#include <QString>
#include <QMetaType>

// Own includes
#include "global.h"

namespace QJack {

/**
 * @author Jacob Dawid ( jacob.dawid@omg-it.works )
 */
class Port {
    friend class Client;
public:
    Port();
    Port(const Port& other);
    virtual ~Port();

    bool isValid() const REALTIME_SAFE { return _jackPort != 0; }

    /** @returns the full name of this port (including the clients name). */
    QString fullName() const REALTIME_SAFE;

    /** @returns the name of the client this port belongs to. */
    QString clientName() const REALTIME_SAFE;

    /** @returns this ports name. */
    QString portName() const REALTIME_SAFE;

    /** @returns the full type of this port. */
    QString portType() const REALTIME_SAFE;

    /** @returns true when this port is an audio port. */
    bool isAudioPort() const REALTIME_SAFE;

    /** @returns true when this port is a midi port. */
    bool isMidiPort() const REALTIME_SAFE;

    /** @returns true, when this port can receive data. */
    bool isInput() const REALTIME_SAFE;

    /** @returns true, when data can be read from this port. */
    bool isOutput() const REALTIME_SAFE;

    /** @returns true, when this port corresponds to a physical I/O connector. */
    bool isPhysical() const REALTIME_SAFE;

    /** @returns whether this port can monitor. */
    bool canMonitor() const REALTIME_SAFE;

    /** @returns whether this port is a terminal. */
    bool isTerminal() const REALTIME_SAFE;

    int numberOfConnections() const REALTIME_SAFE;

    bool isConnectedTo(const Port& other) const REALTIME_SAFE;

protected:
    Port(jack_port_t *jackPort);

    jack_port_t *_jackPort;
};

}

Q_DECLARE_METATYPE(QJack::Port)

namespace QJack {
    class PortMetaTypeInitializer {
    public:
        PortMetaTypeInitializer() {
            qRegisterMetaType<QJack::Port>();
        }
    };

    static PortMetaTypeInitializer portMetaTypeInitializer;
}

