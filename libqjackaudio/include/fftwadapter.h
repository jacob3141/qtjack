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

#ifndef FFTWADAPTER_H
#define FFTWADAPTER_H

// JACK includes:
#include <jack/jack.h>

// FFTW3 includes:
#include "fftw3.h"

namespace FftwAdapter {
  /**
    * fftw works with arrays of fftw_complex numbers. In order to use fftw,
    * you have to convert between JACK samples, which are mere real numbers
    * and fftw_complex arrays.
    * This method copies over from fftw_complex to JACK sample values.
    * @param fftw_complexIn Input array of fftw_complex numbers.
    * @param jack_default_audio_sample_tsOut Output array of JACK samples.
    * @param n Number of samples.
    */
  void blit(fftw_complex *fftw_complexIn, jack_default_audio_sample_t *jack_default_audio_sample_tsOut, int n);

  /**
    * fftw works with arrays of fftw_complex numbers. In order to use fftw,
    * you have to convert between JACK samples, which are mere real numbers
    * and fftw_complex arrays.
    * This method copies over from JACK sample to fftw_complex values.
    * @param jack_default_audio_sample_tsIn Input array of fftw_complex numbers.
    * @param fftw_complexOut Output array of JACK samples.
    * @param n Number of samples.
    */
  void blit(jack_default_audio_sample_t *jack_default_audio_sample_tsIn, fftw_complex *fftw_complexOut, int n);
  void blit(jack_default_audio_sample_t *jack_default_audio_sample_tsIn, jack_default_audio_sample_t *jack_default_audio_sample_tsOut, int n);
  void blit(fftw_complex *fftw_complexIn, fftw_complex *fftw_complexOut, int n);

  /**
    * Performs the fft.
    * @param fftw_complex Input array of comples numbers.
    * @param result Output array of fftw_complex numbers.
    * @param n Number of samples.
    */
  void performFFT(fftw_complex *input, fftw_complex *result, int n);

  /**
    * Performs the inverse fft.
    * @param fftw_complex Input array of comples numbers.
    * @param result Output array of fftw_complex numbers.
    * @param n Number of samples.
    */
  void performInverseFFT(fftw_complex *input, fftw_complex *result, int n);
}

#endif // FFTWADAPTER_H
