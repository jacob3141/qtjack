///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    This file is part of QtJack.                                           //
//    Copyright (C) 2014-2015 Jacob Dawid <jacob@omg-it.works>               //
//                                                                           //
//    QtJack is free software: you can redistribute it and/or modify         //
//    it under the terms of the GNU General Public License as published by   //
//    the Free Software Foundation, either version 3 of the License, or      //
//    (at your option) any later version.                                    //
//                                                                           //
//    QtJack is distributed in the hope that it will be useful,              //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of         //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
//    GNU General Public License for more details.                           //
//                                                                           //
//    You should have received a copy of the GNU General Public License      //
//    along with QtJack. If not, see <http://www.gnu.org/licenses/>.         //
//                                                                           //
//    It is possible to obtain a closed-source license of QtJack.            //
//    If you're interested, contact me at: jacob@omg-it.works                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef QTJACK_JACK2_SUPPORT

// Qt includes
#include <QObject>
#include <QString>

// JACK includes
#include <jack/net.h>

// Own includes
#include "global.h"
#include "processor.h"

namespace QtJack {

/**
 * The net slave class can be used to establish JACK connections
 * via network. This is the slave part that processes audio buffers
 * as requested by the master.
 *
 * First create a master component and make it listen. Then connect
 * with the slave component.
 */
class NetSlave : public QObject {
    Q_OBJECT
public:
    NetSlave(QObject *parent = 0);
    virtual ~NetSlave();

    /**
     * Establishes a connection to a JACK net master.
     * @param ip The IP address of the JACK net master.
     * @param port The port at which to connect.
     * @param name The slave's name reported to the master.
     * @returns true, if a connection could be established.
     */
    bool open(QString ip, int port, QString name);

    /**
     * Close the connection to the JACK net master.
     * @returns 0 on success, an error code otherwise.
     */
    int close();

    /**
     * Activates this slave.
     * @returns 0 on success, an error code otherwise.
     */
    int activate();

    /**
     * Deactivates this slave.
     * @returns 0 on success, an error code otherwise.
     */
    int deactivate();

    /** Assigns a processor that will handle audio processing.
      * @param processor The processor that will handle audio processing.
      */
    void setProcessor(Processor *processor);

private:
    NetSlave(jack_net_slave_t *jackNetSlave);

    jack_net_slave_t *_jackNetSlave;
    jack_slave_t _jackSlave;
    jack_master_t _jackMaster;

    /** Pointer to the current processor object. */
    Processor *_processor;
};

} // namespace QtJack

#endif // QTJACK_JACK2_SUPPORT
