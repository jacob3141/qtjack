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

#ifndef QNOISEGATE_H
#define QNOISEGATE_H

// Own includes
#include <QDigitalFilter>

// Qt includes
#include <QMutex>

/**
 * @class QNoiseGate
 * @author Jacob Dawid ( jacob.dawid@omg-it.works )
 * @brief Noise gate.
 */
class QNoiseGate : public QDigitalFilter
{
    Q_OBJECT
public:
    /** Constructs a new noise gate. */
    QNoiseGate(QObject *parent = 0);

    /** @returns the noise gate threshold in dB. */
    double threshold();

    /** @overload @see QDigitalFilter */
    void process(QSampleBuffer sampleBuffer);

signals:
    /** Emitted whenever the threshold has changed. */
    void thresholdChanged(double threshold);
    void thresholdChanged(int threshold);

    void sensitivityChanged(double sensitivity);
    void sensitivityChanged(int sensitivity);

    void resistanceChanged(double resistance);
    void resistanceChanged(int resistance);

public slots:
    /**
     * Set the threshold for the noise gate in dB. The noise gate will kill
     * all signals below threshold.
     * @brief setThreshold
     * @param threshold Threshold in dB.
     */
    void setThreshold(double threshold);
    void setThreshold(int threshold) { setThreshold((double)threshold); }

    void setSensitivy(double sensitivity);
    void setSensitivy(int sensitivity) { setSensitivy((double)sensitivity); }

    void setResistance(double resistance);
    void setResistance(int resistance) { setResistance((double)resistance); }

private:
    /** Noise gate threshold in dB. */
    double _threshold;

    /** Sensitivity determines how long the signal must be below threshold
     * before the noise gate kicks in. In ms.
     */
    double _sensitivity;

    /** Resistance determines how long the signal must be above threshold
     * in order to disable the noise gate. In ms.
     */
    double _resistance;

    bool _muting;
    int _sampleCount;
};

#endif // QNOISEGATE_H
