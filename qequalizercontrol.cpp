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

#include "qequalizercontrol.h"

QEqualizerControl::QEqualizerControl(QEqualizer *equalizer, ControlType controlType, QObject *parent)
    : QObject(parent)
{
    _controlType = controlType;
    _equalizer = equalizer;

    _q = 1.0;
    _amount = 0.0;
    _controlFrequency = 1000.0;
}


double QEqualizerControl::gainForFrequency(double frequency)
{
    return 0.0;
}

QEqualizerControl::ControlType QEqualizerControl::controlType()
{
    return _controlType;
}

double QEqualizerControl::controlFrequency()
{
    return _controlFrequency;
}

double QEqualizerControl::amount()
{
    return _amount;
}

double QEqualizerControl::q()
{
    return _q;
}
