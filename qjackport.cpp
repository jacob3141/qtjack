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
    _port = port;
}

QJackPort::QJackPort()
{
    _port = 0;
}

QString QJackPort::fullName()
{
    return jack_port_name(_port);
}

QString QJackPort::clientName()
{
    return fullName().split(":").at(0);
}

QString QJackPort::portName()
{
    return jack_port_short_name(_port);
}

QSampleBuffer QJackPort::sampleBuffer()
{
    int bufferSize = QJackClient::instance()->bufferSize();
    return QSampleBuffer(bufferSize, jack_port_get_buffer(_port, bufferSize));
}

bool QJackPort::isAudioPort() {
    QString portType(jack_port_type(_port));
    return portType == "audio";
}

bool QJackPort::isMidiPort() {
    QString portType(jack_port_type(_port));
    return portType == "midi";
}

bool QJackPort::isInput() {
    return jack_port_flags(_port) & JackPortIsInput;
}

bool QJackPort::isOutput() {
    return jack_port_flags(_port) & JackPortIsOutput;
}

bool QJackPort::isPhysical() {
    return jack_port_flags(_port) & JackPortIsPhysical;
}

bool QJackPort::canMonitor() {
    return jack_port_flags(_port) & JackPortCanMonitor;
}

bool QJackPort::isTerminal() {
    return jack_port_flags(_port) & JackPortIsTerminal;
}
