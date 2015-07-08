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

#pragma once

// Own includes
#include "global.h"
#include "parameter.h"

// JACK includes
typedef struct jackctl_driver jackctl_driver_t;

// Qt includes
#include <QString>
#include <QMap>

namespace QtJack {

class Driver {
    friend class Server;
public:
    // We need this for compatibility with QMap.
    Driver() {
        _jackDriver = 0;
    }

    bool isValid() const { return _jackDriver != 0; }

    QString name();
    ParameterMap parameters();

private:
    Driver(jackctl_driver_t *driver);

    jackctl_driver_t *_jackDriver;
};

typedef QMap<QString, Driver> DriverMap;

} // namespace QtJack
