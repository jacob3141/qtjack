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

// Qt includes
#include <QVector>
#include <QSemaphore>

// Own includes
#include <QJackClient>

/**
 * @class QEqualizer
 * @author Jacob Dawid ( jacob.dawid@omg-it.works )
 * @brief Modifies the frequency spectrum of the sampled audio signal.
 */
class QEqualizer
{
public:
    /** Constructs a new digital equalizer. */
    QEqualizer();

    /** Destructor. */
    ~QEqualizer();

    /** Sets the number of controls in linear frequency steps. */
    void setNumberOfControls(int n);

    /** Returns the number of controls in linear frequency steps. */
    int numberOfControls();

    /** Grants exclusive access access to equalizer controls. Blocks in
      * case anyone else has been accessing these. */
    void acquireControls();

    /** Pointer to the controls array.
      * WARNING: Before accessing controls, make sure you have acquired
      *          exclusive access to these by calling acquireControls.
      *          Also, you have to call releaseControls after you are
      *          done. */
    double *controls();

    /** Releases exclusive access to equalizer controls. */
    void releaseControls();

    /**
      * Updates the filter from the given set of equalizer control values.
      * @param values Equalizer control values.
      */
    void generateFilter();

    /**
      * Processes a given number of samples. In order to function properly,
      * this method expects a consecutive stream of samples. Do not call
      * this method more than once on a given set of samples, since this
      * will lead to erroneous results.
      * @param sampleBuffer Input sample buffer.
      * @param result Result sample buffer.
      * @param samples Number of samples.
      */
    void process(fftw_complex *sampleBuffer, fftw_complex *result, int samples);

private:
    /** Semaphore for accessing the number of equalizer controls. */
    QSemaphore *m_numberOfControlsAccessSemaphore;

    /** Semaphore for accessing the equalizer controls. */
    QSemaphore *m_controlsAccessSemaphore;

    /** Maximum number of controls for which memory should be allocated. */
    static const int MAX_NUMBER_OF_CONTROLS = 2048;

    /** Filter spread. Lower values lead to less computation time. */
    static const int FILTER_SPREAD = 100;

    /** Stores the number of controls. */
    int m_numberOfControls;

    /** The current filter coefficients for the FIR filter. */
    double m_filterCoefficients[FILTER_SPREAD * 2 + 1];

    /**
      * Delay line for the convolution. This memory makes it possible
      * to access previous values and thus continous convolution.
      */
    double m_delayLine[MAX_NUMBER_OF_CONTROLS * 2];

    /** State of the equalizer controls. */
    double m_controls[MAX_NUMBER_OF_CONTROLS];

    /** Memory to compute filter coefficients. Allocated once to avoid
      * memory reallocation, which is pretty expensive. */
    fftw_complex m_idealFilter[MAX_NUMBER_OF_CONTROLS * 2];

    /** Memory to compute filter coefficients. Allocated once to avoid
      * memory reallocation, which is pretty expensive. */
    fftw_complex m_ifftIdealFilter[MAX_NUMBER_OF_CONTROLS * 2];
};

#endif // QEQUALIZER_H
