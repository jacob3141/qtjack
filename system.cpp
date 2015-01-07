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
#include "system.h"

// JACK includes
#include <jack/jack.h>

namespace QJack {

System System::_instance;

System::System() {
    jack_set_error_function(System::errorCallback);
    jack_set_info_function(System::informationCallback);
}

void System::emitError(QString errorMessage) {
    emit error(errorMessage);
}

void System::emitInformation(QString informationMessage) {
    emit information(informationMessage);
}

System *System::instance() {
    return &_instance;
}

QString System::versionString() {
    return QString(jack_get_version_string());
}

System::Version System::version() {
    Version version;
    jack_get_version(&version.major,
                     &version.minor,
                     &version.micro,
                     &version.proto);
    return version;
}

void System::errorCallback(const char *message) {
    instance()->emitError(QString(message));
}

void System::informationCallback(const char *message) {
    instance()->emitInformation(QString(message));
}

}
