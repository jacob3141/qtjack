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
#include <QNoiseGate>
#include <QUnits>

// Qt includes
#include <QMutexLocker>

QNoiseGate::QNoiseGate(QObject *parent)
    : QDigitalFilter(parent)
{
    _threshold = -60.0;
}

void QNoiseGate::process(QSampleBuffer sampleBuffer)
{
    bool isClipping = false;
    bool isActive = false;

    _mutex.lock();
    double threshold = _threshold;
    bool bypass = _bypass;
    _mutex.unlock();

    if(bypass) {
        return;
    }

    int bufferSize = sampleBuffer.bufferSize();
    for(int i = 0; i < bufferSize; i++) {
        // Read audio sample
        double sample = sampleBuffer.readAudioSample(i);
        // Determine peak in dB
        double peakDb = QUnits::linearToDb(QUnits::peak(sample));
        double result = sample;

        // Check if peak is under threshold
        if(peakDb < threshold) {
            // Cutoff signal
            isActive = true;
            result = 0.0;
        }

        if(result > 1.0) {
            result = 1.0;
            isClipping = true;
        }

        if(result < -1.0) {
            result = -1.0;
            isClipping = true;
        }

        sampleBuffer.writeAudioSample(i, result);
    }

    if(isClipping) {
        emit clipping();
    }

    if(isActive) {
        emit active();
    }
}

double QNoiseGate::threshold()
{
    QMutexLocker mutexLocker(&_mutex);
    return _threshold;
}

void QNoiseGate::setThreshold(double threshold)
{
    QMutexLocker mutexLocker(&_mutex);
    _threshold = threshold;
    emit thresholdChanged(threshold);
}
