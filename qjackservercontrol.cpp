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
#include "qjackservercontrol.h"

#include <jack/control.h>

QJackServerControl::QJackServerControl() {
    _jackServerHandle = jackctl_server_create(0, 0);
}

QJackServerControl::~QJackServerControl() {
    jackctl_server_destroy(_jackServerHandle);
}

bool QJackServerControl::open(QJackDriver driver) {
    return jackctl_server_open(_jackServerHandle, driver._jackDriver);
}

bool QJackServerControl::close() {
    return _jackServerHandle && jackctl_server_close(_jackServerHandle);
}

bool QJackServerControl::start() {
    return _jackServerHandle && jackctl_server_start(_jackServerHandle);
}

bool QJackServerControl::stop() {
    return _jackServerHandle && jackctl_server_stop(_jackServerHandle);
}

QList<QJackDriver> QJackServerControl::availableDrivers() {
    QList<QJackDriver> driversList;
    const JSList *drivers = jackctl_server_get_drivers_list(_jackServerHandle);
    while(drivers) {
        driversList.append(QJackDriver((jackctl_driver_t*)drivers->data));
        drivers = drivers->next;
    }
    return driversList;
}

QList<QJackParameter> QJackServerControl::parameters() {
    QList<QJackParameter> parameterList;
    const JSList *parameters = jackctl_server_get_parameters(_jackServerHandle);
    while(parameters) {
        parameterList.append(QJackParameter((jackctl_parameter_t*)parameters->data));
        parameters = parameters->next;
    }
    return parameterList;
}
