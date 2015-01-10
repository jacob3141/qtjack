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
#include "netslave.h"

namespace QJack {

NetSlave::NetSlave(QObject *parent)
    : QObject(parent) {
    _jackNetSlave = 0;
}

NetSlave::~NetSlave() {
}

bool NetSlave::open(QString ip, int port, QString name) {
    _jackNetSlave = jack_net_slave_open(ip.toStdString().c_str(),
                                        port,
                                        name.toStdString().c_str(),
                                        &_jackSlave,
                                        &_jackMaster);
    return _jackNetSlave != 0;
}

int NetSlave::close() {
    if(!_jackNetSlave) {
        // Already closed
        return -1;
    }
    return jack_net_slave_close(_jackNetSlave);
}

int NetSlave::activate() {
    if(!_jackNetSlave) {
        return -1;
    }
    return jack_net_slave_activate(_jackNetSlave);
}

int NetSlave::deactivate() {
    if(!_jackNetSlave) {
        return -1;
    }
    return jack_net_slave_deactivate(_jackNetSlave);
}

NetSlave::NetSlave(jack_net_slave_t *jackNetSlave) {
    _jackNetSlave = jackNetSlave;
}

void NetSlave::setProcessor(Processor *audioProcessor) {
    _processor = audioProcessor;
}

}