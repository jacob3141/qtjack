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

char QJackParameter::id() {
    if(!isValid()) {
        return 0;
    }
    return jackctl_parameter_get_id(_jackParameter);
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

bool QJackParameter::hasRangeConstraint() {
    return isValid() && jackctl_parameter_has_range_constraint(_jackParameter);
}


QJackParameter::ParameterValueRange QJackParameter::rangeConstraint() {
    if(!isValid()) {
        return ParameterValueRange();
    }

    ParameterValueRange parameterValueRange;
    jackctl_parameter_value min, max;
    jackctl_parameter_get_range_constraint(_jackParameter, &min, &max);

    switch (type()) {
    case ParameterTypeBool:
        parameterValueRange.minimum = QVariant(min.b);
        parameterValueRange.maximum = QVariant(max.b);
        break;
    case ParameterTypeChar:
        parameterValueRange.minimum = QVariant(min.c);
        parameterValueRange.maximum = QVariant(max.c);
        break;
    case ParameterTypeInt:
        parameterValueRange.minimum = QVariant(min.i);
        parameterValueRange.maximum = QVariant(max.i);
        break;
    case ParameterTypeString:
        parameterValueRange.minimum = QVariant(min.str);
        parameterValueRange.maximum = QVariant(max.str);
        break;
    case ParameterTypeUInt:
        parameterValueRange.minimum = QVariant(min.ui);
        parameterValueRange.maximum = QVariant(max.ui);
        break;
    }

    return parameterValueRange;
}

bool QJackParameter::constraintIsStrict() {
    return isValid() && jackctl_parameter_constraint_is_strict(_jackParameter);
}

bool QJackParameter::constraintIsFakeValue() {
    return isValid() && jackctl_parameter_constraint_is_fake_value(_jackParameter);
}

bool QJackParameter::hasEnumerationConstraint() {
    return isValid() && jackctl_parameter_has_enum_constraint(_jackParameter);
}

int QJackParameter::enumerationConstraintsCount() {
    return isValid() && jackctl_parameter_get_enum_constraints_count(_jackParameter);
}

QVariant QJackParameter::enumerationConstraintValue(int index) {
    if(!isValid()) {
        return 0;
    }

    jackctl_parameter_value jackParameterValue = jackctl_parameter_get_enum_constraint_value(_jackParameter, index);
    QVariant value;

    switch (type()) {
    case ParameterTypeBool:
        value = jackParameterValue.b;
        break;
    case ParameterTypeChar:
        value = jackParameterValue.c;
        break;
    case ParameterTypeInt:
        value = jackParameterValue.i;
        break;
    case ParameterTypeString:
        value = jackParameterValue.str;
        break;
    case ParameterTypeUInt:
        value = jackParameterValue.ui;
        break;
    }
    return value;
}

QString QJackParameter::enumerationConstraintDescription(int index) {
    if(!isValid()) {
        return QString();
    }

    return jackctl_parameter_get_enum_constraint_description(_jackParameter, index);
}
