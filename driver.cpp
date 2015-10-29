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
#include "driver.h"

// JACK includes
#include <jack/control.h>

namespace QtJack {

Driver::Driver(jackctl_driver_t *driver)
{
    _jackDriver = driver;
}

QString Driver::name() {
    if(!isValid()) {
        return QString();
    }

    return QString(jackctl_driver_get_name(_jackDriver));
}

ParameterMap Driver::parameters() {
    if(!isValid()) {
        return ParameterMap();
    }

    ParameterMap parameterMap;
    const JSList *parameters = jackctl_driver_get_parameters(_jackDriver);
    while(parameters) {
        Parameter p = Parameter((jackctl_parameter_t*)parameters->data);
        parameterMap.insert(p.name(), p);
        parameters = parameters->next;
    }
    return parameterMap;
}

} // namespace QtJack
