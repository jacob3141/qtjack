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

#ifndef QEQUALIZER_H
#define QEQUALIZER_H

// Own includes
#include <QJackClient>
#include <QDigitalFilter>
#include <QEqualizerControl>

/**
 * @class QEqualizer
 * @author Jacob Dawid ( jacob.dawid@omg-it.works )
 * @brief Modifies the frequency spectrum of the sampled audio signal.
 */
class QEqualizer : public QDigitalFilter
{
    Q_OBJECT
public:
    /**
     * Constructs a new digital equalizer. convResolution may not be greater than
     * half the filter resolution.
     * @param resolution Total resolution of the equalizer.
     * @param convResolution Resulting filter size of the filter that will be applid
     * to the signal.
     * @param parent
     */
    QEqualizer(int resolution = 2048, int convResolution = 256, QObject *parent = 0);

    /** Destructor. */
    ~QEqualizer();

    /** @overload QDigitalFilter */
    void process(QSampleBuffer sampleBuffer);

    QEqualizerControl* createEqualizerControl(QEqualizerControl::ControlType controlType = QEqualizerControl::Band);

public slots:
    /**
      * Updates the filter from the given set of equalizer control values.
      * @param values Equalizer control values.
      */
    void update();

private:
    QList<QEqualizerControl*> _equalizerControls;

    /** The current filter coefficients for the FIR filter.
      * These will be convoluted with the signals and need to be calculated
      * beforehand.
      */
    int _filterCoefficientsSize;
    double *_filterCoefficients;

    /**
      * Delay line for the convolution. This memory makes it possible
      * to access previous values and thus continous convolution.
      */
    int _delayLineSize;
    double *_delayLine;

    /** State of the equalizer controls. */
    int _controlsSize;
    double *_controls;
};

#endif // QEQUALIZER_H
