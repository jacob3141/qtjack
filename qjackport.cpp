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

// Own includes
#include <QJackPort>
#include <QJackClient>

// Qt includes
#include <QStringList>

QJackPort::QJackPort(jack_port_t *port)
{
    _jackPort = port;
}

QJackPort::QJackPort()
{
    _jackPort = 0;
}

QString QJackPort::fullName()
{
    return jack_port_name(_jackPort);
}

QString QJackPort::clientName()
{
    return fullName().split(":").at(0);
}

QString QJackPort::portName()
{
    return jack_port_short_name(_jackPort);
}

QJackBuffer QJackPort::sampleBuffer(int samples)
{
    return QJackBuffer(samples, jack_port_get_buffer(_jackPort, samples));
}

bool QJackPort::isAudioPort() {
    QString portType(jack_port_type(_jackPort));
    return portType == "audio";
}

bool QJackPort::isMidiPort() {
    QString portType(jack_port_type(_jackPort));
    return portType == "midi";
}

bool QJackPort::isInput() {
    return jack_port_flags(_jackPort) & JackPortIsInput;
}

bool QJackPort::isOutput() {
    return jack_port_flags(_jackPort) & JackPortIsOutput;
}

bool QJackPort::isPhysical() {
    return jack_port_flags(_jackPort) & JackPortIsPhysical;
}

bool QJackPort::canMonitor() {
    return jack_port_flags(_jackPort) & JackPortCanMonitor;
}

bool QJackPort::isTerminal() {
    return jack_port_flags(_jackPort) & JackPortIsTerminal;
}
