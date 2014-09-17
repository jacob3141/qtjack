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
#include <QEqualizer>
#include <QFFTW>

// Qt includes
#include <cmath>
#include <QStringList>

QEqualizer::QEqualizer()
{
    for(int i = 0; i < MAX_NUMBER_OF_CONTROLS * 2; i++) {
        _delayLine[i] = 0.0;
    }

    for(int i = 0; i < MAX_NUMBER_OF_CONTROLS; i++) {
        _controls[i] = 1.0;
    }

    computeFilterCoefficients();
}

QEqualizer::~QEqualizer() {
}

void QEqualizer::computeFilterCoefficients()
{
    _numberOfControls = QJackClient::instance()->bufferSize() / 2;

    // Control values in frequency domain:
    // amplitude
    //
    // A
    // |
    // |                ooooooooooooo                o
    // |       ooooooooo             oooo          oo
    // |   oooo                          ooo    ooo
    // | oo                                 oooo
    // |o
    // |
    // |
    // |
    // |
    // +------------------------------------------------> frequency

    // Construct an ideal filter in the frequency domain.
    for(int i = 0; i < _numberOfControls * 2; i++) {
        if(i < _numberOfControls) {
            // "Draw" frequency response for the equalizer.
            _idealFilter[i][0] = _controls[i];
            _idealFilter[i][1] = 0.0;
        } else {
            // Mirror frequency response for the second half.
            _idealFilter[i][0] = _controls[_numberOfControls * 2 - 1 - i];
            _idealFilter[i][1] = 0.0;
        }
    }

    // Translate into the time domain.
    QFFTW::performInverseFFT(_idealFilter, _ifftIdealFilter, _numberOfControls * 2);

    // Time domain signal after inverse DFT:
    // value
    //
    // A------------------|FILTER_SPREAD
    // |o                                              o
    // | o                                            o
    // |
    // |  o        o                        o        o
    // |          o o                      o o
    // |   o                                        o
    // |         o   o                    o   o
    // |    o             o          o             o
    // |     o  o     o  o o o   o  o o  o     o  o
    // |      oo       oo   o ooo o    oo       oo
    // +------------------------------------------------> coefficients

    // Shift and cut coefficients in order to use as a filter.
    for(int i = 0; i < FILTER_SPREAD * 2 + 1; i++) {
        if(i < FILTER_SPREAD) {
            _filterCoefficients[i] = _ifftIdealFilter[_numberOfControls * 2 - FILTER_SPREAD + i][0];
        } else {
            _filterCoefficients[i] = _ifftIdealFilter[i - FILTER_SPREAD][0];
        }
    }

    // Lower filter coefficients by cutting of samples (determined by FILTER_SPREAD)
    // and shift result:
    // value
    //
    // A
    // |                     o
    // |                    o o
    // |
    // |       o           o   o         o
    // |      o o                       o o
    // |                  o     o
    // |     o   o                     o   o
    // |o               o         o             o
    // | o  o     o    o           o  o     o  o
    // |  oo        oo              oo       oo
    // +------------------------------------------------> coefficients

    // Apply a hamming window
    for(int i = -FILTER_SPREAD; i <= FILTER_SPREAD; i++)
        _filterCoefficients[i + FILTER_SPREAD] *= (0.54 + 0.46 * cos(M_PI * i / FILTER_SPREAD));

    // Apply a hamming windows to smooth the filter, which improves the frequency response a lot:
    // value
    //
    // A
    // |
    // |                     o
    // |                    o o
    // |                   o   o
    // |
    // |                  o     o
    // |       o                         o
    // |     o   o      o         o    o   o
    // |    o     o    o           o  o     o
    // |oooo        oo              oo       oooo
    // +------------------------------------------------> coefficients
}

void QEqualizer::process(QSampleBuffer sampleBuffer)
{
    int bufferSize = sampleBuffer.bufferSize();
    for(int i = 0; i < bufferSize; i++) {
        double result = 0.0;
        _delayLine[0] = sampleBuffer.readAudioSample(i);

        for(int j = 0; j < FILTER_SPREAD * 2 + 1; j++)
            result += _filterCoefficients[j] * _delayLine[j];
        sampleBuffer.writeAudioSample(i, result);

        for(int j = _numberOfControls - 2; j >= 0; j--)
            _delayLine[j + 1] = _delayLine[j];
    }
}

