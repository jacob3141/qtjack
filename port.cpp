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
#include "port.h"
#include "client.h"

// Qt includes
#include <QStringList>

namespace QJack {

Port::Port(jack_port_t *jackPort)
{
    _jackPort = jackPort;
}

Port::Port()
{
    _jackPort = 0;
}

Port::Port(const Port& other) {
    _jackPort = other._jackPort;
}

Port::~Port() {

}

QString Port::fullName() const {
    if(!isValid()) {
        return QString();
    }
    return jack_port_name(_jackPort);
}

QString Port::clientName() const {
    if(!isValid()) {
        return QString();
    }
    return fullName().split(":").at(0);
}

QString Port::portName() const {
    if(!isValid()) {
        return QString();
    }
    return jack_port_short_name(_jackPort);
}

QString Port::portType() const {
    if(!isValid()) {
        return QString();
    }
    return QString(jack_port_type(_jackPort));
}

bool Port::isAudioPort() const {
    QString portType(jack_port_type(_jackPort));
    return isValid() && portType.toLower().contains("audio");
}

bool Port::isMidiPort() const {
    QString portType(jack_port_type(_jackPort));
    return isValid() && portType.toLower().contains("midi");
}

bool Port::isInput() const {
    return isValid() && (jack_port_flags(_jackPort) & JackPortIsInput);
}

bool Port::isOutput() const {
    return isValid() && (jack_port_flags(_jackPort) & JackPortIsOutput);
}

bool Port::isPhysical() const {
    return isValid() && (jack_port_flags(_jackPort) & JackPortIsPhysical);
}

bool Port::canMonitor() const {
    return isValid() && (jack_port_flags(_jackPort) & JackPortCanMonitor);
}

bool Port::isTerminal() const {
    return isValid() && (jack_port_flags(_jackPort) & JackPortIsTerminal);
}

int Port::numberOfConnections() const {
    if(!isValid()) {
        return 0;
    }
    return jack_port_connected(_jackPort);
}

bool Port::isConnectedTo(const Port &other) const {
    if(!isValid() || !other.isValid()) {
        return false;
    }

    return jack_port_connected_to(_jackPort, other.fullName().toStdString().c_str());
}

}
