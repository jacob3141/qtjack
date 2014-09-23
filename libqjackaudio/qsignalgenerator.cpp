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
#include <QSignalGenerator>

// Standard includes
#include <cmath>

QSignalGenerator::QSignalGenerator(QObject *parent)
    : QDigitalFilter(parent)
{
    _signalType = Triangle;
    _frequency = 1000.0;
    _phase = 0.0;
    _amplitude = 0.0;
    _time = 0.0;
}

void QSignalGenerator::process(QSampleBuffer sampleBuffer)
{
    bool isClipping = false;
    bool isActive = false;

    _mutex.lock();
    SignalType signalType = _signalType;
    double amplitude = QUnits::dbToLinear(_amplitude);
    double frequency = _frequency;
    double phase = _phase;
    bool bypass = _bypass;
    _mutex.unlock();

    if(bypass) {
        return;
    }

    isActive = true;

    double timePerSample = QUnits::samplesToMs(1.0);
    double period = 1000.0 / frequency;

    int bufferSize = sampleBuffer.size();
    for(int i = 0; i < bufferSize; i++) {
        double result = 0.0;

        switch(signalType) {
        case Sine:
            result = amplitude * sin(_time * (2 * M_PI) / period + phase * (2 * M_PI) / 360.0);
            break;
        case Square:
            result = _time < period / 2 ? amplitude : -amplitude;
            break;
        case Triangle:
            if(_time <= period / 4) {
                result = amplitude * _time / (period / 4);
            } else
            if(_time > period / 4 && _time <= period * 3 / 4) {
                result = amplitude - amplitude * (_time - (period / 4)) / (period / 4);
            } else
            if(_time > period * 3 / 4) {
                result = amplitude * (_time - period) / (period / 4);
            }
            break;
        case Sawtooth:
            break;
        case ReverseSawtooth:
            break;
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

        _time += timePerSample;
        if(_time >= period) {
            _time = 0.0;
        }
    }

    if(isClipping) {
        emit clipping();
    }

    if(isActive) {
        emit active();
    }
}

QSignalGenerator::SignalType QSignalGenerator::signalType()
{
    QMutexLocker mutexLocker(&_mutex);
    return _signalType;
}

double QSignalGenerator::phase()
{
    QMutexLocker mutexLocker(&_mutex);
    return _phase;
}

double QSignalGenerator::amplitude()
{
    QMutexLocker mutexLocker(&_mutex);
    return _amplitude;
}

double QSignalGenerator::frequency()
{
    QMutexLocker mutexLocker(&_mutex);
    return _frequency;
}

void QSignalGenerator::setSignalType(SignalType signalType)
{
    QMutexLocker mutexLocker(&_mutex);
    _signalType = signalType;
    emit signalTypeChanged(_signalType);
}

void QSignalGenerator::setPhase(double phase)
{
    QMutexLocker mutexLocker(&_mutex);
    _phase = phase;
    emit phaseChanged(_phase);
    emit phaseChanged((int)_phase);
}

void QSignalGenerator::setAmplitude(double amplitude)
{
    QMutexLocker mutexLocker(&_mutex);
    _amplitude = amplitude;
    emit amplitudeChanged(_amplitude);
    emit amplitudeChanged((int)_amplitude);
}

void QSignalGenerator::setFrequency(double frequency)
{
    QMutexLocker mutexLocker(&_mutex);
    _frequency = frequency;
    emit frequencyChanged(_frequency);
    emit frequencyChanged((int)_frequency);
}
