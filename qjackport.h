///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    This file is part of QJackAudio.                                       //
//    Copyright (C) 2014 Jacob Dawid, jacob@omg-it.works                     //
//                                                                           //
//    QJackAudio is free software: you can redistribute it and/or modify     //
//    it under the terms of the GNU General Public License as published by   //
//    the Free Software Foundation, either version 3 of the License, or      //
//    (at your option) any later version.                                    //
//                                                                           //
//    QJackAudio is distributed in the hope that it will be useful,          //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of         //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
//    GNU General Public License for more details.                           //
//                                                                           //
//    You should have received a copy of the GNU General Public License      //
//    along with QJackAudio. If not, see <http://www.gnu.org/licenses/>.     //
//                                                                           //
//    It is possible to obtain a closed-source license of QJackAudio.        //
//    If you're interested, contact me at: jacob@omg-it.works                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#ifndef QJACKPORT_H
#define QJACKPORT_H

// Own includes
#include <QSampleBuffer>

// JACK includes
#include <jack/jack.h>

// Qt includes
#include <QString>

/**
 * @class QJackPort
 * @author Jacob Dawid ( jacob.dawid@omg-it.works )
 */
class QJackPort
{
    friend class QJackClient;
public:
    bool isValid() { return _port != 0; }
    QString fullName();
    QString clientName();
    QString portName();
    QSampleBuffer sampleBuffer();

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
    QJackPort(jack_port_t *port);
    QJackPort();

    jack_port_t *_port;
};

#endif // QJACKPORT_H
