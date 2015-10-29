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

// JACK includes
typedef struct jackctl_parameter jackctl_parameter_t;

// Qt includes
#include <QString>
#include <QMap>
#include <QVariant>

// Own includes
#include "global.h"

namespace QtJack {

class Parameter {
    // Only those should be able to create valid parameter objects.
    friend class Server;
    friend class Driver;
public:
    // We need this for compatibility with QMap.
    Parameter() {
        _jackParameter = 0;
    }

    /** A parameter type. */
    enum ParameterType {
        ParameterTypeInt,
        ParameterTypeUInt,
        ParameterTypeChar,
        ParameterTypeString,
        ParameterTypeBool
    };

    /** A parameter range. */
    struct ParameterValueRange {
        QVariant minimum;
        QVariant maximum;
    };

    bool isValid() const { return _jackParameter != 0; }

    QString name();
    QString shortDescription();
    QString longDescription();
    char id();
    ParameterType type();

    /** @returns true, if this parameters has been set, false otherwise. */
    bool isSet();
    bool reset();

    QVariant value();
    bool setValue(QVariant value);

    QVariant defaultValue();

    bool hasRangeConstraint();
    ParameterValueRange rangeConstraint();
    bool constraintIsStrict();
    bool constraintIsFakeValue();

    bool hasEnumerationConstraint();
    int enumerationConstraintsCount();
    QVariant enumerationConstraintValue(int index);
    QString enumerationConstraintDescription(int index);

private:
    Parameter(jackctl_parameter_t *parameter);

    jackctl_parameter_t *_jackParameter;
};

typedef QMap<QString, Parameter> ParameterMap;

} // namespace QtJack

