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
#include "qjackparameter.h"

// JACK includes
#include <jack/control.h>

#include <QVariant>

QJackParameter::QJackParameter(jackctl_parameter_t *parameter) {
    _jackParameter = parameter;
}

QString QJackParameter::name() {
    if(!isValid()) {
        return QString();
    }

    return jackctl_parameter_get_name(_jackParameter);
}

QString QJackParameter::shortDescription() {
    if(!isValid()) {
        return QString();
    }

    return jackctl_parameter_get_short_description(_jackParameter);
}

QString QJackParameter::longDescription() {
    if(!isValid()) {
        return QString();
    }

    return jackctl_parameter_get_long_description(_jackParameter);
}

QJackParameter::ParameterType QJackParameter::type() {
    switch (jackctl_parameter_get_type(_jackParameter)) {
    case JackParamInt:
    default:
        return ParameterTypeInt;
    case JackParamUInt:
        return ParameterTypeUInt;
    case JackParamChar:
        return ParameterTypeChar;
    case JackParamString:
        return ParameterTypeString;
    case JackParamBool:
        return ParameterTypeBool;
    }
}

bool QJackParameter::isSet() {
    return isValid() && jackctl_parameter_is_set(_jackParameter);
}

bool QJackParameter::reset() {
    return isValid() && jackctl_parameter_reset(_jackParameter);
}

QVariant QJackParameter::value() {
    if(!isValid()) {
        return QVariant();
    }

    jackctl_parameter_value jackValue = jackctl_parameter_get_value(_jackParameter);
    switch (jackctl_parameter_get_type(_jackParameter)) {
    case JackParamInt:
    default:
        return QVariant(jackValue.i);
    case JackParamUInt:
        return QVariant(jackValue.ui);
    case JackParamChar:
        return QVariant(jackValue.c);
    case JackParamString:
        return QVariant(jackValue.str);
    case JackParamBool:
        return QVariant(jackValue.b);
    }
}

bool QJackParameter::setValue(QVariant value) {
    if(!isValid()) {
        return false;
    }

    bool conversionSuccess;
    jackctl_parameter_value jackValue;
    QString stringValue;

    switch (jackctl_parameter_get_type(_jackParameter)) {
    case JackParamInt:
        jackValue.i = value.toInt(&conversionSuccess);
        return conversionSuccess && jackctl_parameter_set_value(_jackParameter, &jackValue);
        break;
    case JackParamUInt:
        jackValue.ui = value.toUInt(&conversionSuccess);
        return conversionSuccess && jackctl_parameter_set_value(_jackParameter, &jackValue);
        break;
    case JackParamChar:
        jackValue.c = value.toChar().toLatin1();
        return jackValue.c && jackctl_parameter_set_value(_jackParameter, &jackValue);
        break;
    case JackParamString:
        stringValue = value.toString();
        if(stringValue.length() <= JACK_PARAM_STRING_MAX) {
            strcpy(jackValue.str, stringValue.toStdString().c_str());
            return jackctl_parameter_set_value(_jackParameter, &jackValue);
        } else {
            return false;
        }
        break;
    case JackParamBool:
        jackValue.b = value.toBool();
        return jackctl_parameter_set_value(_jackParameter, &jackValue);
        break;
    }

    return false;
}

QVariant QJackParameter::defaultValue() {
    if(!isValid()) {
        return QVariant();
    }

    jackctl_parameter_value jackValue = jackctl_parameter_get_default_value(_jackParameter);
    switch (jackctl_parameter_get_type(_jackParameter)) {
    case JackParamInt:
    default:
        return QVariant(jackValue.i);
    case JackParamUInt:
        return QVariant(jackValue.ui);
    case JackParamChar:
        return QVariant(jackValue.c);
    case JackParamString:
        return QVariant(jackValue.str);
    case JackParamBool:
        return QVariant(jackValue.b);
    }
}
