///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    This file is part of QJackAudio.                                       //
//    Copyright (C) 2014 Jacob Dawid, jacob@omg-it.works                     //
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
#include "qjack.h"

// JACK includes
#include <jack/jack.h>

QJack QJack::_instance;

QJack::QJack() {
    jack_set_error_function(QJack::errorCallback);
    jack_set_info_function(QJack::informationCallback);
}

void QJack::emitError(QString errorMessage) {
    emit error(errorMessage);
}

void QJack::emitInformation(QString informationMessage) {
    emit information(informationMessage);
}

QJack *QJack::instance() {
    return &_instance;
}

QString QJack::versionString() {
    return QString(jack_get_version_string());
}

QJack::Version QJack::version() {
    Version version;
    jack_get_version(&version.major,
                     &version.minor,
                     &version.micro,
                     &version.proto);
    return version;
}

void QJack::errorCallback(const char *message) {
    instance()->emitError(QString(message));
}

void QJack::informationCallback(const char *message) {
    instance()->emitInformation(QString(message));
}
