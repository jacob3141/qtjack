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
#include "netmaster.h"

#include <QDebug>

namespace QJack {

NetMaster::NetMaster() {
    _p = QSharedPointer<NetMasterPrivate>(new NetMasterPrivate());
}

NetMaster::NetMaster(const NetMaster& other) {
    _p = other._p;
}

NetMaster::~NetMaster() {
}

bool NetMaster::open(QString ip,
                     int port,
                     QString name) {

    _p->_jackMaster.audio_input = -1;
    _p->_jackMaster.audio_output = -1;
    strcpy(_p->_jackMaster.master_name, name.toStdString().c_str());
    qDebug() << "a";
    _p->_jackNetMaster = jack_net_master_open(ip.toStdString().c_str(),
                                              port,
                                              name.toStdString().c_str(),
                                              &_p->_jackMaster,
                                              &_p->_jackSlave);
    qDebug() << "b";
    return _p->_jackNetMaster != 0;
}

int NetMaster::close() {
    if(!_p->_jackNetMaster) {
        return -1;
    }
    return jack_net_master_close(_p->_jackNetMaster);
}

int NetMaster::synchronizeInputBuffers(int numberOfAudioBuffers,
                                       float** audioBuffers,
                                       int numberOfMidiBuffers,
                                       void** midiBuffers) {
    if(!_p->_jackNetMaster) {
        return -1;
    }

    return jack_net_master_recv(_p->_jackNetMaster,
                                numberOfAudioBuffers,
                                audioBuffers,
                                numberOfMidiBuffers,
                                midiBuffers);
}

int NetMaster::synchronizeOutputBuffers(int numberOfAudioBuffers,
                                        float** audioBuffers,
                                        int numberOfMidiBuffers,
                                        void** midiBuffers) {
    if(!_p->_jackNetMaster) {
        return -1;
    }

    return jack_net_master_send(_p->_jackNetMaster,
                                numberOfAudioBuffers,
                                audioBuffers,
                                numberOfMidiBuffers,
                                midiBuffers);
}

}
