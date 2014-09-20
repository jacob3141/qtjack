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

#ifndef QSIGNALGENERATOR_H
#define QSIGNALGENERATOR_H

// Own includes
#include <QDigitalFilter>

class QSignalGenerator : public QDigitalFilter
{
    Q_OBJECT
public:
    enum SignalType {
        Sine,
        Square,
        Triangle,
        Sawtooth,
        ReverseSawtooth
    };

    QSignalGenerator(QObject *parent = 0);

    /** @overload @see QDigitalFilter */
    void process(QSampleBuffer sampleBuffer);

    SignalType signalType();
    double phase();
    double amplitude();
    double frequency();

signals:
    void signalTypeChanged(SignalType signalType);
    void phaseChanged(double phase);
    void phaseChanged(int phase);

    void amplitudeChanged(double amplitude);
    void amplitudeChanged(int amplitude);

    void frequencyChanged(double frequency);
    void frequencyChanged(int frequency);

public slots:
    void setSignalType(SignalType signalType);

    void setPhase(double phase);
    void setPhase(int phase) { setPhase((double)phase); }

    void setAmplitude(double amplitude);
    void setAmplitude(int amplitude) { setAmplitude((double)amplitude); }

    void setFrequency(double frequency);
    void setFrequency(int frequency) { setFrequency((double)frequency); }

private:
    double _time;

    /** Signal type. */
    SignalType _signalType;

    /** Phase shift in degrees. */
    double _phase;

    /** Amplitude in dB. */
    double _amplitude;

    /** Signal frequency in Hz. */
    double _frequency;
};

#endif // QSIGNALGENERATOR_H
