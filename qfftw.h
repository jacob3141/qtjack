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

#pragma once

// JACK includes:
#include <jack/jack.h>

// FFTW3 includes:
#include "fftw3.h"

namespace QFFTW {
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
