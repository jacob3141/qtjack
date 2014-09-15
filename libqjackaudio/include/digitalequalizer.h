/* This file is part of EAR, an audio processing tool.
 *
 * Copyright (C) 2011 Otto Ritter, Jacob Dawid
 * otto.ritter.or@googlemail.com
 * jacob.dawid@cybercatalyst.net
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the Affero GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the Affero GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EQUALIZER_H
#define EQUALIZER_H

#include <QVector>
#include <QSemaphore>
#include "jackadapter.h"

/**
 * @class DigitalEqualizer
 *
 * @author Jacob Dawid ( jacob.dawid@cybercatalyst.net )
 * @author Otto Ritter ( otto.ritter.or@googlemail.com )
 * @date 09.2011
 *
 * @brief Modifies the frequency spectrum of the sampled audio signal.
 */
class DigitalEqualizer
{
public:
    /** Constructs a new digital equalizer. */
    DigitalEqualizer();

    /** Destructor. */
    ~DigitalEqualizer();

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

    /** Attempts to write control values into a file.
      * @param fileName File name of the file that shall be saved.
      * @return true on success, otherwise false. */
    bool saveControlsToFile(QString fileName);

    /** Attempts to restore control values from a file.
      * @param fileName File name of the file form which shall be loaded.
      * @return true on success, otherwise false. */
    bool loadControlsFromFile(QString fileName);

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
    /** Serializes equalizer state into a string. */
    QString serializeCSV();

    /** Attempts to restore equalizer state based on the given string. */
    void unserializeCSV(QString stream);

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

#endif // EQUALIZER_H
