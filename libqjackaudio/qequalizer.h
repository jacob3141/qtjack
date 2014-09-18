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

/**
 * @class QEqualizer
 * @author Jacob Dawid ( jacob.dawid@omg-it.works )
 * @brief Modifies the frequency spectrum of the sampled audio signal.
 */
class QEqualizer : public QDigitalFilter
{
    Q_OBJECT
public:
    /** Constructs a new digital equalizer. */
    QEqualizer(QObject *parent = 0);

    /** Destructor. */
    ~QEqualizer();

    /**
      * Updates the filter from the given set of equalizer control values.
      * @param values Equalizer control values.
      */
    void computeFilterCoefficients();

    /** @overload QDigitalFilter */
    void process(QSampleBuffer sampleBuffer);

private:
    /** Maximum number of controls for which memory should be allocated. */
    static const int MAX_NUMBER_OF_CONTROLS = 2048;

    /** Filter spread. Lower values lead to less computation time. */
    static const int FILTER_SPREAD = 16;

    /** Stores the number of controls. */
    int _numberOfControls;

    /** The current filter coefficients for the FIR filter. */
    double _filterCoefficients[FILTER_SPREAD * 2 + 1];

    /**
      * Delay line for the convolution. This memory makes it possible
      * to access previous values and thus continous convolution.
      */
    double _delayLine[MAX_NUMBER_OF_CONTROLS * 2];

    /** State of the equalizer controls. */
    double _controls[MAX_NUMBER_OF_CONTROLS];

    /** Memory to compute filter coefficients. Allocated once to avoid
      * memory reallocation, which is pretty expensive. */
    fftw_complex _idealFilter[MAX_NUMBER_OF_CONTROLS * 2];

    /** Memory to compute filter coefficients. Allocated once to avoid
      * memory reallocation, which is pretty expensive. */
    fftw_complex _ifftIdealFilter[MAX_NUMBER_OF_CONTROLS * 2];
};

#endif // QEQUALIZER_H
