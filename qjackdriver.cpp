///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    This file is part of QJackAudio.                                       //
//    Copyright (C) 2015 Jacob Dawid, jacob@omg-it.works                     //
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
#include "qjackdriver.h"

#include <jack/control.h>

QJackDriver::QJackDriver(jackctl_driver_t *driver)
{
    _jackDriver = driver;
}

QString QJackDriver::name() {
    return QString(jackctl_driver_get_name(_jackDriver));
}

QJackDriver::DriverType QJackDriver::type() {
    switch (jackctl_driver_get_type(_jackDriver)) {
    case JackMaster:
    default:
        return DriverTypeMaster;
    case JackSlave:
        return DriverTypeSlave;
    }
}
