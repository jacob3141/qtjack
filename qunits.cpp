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
#include <QUnits>
#include <QJackClient>

// Standard includes
#include <cmath>

double QUnits::dbToLinear(double db)
{
   return pow(10.0, db / 20.0);
}

double QUnits::linearToDb(float linear)
{
   return 20.0 * log10(linear);
}

double QUnits::peak(double value)
{
    return value > 0.0 ? value : -value;
}

double QUnits::msToSamples(double ms)
{
    return (double)QJackClient::instance()->sampleRate() * ms / 1000.0 ;
}

double QUnits::samplesToMs(double samples)
{
    return samples * 1000.0 / (double)QJackClient::instance()->sampleRate();
}

double QUnits::sumDb(double valueDb1, double valueDb2)
{
    return linearToDb(dbToLinear(valueDb1) + dbToLinear(valueDb2));
}
