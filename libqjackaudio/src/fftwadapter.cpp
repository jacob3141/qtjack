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

// FFTW3 includes:
#include "fftwadapter.h"

void FftwAdapter::blit(fftw_complex *fftw_complexIn,
                       jack_default_audio_sample_t *jack_default_audio_sample_tsOut,
                       int n) {
    for(int i = 0; i < n; i++) {
        jack_default_audio_sample_tsOut[i] = fftw_complexIn[i][0];
   }
}

void FftwAdapter::blit(jack_default_audio_sample_t *jack_default_audio_sample_tsIn,
                       fftw_complex *fftw_complexOut,
                       int n) {
    for(int i = 0; i < n; i++) {
        fftw_complexOut[i][0] = jack_default_audio_sample_tsIn[i]; // SEGFAULT...
        fftw_complexOut[i][1] = 0;
    }
}

void FftwAdapter::blit(jack_default_audio_sample_t *jack_default_audio_sample_tsIn,
                       jack_default_audio_sample_t *jack_default_audio_sample_tsOut,
                       int n) {
    for(int i = 0; i < n; i++) {
        jack_default_audio_sample_tsOut[i] = jack_default_audio_sample_tsIn[i];
   }
}

void FftwAdapter::blit(fftw_complex *fftw_complexIn,
                       fftw_complex *fftw_complexOut,
                       int n) {
    for(int i = 0; i < n; i++) {
        fftw_complexOut[i][0] = fftw_complexIn[i][0];
        fftw_complexOut[i][1] = fftw_complexIn[i][1];
    }
}

void FftwAdapter::performFFT(fftw_complex *input, fftw_complex *result, int n) {
    fftw_plan plan = fftw_plan_dft_1d(n, input, result,
                                      FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
}

void FftwAdapter::performInverseFFT(fftw_complex *input, fftw_complex *result, int n) {
    fftw_plan plan = fftw_plan_dft_1d(n, input, result,
                                      FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);

    for(int i = 0; i < n; i++) {
        result[i][0] /= (double)n;
        result[i][1] /= (double)n;
    }
}
