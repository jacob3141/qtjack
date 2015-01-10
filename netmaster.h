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
#include <jack/net.h>

// Qt includes
#include <QString>
#include <QSharedPointer>

namespace QJack {

class NetMasterPrivate;
class NetMaster {
public:
   NetMaster();
   NetMaster(const NetMaster& other);

   virtual ~NetMaster();

   /**
    * Opens a server for this master.
    * @param ip The IP address for this master.
    * @param port The port for this master.
    * @param name The name of this master.
    * @returns true on success.
    */
   bool open(QString ip,
             int port,
             QString name);

   /**
    * Closes this master.
    * @returns 0 on success, an error code otherwise.
    */
   int close();

   /**
    * @brief synchronizeInputBuffers
    * @param numberOfAudioBuffers
    * @param audioBuffers
    * @param numberOfMidiBuffers
    * @param midiBuffers
    * @returns 0 on success, an error code otherwise.
    */
   int synchronizeInputBuffers(int numberOfAudioBuffers,
                               float** audioBuffers,
                               int numberOfMidiBuffers,
                               void** midiBuffers);

   /**
    * @brief synchronizeOutputBuffers
    * @param numberOfAudioBuffers
    * @param audioBuffers
    * @param numberOfMidiBuffers
    * @param midiBuffers
    * @returns 0 on success, an error code otherwise.
    */
   int synchronizeOutputBuffers(int numberOfAudioBuffers,
                                float** audioBuffers,
                                int numberOfMidiBuffers,
                                void** midiBuffers);

//   int numberOfAudioInputs();
//   int numberOfAudioOutputs();
//   int numberOfMidiInputs();
//   int numberOfMidiOutputs();
//   int maximumTransmissionUnit();
//   int timeOut();
//   int encoderType();
//   int celtKbps();
//   int latency();

private:
   QSharedPointer<NetMasterPrivate> _p;
};

class NetMasterPrivate {
public:
    NetMasterPrivate() {
        _jackNetMaster = 0;
    }

    virtual ~NetMasterPrivate() {
        jack_net_master_close(_jackNetMaster);
    }

    jack_net_master_t *_jackNetMaster;

    jack_master_t _jackMaster;
    jack_slave_t _jackSlave;
};

}
