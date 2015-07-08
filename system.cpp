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
#include "global.h"
#include "system.h"

// JACK includes
#include <jack/jack.h>

namespace QtJack {

System System::_instance;

System::System() {
    jack_set_error_function(System::errorCallback);
    jack_set_info_function(System::informationCallback);
}

void System::emitError(QString errorMessage) {
    Q_EMIT error(errorMessage);
}

void System::emitInformation(QString informationMessage) {
    Q_EMIT information(informationMessage);
}

System *System::instance() {
    return &_instance;
}

void System::errorCallback(const char *message) {
    instance()->emitError(QString(message));
}

void System::informationCallback(const char *message) {
    instance()->emitInformation(QString(message));
}

} // namespace QtJack
