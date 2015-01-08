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

Port::Port(jack_port_t *port)
{
    _jackPort = port;
}

Port::Port()
{
    _jackPort = 0;
}

Port::Port(const Port& other) {
    _jackPort = other._jackPort;
}

QString Port::fullName()
{
    return jack_port_name(_jackPort);
}

QString Port::clientName()
{
    return fullName().split(":").at(0);
}

QString Port::portName()
{
    return jack_port_short_name(_jackPort);
}

Buffer Port::sampleBuffer(int samples)
{
    return Buffer(samples, jack_port_get_buffer(_jackPort, samples));
}

bool Port::isAudioPort() {
    QString portType(jack_port_type(_jackPort));
    return portType == "audio";
}

bool Port::isMidiPort() {
    QString portType(jack_port_type(_jackPort));
    return portType == "midi";
}

bool Port::isInput() {
    return jack_port_flags(_jackPort) & JackPortIsInput;
}

bool Port::isOutput() {
    return jack_port_flags(_jackPort) & JackPortIsOutput;
}

bool Port::isPhysical() {
    return jack_port_flags(_jackPort) & JackPortIsPhysical;
}

bool Port::canMonitor() {
    return jack_port_flags(_jackPort) & JackPortCanMonitor;
}

bool Port::isTerminal() {
    return jack_port_flags(_jackPort) & JackPortIsTerminal;
}

}
