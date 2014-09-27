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
#include <QEqualizerControl>
#include <QUnits>

// Standard includes
#include <cmath>

QEqualizerControl::QEqualizerControl(QEqualizer *equalizer, ControlType controlType, QObject *parent)
    : QObject(parent)
{
    _controlType = controlType;
    _equalizer = equalizer;

    _q = 1.0;
    _amount = 0.0;
    _controlFrequency = 1000.0;
}

double QEqualizerControl::gainForFrequencyLowShelf(double frequency)
{
    if(frequency < _controlFrequency) {
        return _amount;
    } else {
        double topFrequency = _controlFrequency + _controlFrequency * pow(0.5, _q);
        double p = (frequency - _controlFrequency) * M_PI / (topFrequency - _controlFrequency);
        double scaledAmount = _amount * (cos(p) / 2.0 + 0.5 );
        return scaledAmount;
    }
}

double QEqualizerControl::gainForFrequencyBand(double frequency)
{
    double edgeFrequencyTop = _controlFrequency + _bandwidth / 2;
    double edgeFrequencyBottom = _controlFrequency - _bandwidth / 2;

    if(frequency > edgeFrequencyBottom
    || frequency < edgeFrequencyTop) {
        return _amount;
    } else {
        if(frequency >= edgeFrequencyTop) {
            double topFrequency = edgeFrequencyTop + edgeFrequencyTop * pow(0.5, _q);
            double p = (frequency - edgeFrequencyTop) * M_PI / (topFrequency - edgeFrequencyTop);
            double scaledAmount = _amount * (cos(p) / 2.0 + 0.5);
            return scaledAmount;
        } else {
        // (frequency <= edgeFrequencyBottom) {
            double bottomFrequency = edgeFrequencyBottom - edgeFrequencyBottom * pow(0.5, _q);
            double p = (frequency - edgeFrequencyBottom) * M_PI / (bottomFrequency - edgeFrequencyBottom);
            double scaledAmount = _amount * (cos(p) / 2.0 + 0.5);
            return scaledAmount;
        }
    }
}

double QEqualizerControl::gainForFrequencyHighShelf(double frequency)
{
    if(frequency > _controlFrequency) {
        return _amount;
    } else {
        double bottomFrequency = _controlFrequency - _controlFrequency * pow(0.5, _q);
        double p = (frequency - _controlFrequency) * M_PI / (bottomFrequency - _controlFrequency);
        double scaledAmount = _amount * (cos(p) / 2.0 + 0.5 );
        return scaledAmount;
    }
}

double QEqualizerControl::gainForFrequency(double frequency)
{
    switch(_controlType) {
    case LowShelf:
        return gainForFrequencyLowShelf(frequency);
        break;
    case Band:
        return gainForFrequencyBand(frequency);
        break;
    case HighShelf:
        return gainForFrequencyHighShelf(frequency);
        break;
    }

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

double QEqualizerControl::bandwidth()
{
    return _bandwidth;
}

void QEqualizerControl::setControlType(ControlType controlType)
{
    _controlType = controlType;
    emit controlTypeChanged(_controlType);
    emit controlChanged();
}

void QEqualizerControl::setAmount(double amount)
{
    _amount = amount;
    emit amountChanged(_amount);
    emit amountChanged((int)_amount);
    emit controlChanged();
}

void QEqualizerControl::setQ(double q)
{
    _q = q;
    emit qChanged(_q);
    emit qChanged((int)_q);
    emit controlChanged();
}

void QEqualizerControl::setControlFrequency(double controlFrequency)
{
    _controlFrequency = controlFrequency;
    emit controlFrequencyChanged(_controlFrequency);
    emit controlFrequencyChanged((int)_controlFrequency);
    emit controlChanged();
}

void QEqualizerControl::setBandwidth(double bandwidth)
{
    _bandwidth = bandwidth;
    emit bandwidthChanged(_bandwidth);
    emit bandwidthChanged((int)_bandwidth);
    emit controlChanged();
}
