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

#include "qamplifier.h"

QAmplifier::QAmplifier(QObject *parent)
    : QDigitalFilter(parent)
{
    _gain = 0.0;
}

void QAmplifier::process(QJackBuffer sampleBuffer)
{
    //bool isClipping = false;
    bool isActive = false;

    _mutex.lock();
    double gain = _gain;
    bool bypass = _bypass;
    _mutex.unlock();

    if(bypass) {
        return;
    }

    isActive = true;
    sampleBuffer.multiply(QUnits::dbToLinear(gain));

//    int bufferSize = sampleBuffer.size();
//    for(int i = 0; i < bufferSize; i++) {
//        // Read audio sample
//        double result = sampleBuffer.readAudioSample(i) * QUnits::dbToLinear(gain);

//        if(result > 1.0) {
//            result = 1.0;
//            isClipping = true;
//        }

//        if(result < -1.0) {
//            result = -1.0;
//            isClipping = true;
//        }
//        sampleBuffer.writeAudioSample(i, result);
//    }

//    if(isClipping) {
//        emit clipping();
//    }

    if(isActive) {
        emit active();
    }
}

double QAmplifier::gain()
{
    QMutexLocker mutexLocker(&_mutex);
    return _gain;
}

void QAmplifier::setGain(double gain)
{
    QMutexLocker mutexLocker(&_mutex);
    _gain = gain;
    emit gainChanged(_gain);
    emit gainChanged((int)_gain);
}
