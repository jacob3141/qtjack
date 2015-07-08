///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    This file is part of QtJack.                                           //
//    Copyright (C) 2014-2015 Jacob Dawid <jacob@omg-it.works>               //
//                                                                           //
//    QtJack is free software: you can redistribute it and/or modify         //
//    it under the terms of the GNU General Public License as published by   //
//    the Free Software Foundation, either version 3 of the License, or      //
//    (at your option) any later version.                                    //
//                                                                           //
//    QtJack is distributed in the hope that it will be useful,              //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of         //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
//    GNU General Public License for more details.                           //
//                                                                           //
//    You should have received a copy of the GNU General Public License      //
//    along with QtJack. If not, see <http://www.gnu.org/licenses/>.         //
//                                                                           //
//    It is possible to obtain a closed-source license of QtJack.            //
//    If you're interested, contact me at: jacob@omg-it.works                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// Own includes
#include "server.h"

// JACK includes
#include <jack/control.h>

namespace QtJack {

Server::Server() {
    _jackServer = jackctl_server_create(0, 0);
}

Server::~Server() {
    jackctl_server_destroy(_jackServer);
}

bool Server::start(Driver driver) {
    return isValid()
        && driver.isValid()
        // Note: Slightly different API than JACK1
        && jackctl_server_open(_jackServer, driver._jackDriver)
        && jackctl_server_start(_jackServer);
}

bool Server::stop() {
    return isValid()
        // Note: Slightly different API than JACK1
        && jackctl_server_stop(_jackServer)
        && jackctl_server_close(_jackServer);
}

DriverMap Server::availableDrivers() const {
    if(!isValid()) {
        return DriverMap();
    }

    DriverMap driversMap;
    const JSList *drivers = jackctl_server_get_drivers_list(_jackServer);
    while(drivers) {
        Driver d = Driver((jackctl_driver_t*)drivers->data);
        driversMap.insert(d.name(), d);
        drivers = drivers->next;
    }
    return driversMap;
}

ParameterMap Server::parameters() const {
    if(!isValid()) {
        return ParameterMap();
    }

    ParameterMap parameterMap;
    const JSList *parameters = jackctl_server_get_parameters(_jackServer);
    while(parameters) {
        Parameter p = Parameter((jackctl_parameter_t*)parameters->data);
        parameterMap.insert(p.name(), p);
        parameters = parameters->next;
    }
    return parameterMap;
}

} // namespace QtJack
